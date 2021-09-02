/******************************************************************************
 * FILENAME: mem.c
 * AUTHOR:   cherin@cs.wisc.edu <Cherin Joseph>
 * DATE:     20 Nov 2013
 * PROVIDES: Contains a set of library functions for memory allocation
 * *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

int fit; // fit polilcy flag

/* this structure serves as the header for each block */
typedef struct block_hd{
	/* The blocks are maintained as a linked list */
	/* The blocks are ordered in the increasing order of addresses */
	struct block_hd* next;

	/* size of the block is always a multiple of 4 */
	/* ie, last two bits are always zero - can be used to store other information*/
	/* LSB = 0 => free block */
	/* LSB = 1 => allocated/busy block */
	/* LSB: 最下位Bit */


	/* So for a free block, the value stored in size_status will be the same as the block size*/
	/* And for an allocated block, the value stored in size_status will be one more than the block size*/

	/* The value stored here does not include the space required to store the header */

	/* Example: */
	/* For a block with a payload of 24 bytes (ie, 24 bytes data + an additional 8 bytes for header) */
	/* If the block is allocated, size_status should be set to 25, not 24!, not 23! not 32! not 33!, not 31! */
	/* If the block is free, size_status should be set to 24, not 25!, not 23! not 32! not 33!, not 31! */
	int size_status;

	// メモリのサイズと次のブロックヘッダへのポインタで構成

}block_header;

/* Global variable - This will always point to the first block */
/* ie, the block with the lowest address */
block_header* list_head = NULL;
/* list_head: 最初にある唯一のヘッダ。いろんな情報入れてある（←通常は）
*/


/* Function used to Initialize the memory allocator */
/* Not intended to be called more than once by a program */
/* Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated 
	 				policy: indicates the policy to use eg: best fit is 0*/
/* Returns 0 on success and -1 on failure */
int Mem_Init(int sizeOfRegion,  int policy)
// sizeOfRegion: 初期化を要求されるバイト数＝メモリ全体。なので大きくなるはず
// OSはページ単位で割り当てるので、ページ単位になる
// policy: 2ならワーストフィット、など、フィットの考え方
// Mem_Initは、mmap()システムコールを呼び出してヒープ上の空間を初期化する
/*
これにより、モジュールが管理するヒープ空間の設定と初期化が行われます。
 sizeOfRegion は、ヒープ上で初期化されるように要求されたバイト数です
 。2 番目の引数 policy は、使用する割り当てポリシーを決定します。0の
 合はベストフィットに、1の場合はファーストフィットに、2の場合はワース
 トフィットに従うことになります。

この関数は、プログラムの最初に一度だけ、他の3つの関数を呼び出す前に呼
び出す必要があります。コードをテストする際には、最初にこの関数を呼び出
して十分な領域を初期化し、その後のMem_Alloc()による領域確保の呼び出
しがうまくいくようにします。私たちが提供するテストファイル（後述）も同
様です。

 プロセスがOSにヒープのメモリを要求するとき、OSはページ単位でメモリを
 割り当てます。ページとは、仮想メモリOSにおけるメモリ管理のための最小
 データ単位である。固定長の連続した仮想メモリのブロックである。ページに
 ついての詳細はこちらをご覧ください。Mem_Initは、要求されたメモリ量を
 このページサイズの単位で切り上げることに注意してください。切り上げの
 ため、初期化されるメモリの量は sizeOfRegion よりも多くなることがあり
 ます。この初期化された領域のすべてを、ユーザーへのメモリ割り当てに使用
 することができます。

Mem_Init が正常に呼び出されると、list_head はフリーリストの最初で唯
一のヘッダとして初期化され、1 つのフリーチャンクのメモリを指します。こ
のリストを使用して、Mem_Alloc()コールによってユーザーにスペースを割り
当てることになります。
Mem_Initはmmap()システムコールを使用してヒープ上の空間を初期化しま
す。興味のある方はmanページを読んでその仕組みを確認してみてください。
*/

{
	int pagesize;
	int padsize;
	int fd;
	int alloc_size;
	void* space_ptr;
	static int allocated_once = 0;
	
	if(0 != allocated_once)
	{
		fprintf(stderr,"Error:mem.c: Mem_Init has allocated space during a previous call\n");
		return -1;
	}
	if(sizeOfRegion <= 0)
	{
		fprintf(stderr,"Error:mem.c: Requested block size is not positive\n");
		return -1;
	}

	/* Get the pagesize */
	pagesize = getpagesize();

	/* Calculate padsize as the padding required to round up sizeOfRegion to a multiple of pagesize */
	padsize = sizeOfRegion % pagesize;
	padsize = (pagesize - padsize) % pagesize;

	alloc_size = sizeOfRegion + padsize;

	/* Using mmap to allocate memory */
	fd = open("/dev/zero", O_RDWR);
	if(-1 == fd)
	{
		fprintf(stderr,"Error:mem.c: Cannot open /dev/zero\n");
		return -1;
	}
	space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (MAP_FAILED == space_ptr)
	{
		fprintf(stderr,"Error:mem.c: mmap cannot allocate space\n");
		allocated_once = 0;
		return -1;
	}
	
	allocated_once = 1;
	
	/* To begin with, there is only one big, free block */
	list_head = (block_header*)space_ptr;
	list_head->next = NULL;
	/* Remember that the 'size' stored in block size 
	excludes the space for the header */
	list_head->size_status = alloc_size - (int)sizeof(block_header); // 自分自身の情報分はサイズから控除する
	fit = policy;
	return 0;
} // end of mem init


/* Function for allocating 'size' bytes. */
/* Returns address of allocated block on success */
/* Returns NULL on failure */
/* Here is what this function should accomplish */
/* - Check for sanity of size - Return NULL when appropriate */
/* - Round up size to a multiple of 4 */
/* - Traverse the list of blocks and allocate the best free block which can accommodate the requested size */
/* -- Also, when allocating a block - split it into two blocks when possible */
/* Tips: Be careful with pointer arithmetic */
void* Mem_Alloc(int size)
/* 仕様
void *Mem_Alloc(int size):
Mem_Alloc()は、ライブラリ関数のmalloc()に似ています。Mem_Allocは、
割り当てられるメモリ空間のサイズをバイト単位で入力パラメータとして受け取り、
そのメモリ空間の開始点へのポインタを返します。(つまり、ヘッダ後の最初の有用
なバイトの開始点へのポインタです）。) -> done

この関数は、Mem_Init() によって割り当てられた sizeOfRegion 内に、
この要求を満たすのに十分な連続した空き領域がない場合は、NULL を返す。 > done

パフォーマンスを向上させるために、Mem_Alloc()は4バイトアラインメント
のメモリチャンクを返すようにしている。例えば、ユーザが1バイトのメモリを
要求した場合、Mem_Alloc()の実装は4バイトのメモリを返すべきで、そうすれ
ば次の空きブロックも4バイト配列になります。4バイトアラインドのポインタを
返しているかどうかをデバッグするには、次のようにポインタを表示します。
	printf("%08x", ptr)
最後の桁は4の倍数（つまり、0、4、8、C）でなければなりません。
例えば、0xb7b2c04cはOKで、0xb7b2c043はNGです。 > done

適切な空きブロックが見つかったら、そのブロック全体を割り当てに使
うことができます。しかし、内部の断片化を引き起こし、スペースを無駄
にしてしまうというデメリットがあります。そこで、このブロックを2つ
に分割します。最初の部分が割り当てられたブロックになり、残りの部分が
新しいフリーブロックになります。ブロックを分割する前に、新しいフリー
ブロックに十分なスペースが残っていなければなりません。すなわち、ヘッ
ダとその最小ペイロードである4バイトが含まれていなければ、ブロックを分
割しません。

ブロックのサイズには、ヘッダのサイズは含まれません。これは、講義で見
たアロケータブロックがヘッダのサイズをブロックサイズに含むのとは異な
ることに注意してください。
*/
{

	/* Your code should go in here */
	block_header *bh; // 定義で block_headerときちんと名付けしてあるので、Structは不要
	block_header *index;

	index = list_head;
	printf("bh size and list_headsize is: %p, %p\n", list_head, list_head->next);
	printf("bh size and list_headsize is hoge: %d, %d\n", index->size_status, list_head->size_status);

	while(index->next != NULL) {
		printf("index is: %p, %p\n", index, index->next);
		index = index->next;
	}

	if (index == list_head) {
		printf("hoge");
		bh = list_head + 4 * size;
		list_head->next = bh;
		bh->size_status = 4 * size + 1; // for busy status
		bh->next = NULL;
	} else {
		printf("in index is: %p, %p, %d\n", index, index->next, index->size_status);
		printf("fuga");
		bh = index + index->size_status;
		index -> next = bh;
		bh->size_status = 4 * size + 1; // for busy status
		bh->next = NULL;
	}

	// bh = index + 4 *size;
	// index -> next = bh;
	// bh->size_status = 4 * size + 1; // for busy status
	// bh->next = NULL;

// 		bh = list_head + 4 * size;
	// list_head->next	= bh;
	// bh->size_status = 4 * size;
	// bh->next = NULL;

	printf("init size is: %d\n", list_head->size_status);
	printf("bh size and list_headsize is: %d, %d\n", bh->size_status, list_head->size_status);
	// printf("bh pointer is : %08x\n", bh);

	if(bh->size_status > list_head->size_status) {
		printf("bh size and list_headsize is hoge: %d, %d\n", bh->size_status, list_head->size_status);
		return NULL;
	}

	return bh;

}

/* Function for freeing up a previously allocated block */
/* Argument - ptr: Address of the block to be freed up */
/* Returns 0 on success */
/* Returns -1 on failure */
/* Here is what this function should accomplish */
/* - Return -1 if ptr is NULL */
/* - Return -1 if ptr is not pointing to the first byte of a busy block */
/* - Mark the block as free */
/* - Coalesce if one or both of the immediate neighbours are free */
int Mem_Free(void *ptr)

/* 仕様
int Mem_Free(void *ptr):
Mem_Free()は、ptrが指すメモリオブジェクトを解放します。標準的な
free()と同様に、ptrがNULLの場合は何の処理も行われません。この関数
は、成功すると0を返し、ptrがMem_Alloc()によって割り当てられてい
ない場合は-1を返します。ptrがNULLの場合も-1を返します。解放される
ブロックは、隣接するブロックのいずれかまたは両方が空いていれば、常
にそのブロックと合体します。
*/

{
	/* Your code should go in here */

}

/* Function to be used for debug */
/* Prints out a list of all the blocks along with the following information for each block */
/* No.      : Serial number of the block */
/* Status   : free/busy */
/* Begin    : Address of the first useful byte in the block */
/* End      : Address of the last byte in the block */
/* Size     : Size of the block (excluding the header) */
/* t_Size   : Size of the block (including the header) */
/* t_Begin  : Address of the first byte in the block (this is where the header starts) */
void Mem_Dump()
/*
これは、すべてのメモリブロック（空きブロックと割り当て済みブロックの両方）
のリストを表示するもので、デバッグに使用します。自分のコードが正しく動作
しているかどうかを確認する際に、非常に便利です。今後のプログラミングの
注意点としては、この関数に注目してください。複雑なプログラムの実装に取り
組んでいるとき、データ構造に関する多くの有用な情報を生成するこのような
関数は、それを実装するのに費やす時間を十分に考慮する価値があります。
*/

{
	int counter;
	block_header* current = NULL;
	char* t_Begin = NULL;
	char* Begin = NULL;
	int Size;
	int t_Size;
	char* End = NULL;
	int free_size;
	int busy_size;
	int total_size;
	char status[5];

	free_size = 0;
	busy_size = 0;
	total_size = 0;
	current = list_head;
	counter = 1;
	fprintf(stdout,"************************************Block list***********************************\n");
	fprintf(stdout,"No.\tStatus\tBegin\t\tEnd\t\tSize\tt_Size\tt_Begin\n");
	fprintf(stdout,"---------------------------------------------------------------------------------\n");
	while(NULL != current)
	{
		t_Begin = (char*)current;
		Begin = t_Begin + (int)sizeof(block_header);
		Size = current->size_status;
		strcpy(status,"Free");
		if(Size & 1) /*LSB = 1 => busy block*/
		{
			strcpy(status,"Busy");
			Size = Size - 1; /*Minus one for ignoring status in busy block*/
			t_Size = Size + (int)sizeof(block_header);
			busy_size = busy_size + t_Size;
		}
		else
		{
			t_Size = Size + (int)sizeof(block_header);
			free_size = free_size + t_Size;
		}
		End = Begin + Size;
		fprintf(stdout,"%d\t%s\t0x%08lx\t0x%08lx\t%d\t%d\t0x%08lx\n",counter,status,(unsigned long int)Begin,
					 (unsigned long int)End,Size,t_Size,(unsigned long int)t_Begin);
		total_size = total_size + t_Size;
		current = current->next;
		counter = counter + 1;
	}
	fprintf(stdout,"---------------------------------------------------------------------------------\n");
	fprintf(stdout,"*********************************************************************************\n");

	fprintf(stdout,"Total busy size = %d\n",busy_size);
	fprintf(stdout,"Total free size = %d\n",free_size);
	fprintf(stdout,"Total size = %d\n",busy_size+free_size);
	fprintf(stdout,"*********************************************************************************\n");
	fflush(stdout);
	return;
}




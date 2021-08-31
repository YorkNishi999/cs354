#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#define HDR_SZ 20 // byte


//https://docs.google.com/spreadsheets/d/1i2wCyYvGyrIV2F-k-KzwTfA2XjZZ1y0LmbgpjxTycdg/edit#gid=0




int is_little_endian() {
	/* TODO: return 1 if machine is little endian, 0 if not */ 

	int num = 0x12345678;
	int *p = &num;
	char *pc = (char *)p;
	int i;
	for (i = 0; i < 4; ++i)
		printf("mem[%d] = %x\n", i, pc[i]);
	if(pc[0] >= pc[4])
		return 1; // little endian
	else
		return 0;

}

unsigned int get_length(unsigned char *hdr) {
	/* TODO: extract Total length field*/
	unsigned int ans = 0;
	int i;

	// hdr[2] ~ hdr[3]を連続して取って10進法にする
	for (i = 2; i < 4; ++i) {
		ans = ans*16*16 + hdr[i];
	}

	return ans; // 0x44 = 68

}

unsigned int get_ip(unsigned char *hdr, int option) {
	/* TODO: extract Source IP if option is 0 and 
			 Destination IP if option is 1 */

	int i = 0;
	unsigned long int ans = 0;

	if (option == 0){
		for (i = 12; i < 16; ++i) {
			ans = ans*16*16 + hdr[i]; //2886992637
			printf("hdr[i] i ans: %d %d %ld\n",hdr[i], i, ans);
		}
	} else if (option == 1) {
		for (i = 16; i < 20; ++i) {
			ans = ans*16*16 + hdr[i]; //2886991878
			printf("hdr[i] i ans: %d %d %ld\n",hdr[i], i, ans);
		}
	}
	return ans;

}

char* charcat(char* p, char ch) {
  *p = ch;
  return ++p;
}

void reversechar(char s[])
{
	char* pstart;
	int len = 0;
	char tmp;
	int i;

	pstart = s; // pstart is the beginning of pointer of s

	while (*s++ != '\0')
		++len;
	--s;
	--s;

	while(pstart < s) {	
		tmp = *pstart;
		*pstart = *s;
		*s = tmp;
		++pstart;
		--s;
	}
 // '\0' remains at the last address.

}

void itob(int n, char s[], int b) // nをb進数のchar*に変換
{
	int i = 0;
	
	do {
		s[i++] = (n % b > 9) ? (n % b + 39 + '0') : (n % b + '0');
	} while ((n /= b) > 0);

	s[i] = '\0';

}

void printcharptr(unsigned char* ch)  // Char*を表示
{
	while (*ch) {
		printf("%c", *ch);
		ch++;
	}

	printf("\n");

}

char *format_ip(unsigned int ip_int) {
	/* TODO: return IP in the form A.B.C.D (dotted decimal notation) */
	int i = 0, counter = 0;
	int ans[4], index = 0; // ans: 10進数。ピリオドで挟まれた数値
	unsigned char *charans[4]; // 10進数をChar*にする。Char*（ポインタ）の配列
	unsigned char *res, *resstart; // res: 答え
	unsigned char c;

	for (i = 0; i < 4; i++) { // メモリ確保
		charans[i] = (char*)malloc(sizeof(char[4]));
	}

	for( i = 0; i < 4; ++i){ // 16進数を10進数に変換
		ans[i] = ip_int % (16*16);
		ip_int /= (16*16);
	}

	for ( i = 0; i < 4; i++) { // int -> char* に変換
		index = ans[i];
		itob(ans[i], charans[i], 10);
	}  // charans[]はポインタがはいってる。その先は、charの文字列

	res = (char*)malloc(sizeof(char[32])); // resメモリ確保
	resstart = res; // あとからresの最初に戻すので、開始アドレスを保存

	for (i = 0; i < 4; ++i) {
		while(*charans[i]) { 
			*res =  *charans[i];
			++res;
			++charans[i];
		}
		if (i < 3)
			res = charcat(res, '.');
	}
	res = charcat(res, '\0');
	
	res = resstart;
	reversechar(res);
	
	return res;

}


int is_checksum_valid(char *hdr) {
	/* TODO: if checksum valid, return 1; else return 0; */

	// 1. IPヘッダをすべて足し算する。intだと、大きすぎて入りきれないので
	// １６進数のまま計算する必要ある。

	unsigned short a[10], b[10];
	unsigned long sum[10];
	// sum = (unsigned long *)malloc(sizeof(long));
	int i, j;
	// a[0] = hdr[0];
	// b[0] = hdr[1];

	for (i = 0; i < 10; ++i) {
		a[i] = hdr[2*i];
		b[i] = hdr[2*i+1];
		printf("a[i] b[i] i: %x, %x, %d\n", a[i], b[i], i);
	}

	sum[0] = (a[0] & 0xFF) + (b[0] & 0xFF);
	// printf("header sum 経過 is: %lx\n", sum[0]);
	//sum[1] = (a[1] & 0xFF) + (b[1] & 0xFF) + (sum[0] >> 8);
	// printf("nakami: %x, %x, %lx\n", (a[1] & 0xFF), (b[1] & 0xFF), (sum[0] >> 8) ); 
	// printf("header sum 経過 is: %lx\n", sum[1]);
	// printf("%02x %02x\n",(unsigned char)sum[1],(unsigned char)(sum[0]));
	for (int i = 1; i < 10; i++) {
		sum[i] = (a[i] & 0xFF) + (b[i] & 0xFF) + (sum[i-1] >> 8);
		printf("header sum 経過 after is: %lx\n", sum[i]);
	}

	for (int i = 9; i > -1; --i) {
		printf("%02x ",(unsigned char)sum[i]);
	}
	printf('\n');

	// 2. １６進数になおして、一番左と残りを分離する

	// 3. 一番左をintにして、残りのIntに足す

	// 4. 全部を２進法にして、〜を取る　→　０になればOK。




}

int get_protocol(char *hdr) {
	/* TODO: return the 8 bit protocol number as an integer */
	unsigned long long int ans = 0;
	int i;

	// hdr[9]取って10進法にする
	for (i = 9; i < 10; ++i) {
		ans = ans*16*16 + hdr[i];
	}

	return ans; // 0x11 = 17
} 

int get_version(char *hdr) {
	/* TODO: returns the 4 bit version number as an integer */
	unsigned int ans = 0;
	int i;

	// hdr[9]取って10進法にする
	for (i = 0; i < 1; ++i) {
		ans = ans*16*16 + hdr[i];
	}

	return ans; // 0x45 = 69
} 


int get_flags(char *hdr, int pos) {
	/* TODO: returns the 1 bit (corresponding to the pos)
			 from the flags field */
	unsigned int ans = 0;
	int i;
	int anslist[3];

	// hdr[６]から、１桁だけ持ってくる
	for (i = 6; i < 7; ++i) {
		ans = hdr[i]/16; // 0
	}


	// 二進法に変換
	for (i = 0; i < 3; ++i) {
		anslist[i] = ans % 2;
		ans /= 2;
	}
	
	switch (pos) {
		case 0:
			return anslist[0];
		case 1:
			return anslist[1];
		case 2:
			return anslist[2];
	}

}


int main(int argc, char *argv[]) {
	unsigned char *hdr; //binary file の書き出し先
	hdr = (char *)malloc(sizeof(char)); // ポインタの初期化を忘れない！！
	unsigned int n = 0;
 
	/* TODO: Read the contents of the file into hdr */
	FILE *fp, *outp;

	fp = fopen("sample", "rb");
	n = fread(hdr, sizeof(char), sizeof(fp)+12, fp);

	outp = fopen("ansSample", "wb");
	fwrite(hdr, sizeof(char), sizeof(fp)+12, outp);
	fclose(outp);


	/* TODO: Store the number of bytes read into n */
	fclose(fp);

	printf("start hdr address: %p\n", hdr);

	/* DO NOT MODIFY BELOW THIS LINE */

	printf("read %u bytes\n", n);
	if (is_little_endian()) { 
		printf("Machine is little endian\n");
	} else {
		printf("Machine is big endian\n");
		return -1;
	}
	unsigned int len = get_length(hdr);


	printf("total length: %u bytes (0x%x)\n", len, len);	
	
	if (is_checksum_valid(hdr)) {
		printf("checksum is valid\n");
	
	} else {
		printf("checksum is not valid\n");
	}

	printf("protocol: 0x%x\n", get_protocol(hdr));

	printf("version: %u\n", get_version(hdr));
	
	printf("flag: 0x%x%x%x\n", get_flags(hdr, 0),
	get_flags(hdr, 1), get_flags(hdr, 2));

	// get source ip
	unsigned long int sip = get_ip(hdr, 0);
	printf("source ip: %lu (0x%lx)\n", sip, sip);   
	char *sip_formatted = format_ip(sip);
	printf("formatted: %s\n",sip_formatted);
	free(sip_formatted);

	// get dest ip
	unsigned int dip = get_ip(hdr, 1);
	printf("destination ip: %u (0x%x)\n", dip, dip); 
	char *dip_formatted = format_ip(dip);
	printf("formatted: %s\n",dip_formatted);
	free(dip_formatted);    

	free(hdr);
	return 0;

}

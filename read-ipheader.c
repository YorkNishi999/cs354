#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

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

	// hdr[8] ~ hdr[15]を連続して取って10進法にする
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
		printf("3 tmp pstart s %c %c %c\n", tmp, *pstart, *s);
		*pstart = *s;
		printf("4 tmp pstart s %c %c %c\n", tmp, *pstart, *s);
		*s = tmp;
		printf("5 tmp pstart s %c %c %c\n", tmp, *pstart, *s);
		++pstart;
		--s;
	}
 // '\0' remains at the last address.

}

char *format_ip(unsigned int ip_int) {
	/* TODO: return IP in the form A.B.C.D (dotted decimal notation) */
	int i = 0, counter = 0;
	int ans[4], index = 0;
	unsigned char *res, *resstart;
	unsigned char c;
	res = (char*)malloc(sizeof("255.255.255.255"));
	resstart = res;
	//printf("start res: %p\n", res);
	//printf("start resstart: %p\n", resstart);
	

	for( i = 0; i < 4; ++i){
		ans[i] = ip_int % (16*16);
		ip_int /= (16*16);
		printf("pre IP is %d\n", ans[i]);  
	}

	for ( i = 0; i < 4; i++) {
		index = ans[i];
		printf("index is %d\n", index);

		while( index%10 ) {
			c = '0' + index%10;
			res = charcat(res, c);
			printf("index amari is %d\n", index%10);
			printf("*res is %c\n", *(res-1)); // it works here.
			printf("address is %p\n", res);
		
			index /= 10;
			counter++;
		}
		res = charcat(res, '\0');
		res = resstart; 
		reversechar(res);

		res = charcat(res, '.');

		for (res = resstart; *res != '\0'; res++ )
			printf("the complete of insert of index[i] + . is: %c\n", *res);
		//res = resstart;
		//printf("after: %p\n", res);

	} // end of for

	while(*res != '\0'){
		printf("char of res %c\n", *res);
		res++;
	}
	
	return res;

}

int is_checksum_valid(char *hdr) {
	/* TODO: if checksum valid, return 1; else return 0; */
}

int get_protocol(char *hdr) {
	/* TODO: return the 8 bit protocol number as an integer */
} 

int get_version(char *hdr) {
	/* TODO: returns the 4 bit version number as an integer */
} 

char get_flags(char *hdr, int pos) {
	/* TODO: returns the 1 bit (corresponding to the pos)
			 from the flags field */
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

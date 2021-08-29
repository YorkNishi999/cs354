#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#define HDR_SZ 48

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

unsigned long long int get_length(unsigned char *hdr) {
	/* TODO: extract Total length field*/
	unsigned long long int ans = 0;
	int i;
	char *hdrans;
	hdrans = (char *)malloc(sizeof(char));

	// hdr[8] ~ hdr[15]を連続して取って10進法にする
	for (i = 8; i < 16; ++i) {
		ans = ans*16*16 + hdr[i];
		printf("hdr[i] hdr[i+1] i ans: %d %d %lld\n",hdr[i], i, ans);
	}

	return ans; // 4616596929870299901 になるべき

}

unsigned int get_ip(char *hdr, int option) {
	/* TODO: extract Source IP if option is 0 and 
			 Destination IP if option is 1 */
}

char *format_ip(unsigned int ip_int) {
	/* TODO: return IP in the form A.B.C.D (dotted decimal notation) */
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
	unsigned long long int len = get_length(hdr);


	printf("total length: %llu bytes (0x%llx)\n", len, len);	
	
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
	unsigned int sip = get_ip(hdr, 0);
	printf("source ip: %u (0x%x)\n", sip, sip);   
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

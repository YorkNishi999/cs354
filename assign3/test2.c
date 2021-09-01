#include <stdio.h>

int main(){
	unsigned short a[2],b[2],c[2];
	a[1]=0x0F;
	a[0]=0xFF;
	
	b[1]=0x0F;
	b[0]=0xFF;

	c[0]=a[0]+b[0];
	c[1]=a[1]+b[1]+(c[0]>>8);

	printf("%02x %02x\n",(unsigned char)c[1],(unsigned char)(c[0] & 0xFF));

	return 0;
}

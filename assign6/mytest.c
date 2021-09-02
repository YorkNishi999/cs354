#include <stdio.h>
#include <stdlib.h>

int main ()
{
	char a = 'a';
	char b = 'a';
	char *c;
	char *d;

	d = (char *)malloc(sizeof(char));

	*c = a; // a の中身をコピーして、確保されているCのメモリに入れてるから、CとAのアドレスは（当然）異なる
	d = &a;

	
	printf("address of d is: %p\n", d);
	printf("address of a is: %p\n", &a);
	printf("char of a is: %c\n", a);
	printf("address of c is: %p\n", c);
	printf("address of a+1 is: %p\n", &a+1);
	printf("address of b is: %p\n", &b);

	*c = NULL;
	free(c);

	printf("saki of c is: %d\n", *c);
	printf("address of c is: %p\n", c);
	
	return 0;

	
}
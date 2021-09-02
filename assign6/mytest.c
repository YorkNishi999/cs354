#include <stdio.h>

int main ()
{
	char a = 'a';
	char b = 'a';

	
	printf("address of a is: %p\n", &a);
	printf("address of a is: %d\n", a);
	printf("address of a is: %p\n", &a+1);
	printf("address of a is: %p\n", &b);

	return 0;

	
}
#include<stdio.h>
#include<stdlib.h>


int main()
{
	// char * ch = "ABC";
	// char c;
	//ch = (char *)malloc(sizeof("ABC"));

	char *ch = 'A';


	while(*ch){
		ch++;
	}
	
	*ch = 'D';
	ch++;
	*ch = '\0';

	//ch = ch - 2;

	// printf("%c\n", *(ch+3)); //*ch は先頭のアドレス

	return 0;

}
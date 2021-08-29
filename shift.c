#include <stdio.h>

int main() {
    unsigned int num1 = 0x12345678;
    printf("num1 = 0x%x\n", num1);
    printf("num1 = %d\n", num1);
    num1 = num1 << 16;
    printf("num1 = 0x%x\n", num1);
	printf("num1 = %d\n", num1);
    num1 = num1 >> 16;
    printf("num1 = 0x%x\n", num1);
    num1 = num1 + 0x3000;
    printf("num1 = 0x%x\n", num1);
    num1 = num1 << 16;
    printf("num1 = 0x%x\n", num1);
    num1 = num1 >> 16;
    printf("num1 = 0x%x\n", num1);

    printf("\n");

    int num2 = 0x12345678;
    printf("num2 = 0x%x\n", num2);
    num2 = num2 << 16;
    printf("num2 = 0x%x\n", num2);
    num2 = num2 >> 16;
    printf("num2 = 0x%x\n", num2);
    num2 = num2 + 0x3000;
    printf("num2 = 0x%x\n", num2);
    num2 = num2 << 16;
    printf("num2 = 0x%x\n", num2);
    num2 = num2 >> 16;
    printf("num2 = 0x%x\n", num2);
    return 0;
}
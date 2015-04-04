#include <stdio.h>

int main() {
	int a, b;
	printf("input the two numbers:\n ");
	scanf("%x%x", &a, &b);
	int c = (b & 0xffffff00) | (a & 0xff);
	printf("result is 0x%x\n", c);
	return 0;
}

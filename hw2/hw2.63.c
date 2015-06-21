#include <stdio.h>

unsigned srl(unsigned x, int k);
int sra(int x, int k);

int main(){
	int a = 0xffffffff;
	unsigned b = 0xffffffff;
	printf("result signed:%x\n", a >> 1);
	printf("result unsigned:%x\n", b >> 1);
	return 0;
}

unsigned srl(unsigned x, int k) {
	int xsra = (int)x >> k;
}

int sra(int x, int k) {

	unsigned xsrl = (unsigned)x >> k;
}

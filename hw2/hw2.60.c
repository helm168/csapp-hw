#include <stdio.h>

unsigned replace_byte(unsigned h, int i, unsigned char b);

int main() {
	unsigned h;
	int i;
	unsigned char b;
	printf("input replace parameters(use hexdecimal except i):\n");
	scanf("%x%d%x", &h, &i, (unsigned int*)&b);
	replace_byte(h, i, b);
}

unsigned replace_byte(unsigned h, int i, unsigned char b) {
	if(i < 4 && i>=0) {
		unsigned char* ha = (unsigned char*)&h;
		ha[i] = b;
		printf("number after replace:%x\n", h);
	}else {
		printf("wrong paramter i:%d\n", i);
	}
}

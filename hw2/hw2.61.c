#include <stdio.h>

int all_bits_equals_one(int x);
int all_bits_equals_zero(int x);
int lsb_equals_one(int x);
int msb_equals_zero(int x); 

int main() {
	printf("input the number you want to check(hex format):");
	int x;
	scanf("%x", &x);
	if(all_bits_equals_one(x)) {
		printf("all bits equals one.\n");
	}
	if(all_bits_equals_zero(x)) {
		printf("all bits equals zero.\n");
	}
	if(lsb_equals_one(x)) {
		printf("lsb equals one.\n");
	}
	if(msb_equals_zero(x)) {
		printf("msb equals zero.\n");
	}
}

int all_bits_equals_one(int x) {
	//the bits of -1 are all one
	int all_bits_one = -1; 
	return !(x ^ all_bits_one); 
}

int all_bits_equals_zero(int x) {
	int all_bits_zero = 0;
	return !(x ^ all_bits_zero);
}

int lsb_equals_one(int x) {
	int left_shift_val = (sizeof(int) - 1) << 3;
	return all_bits_equals_zero((x - 0xff) << left_shift_val); 
}

int msb_equals_zero(int x) {
	int right_shift_val = (sizeof(int) - 1) << 3;
	return all_bits_equals_zero(x >> right_shift_val);
}		

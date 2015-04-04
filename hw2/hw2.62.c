#include <stdio.h>

int int_shifts_are_arithmetic();

int main() {
	if(int_shifts_are_arithmetic()) {
		printf("Your machine are arithmetic shift\n");
	} else {
		printf("Your machine are logic shift\n");
	}
}


int int_shifts_are_arithmetic() {
	int i = -1;
	return !((i >> 1) ^ i);
}

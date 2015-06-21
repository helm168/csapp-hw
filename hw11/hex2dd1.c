#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc != 2) return;

	struct in_addr inaddr;
	unsigned int addr;
	sscanf(argv[1], "%x", &addr);
	inaddr.s_addr = htonl(addr);
	printf("%s\n", inet_ntoa(inaddr));

	return 0;
}

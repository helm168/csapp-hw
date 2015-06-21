#include <arpa/inet.h>
#include <stdio.h>

int main(int argc, char **argv) {
	if(argc != 2) return;
	
	struct in_addr inaddr;
	char *addrstr;
	addrstr = argv[1];

	inet_aton(addrstr, &inaddr);
	printf("0x%x\n", ntohl(inaddr.s_addr));

	return 0;
}

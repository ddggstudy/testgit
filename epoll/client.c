#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>

const char *ip = "192.168.5.102";

int main(int argc, char* argv[])
{
	if(argc == 2)
		ip = argv[1];
	int fd;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("fd = %d\n", fd);

	if(fd == -1)
	{
		printf("socket err\n");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(18888);
	addr.sin_addr.s_addr = inet_addr(ip);
	
	int ret;
	ret = connect(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	if(ret == -1)
	{
		printf("ret = %d, errno=%d\n", ret, errno); 
		return -1;
	} 
	
	sleep(5);

	char buff[1024] = "aaaaaaaaaaa";
	int n = send(fd, buff, sizeof(buff), 0);	
	printf("n=%d\n", n);

	close(fd);
}


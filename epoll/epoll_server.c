#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>

const char *ip = "192.168.137.70";
const short port = 18888;

int main()
{
	int ret;
	int efd;
	
	efd = epoll_create(1024);
	printf("efd = %d\n", efd);

	int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	printf("listenfd = %d\n", listenfd);
	if(listenfd==-1)
		printf("errno=%d\n", errno);
	
	struct epoll_event ev;
	ev.data.fd = listenfd;
	ev.events = (uint32_t)(EPOLLIN | EPOLLOUT | EPOLLET);
	if(epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &ev))
	{
		printf("epoll add err\n");
		return -1;
	}
	

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	ret = bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	printf("bind ret = %d\n", ret);

	listen(listenfd, 20);

	int cnt;
	struct epoll_event events[100];
	
	struct sockaddr_in clientaddr;
	int len = sizeof(struct sockaddr_in);
	while(1)
	{
		cnt = epoll_wait(efd, events, 100, 500);
		printf("cnt = %d\n", cnt);
		if(cnt == -1)
			printf("epoll_wait errno = %d\n", errno);

		else
		{
			int i;
			for(i=0; i<cnt; i++)
			{
				if(events[i].data.fd == listenfd)
				{
					printf("new client comming\n");
					int cfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len); 
					printf("cfd=%d, errno=%d\n", cfd, errno);
					ev.data.fd = cfd;
					ev.events = (uint32_t)(EPOLLIN | EPOLLOUT | EPOLLET);
					ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
					printf("xxxxxxxxxxxxxxx ret = %d, errno=%d\n", ret, errno);
				}
				else if(events[i].events & EPOLLIN)
				{
					if(events[i].data.fd < 0)
					{
						printf("fd err\n");
						continue;
					}
					char buff[1024] = { 0 };
					int n = recv(events[i].data.fd, buff, 1024, 0); 
					printf("n=%d, buff=%s\n", n, buff);
				}
			}
		}
	}
	
	while(1);
}


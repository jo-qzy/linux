/*************************************************************************
	> File Name: epoll.c
	> Author: jo-qzy
	> Mail: jo-qzy@foxmail.com
	> Created Time: Sat 10 Nov 2018 03:24:51 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/epoll.h>

void setnonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

int recv_data(int fd, void* buf)
{
	ssize_t ret = 0;
	int recv_len = 0;
	while (1)
	{
		ret = recv(fd, buf + recv_len, 2, 0);
		if (ret < 0)
		{
			if (errno == EAGAIN)
			{
				//没数据了，退出
				break;
			}
			else if (errno == EINTR)
			{
				//读取被打断了，继续读取
				continue;
			}
		}
		else if (ret == 0)
		{
			return 0;
		}
		else if (ret < 2)
		{
			recv_len += ret;
			break;
		}
		recv_len += ret;
	}
	return recv_len;
}

int main(int argc, char* argv[])
{
	int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_fd < 0)
	{
		perror("socket error");
		return -1;
	}
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int ret = 0;
	ssize_t recv_len = 0;
	char buf[1024] = {0};
	int epoll_fd = epoll_create(1);
	if (epoll_fd < 0)
	{
		perror("epoll_create failed");
		return -1;
	}
	struct sockaddr_in listen_addr;
	socklen_t listen_len = sizeof(listen_addr);
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(atoi(argv[2]));
	listen_addr.sin_addr.s_addr = inet_addr(argv[1]);
	ret = bind(listen_fd, (struct sockaddr*)&listen_addr, listen_len);
	if (ret < 0)
	{
		perror("bind error");
		return -1;
	}
	ret = listen(listen_fd, 5);
	if (ret < 0)
	{
		perror("listen failed");
		return -1;
	}
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = listen_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);
	while (1)
	{
		struct epoll_event evs[10];
		int ready_fd = epoll_wait(epoll_fd, evs, 10, 3000);
		if (ready_fd < 0)
		{
			perror("epoll_wait error");
			return -1;
		}
		else if (ready_fd == 0)
		{
			printf("have no data arrived\n");
			continue;
		}
		int i = 0;
		for (i = 0; i < ready_fd; i++)
		{
			if (evs[i].data.fd == listen_fd)
			{
				int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
				if (client_fd < 0)
				{
					perror("accept error");
					continue;
				}
				setnonblock(client_fd);
				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = client_fd;
				epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);	
			}
			else
			{
				memset(buf, 0x00, 1024);
				ret = recv_data(evs[i].data.fd, buf);
				if (ret <= 0)
				{
					if (errno == EAGAIN || errno == EINTR)
					{
						continue;
					}
					else
					{
						perror("client had closed");
						epoll_ctl(epoll_fd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
						close(evs[i].data.fd);
						continue;
					}
				}
				printf("client say -> %s\n", buf);
			}
		}
	}
	close(listen_fd);
	return 0;
}

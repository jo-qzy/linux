/*************************************************************************
  > File Name: select.c
  > Author: jo-qzy
  > Mail: jo-qzy@foxmail.com
  > Created Time: Tue 06 Nov 2018 06:16:22 PM CST
  > introduction: 使用select多路转接模型的TCP服务端，
  实现多个客户端的连接处理
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
	struct sockaddr_in client_addr;
	struct sockaddr_in listen_addr;
	socklen_t listen_len = sizeof(struct sockaddr_in);
	socklen_t client_len = sizeof(struct sockaddr_in);
	int ret = 0;
	int i = 0;
	fd_set read_fd;//读文件就绪位图
	int max_fd;//max_fd记录当前最大使用到的文件描述符
	int fd_hash[1024];//记录使用到的文件描述符
	char buf[1024];
	memset(fd_hash, 0x00, sizeof(int) * 1024);

	int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listen_fd < 0)
	{
		perror("socket error");
		return -1;
	}
	fd_hash[listen_fd] = 1;
	max_fd = listen_fd;

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
		perror("listen error");
		return -1;
	}
	//先将read_fd置0，防止无关数据造成影响
	FD_ZERO(&read_fd);
	FD_SET(listen_fd, &read_fd);
	while (1)
	{
		max_fd = listen_fd;
		for (i = 0; i < 1024; i++)
		{
			if (fd_hash[i] == 1)
			{
				FD_SET(i, &read_fd);
				max_fd = max_fd > i ? max_fd : i;
			}
		}
		struct timeval tv;
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		ret = select(max_fd + 1, &read_fd, NULL, NULL, &tv);
		if (ret < 0)
		{
			perror("select error");
			return -1;
		}
		else if (ret == 0)
		{
			printf("have no request arrived\n");
			//此处可以做一些超时相关的事情
			continue;
		}
		//这里开始表示有描述符读就绪
		//循环查找描述符，并处理
		for (i = 0; i < max_fd + 1; i++)
		{
			if (FD_ISSET(i, &read_fd))
			{
				if (i == listen_fd)
				{
					//监听描述符，accept接收
					int client_fd;
					client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
					if (client_fd < 0)
					{
						perror("accept error");
						continue;
					}
					fd_hash[client_fd] = 1;
				}
				else
				{
					//如果不是监听描述符，说明是消息传入
					memset(buf, 0x00, 1024);
					recv(i, buf, 1024, 0);
					if (ret <= 0)
					{
						if (errno == EAGAIN || errno == EINTR)
						{
							continue;
						}
						else
						{
							perror("client shutdown");
							close(i);
							fd_hash[i] = 0;
							continue;
						}
					}
					printf("client say -> %s\n", buf);
				}
			}
		}
	}
	return 0;
}

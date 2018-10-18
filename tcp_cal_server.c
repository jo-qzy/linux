/*************************************************************************
	> File Name: cal.c
	> Author: jo-qzy
	> Mail: jo-qzy@foxmail.com
	> Created Time: Thu 18 Oct 2018 06:45:10 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct __cal_req_t
{
	int x;
	int y;
	unsigned char op;
}cal_req_t;

int main(int argc, char* argv[])
{
	int server_fd;
	server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd < 0)
	{
		perror("socket failed");
		return -1;
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[2]));
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	socklen_t server_len = sizeof(server_addr);
	int ret = bind(server_fd, (struct sockaddr*)&server_addr, server_len);
	if (ret < 0)
	{
		perror("bind error");
		return -1;
	}
	ret = listen(server_fd, 5);
	if (ret < 0)
	{
		perror("listen error");
		return -1;
	}
	while (1)
	{
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
		if (client_fd < 0)
		{
			perror("accept failed");
			continue;
		}
		char buf[1024] = {0};
		ssize_t recv_len = recv(client_fd, buf, 1024, 0);
		if (recv_len < 0)
		{
			perror("recv error");
			close(client_fd);
			continue;
		}
		else if (recv == 0)
		{
			printf("peer shutdown\n");
			close(client_fd);
			continue;
		}
		cal_req_t *seq = (cal_req_t*)buf;
		int sum = 0;
		switch (seq->op)
		{
		case '+':
			sum = seq->x + seq->y;
			break;
		case '-':
			sum = seq->x - seq->y;
			break;
		case '*':
			sum = seq->x * seq->y;
			break;
		case '/':
			sum = seq->x / seq->y;
			break;
		}
		send(client_fd, (void*)&sum, 4, 0);
		close(client_fd);
	}
	return 0;
}

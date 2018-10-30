/*************************************************************************
	> File Name: fifo_server.c
	> Author: jo-qzy
	> Mail: jo-qzy@foxmail.com
	> Created Time: Tue 30 Oct 2018 07:50:03 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	umask(0);
	if (mkfifo("my_pipe", 0664) < 0)
	{
		perror("mkfifo error");
		exit(EXIT_FAILURE);
	}
	int fd = open("my_pipe", O_RDONLY);
	if (fd < 0)
	{
		perror("open pipe error");
		return -1;
	}
	char buf[1024];
	while (1)
	{
		memset(buf, 0x00, 1024);
		printf("waiting for client write\n");
		ssize_t read_size = read(fd, buf, 1024);
		if (read_size > 0)
			printf("client say->%s\n", buf);
		else if (read_size == 0)
		{
			perror("client end this conversation, ready to exit");
			break;
		}
		else if (read_size < 0)
		{
			perror("read error");
			close(fd);
			return -1;
		}
	}
	close(fd);
	return 0;
}

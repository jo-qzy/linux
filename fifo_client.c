/*************************************************************************
	> File Name: fifo_client.c
	> Author: jo-qzy
	> Mail: jo-qzy@foxmail.com
	> Created Time: Tue 30 Oct 2018 08:35:32 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	int fd = open("my_pipe", O_WRONLY);
	if (fd < 0)
	{
		perror("open fifo failed");
		exit(EXIT_FAILURE);
	}
	char buf[1024];
	while (1)
	{
		memset(buf, 0x00, 1024);
		printf("Enter msg >>");
		fflush(stdout);
		ssize_t read_from_stdout = read(0, buf, 1024);
		if (read_from_stdout > 0)
			write(fd, buf, strlen(buf));
		else
			break;
	}
	close(fd);
	return 0;
}

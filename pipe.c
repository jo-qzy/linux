#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
	int fd[2];
	//int pipe(int pipefd[2]);
	//	pipefd:用于接收匿名管道创建成功后返回的两个描述符
	//	pipefd[0]:用于从管道读取数据
	//	pipefd[1]:用于向管道写入数据
	//	返回值：
	//		成功：0     失败：-1
	if (pipe(fd))
	{
		perror("Open pipe failed");
		return -1;
	}
	int pid = fork();
	if (pid < 0)
	{
		perror("Fork failed");
		return -1;
	}
	else if (pid == 0)
	{
		//匿名管道，子进程只读
		//关闭写端
		char buff[1024] = {0};
		close(fd[1]);
		sleep(3);
		read(fd[0], buff, 1024);
		printf("child -> %s\n", buff);
		close(fd[0]);
	}
	else
	{
		//父进程只写，关闭读端
		close(fd[0]);
		write(fd[1], "Hey ", 4);
		write(fd[1], "guys!\0", 5);
		close(fd[1]);
	}
	return 0;
}
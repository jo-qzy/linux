/*************************************************************************
	> File Name: test.c
	> Author: jo-qzy
	> Mail: jo-qzy@foxmail.com
	> Created Time: Sat 17 Nov 2018 10:18:09 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

void my_daemon()
{
	int pid = fork();
	if (pid < 0)
	{
		exit(-1);
	}
	else if (pid > 0)
	{
		exit(0);
	}
	//创建新会话
	setsid();

	//忽略SIGCHLD信号
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGCHLD, &sa, NULL);

	//将子进程关闭，创建孙子进程以防止其和其他终端建立联系
	pid = fork();
	if (pid < 0)
	{
		exit(-1);
	}
	else if (pid > 0)
	{
		exit(0);
	}
	//修改工作目录
	chdir("/");
	//关闭标准输入，重定向标准输出、标准错误输出
	close(0);
	int fd = open("/dev/null", O_RDWR);
	dup2(fd, 1);
	dup2(fd, 2);
}

int main()
{
	my_daemon();
	while (1)
	{
		sleep(1);
	}
	return 0;
}

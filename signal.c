/*************************************************************************
	> File Name: signal.c
	> Author: jo-qzy
	> Mail: jo-qzy@foxmail.com
	> Created Time: Wed 14 Nov 2018 05:11:23 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void my_SIGINT(int signo)
{
	printf("receive signo -> %d\n", signo);
	sleep(10);
}

int main()
{
	//1. 忽略处理
	//signal(SIGINT, SIG_IGN);
	//2. 自定函数处理
	//signal(SIGINT, my_SIGINT);
	//3. sigaction自定处理
	//n_act表示新的处理方式，o_act旧的处理方式
	struct sigaction n_act, o_act;
	//不希望其他信号受影响
	sigemptyset(&n_act.sa_mask);
	sigaddset(&n_act.sa_mask, SIGQUIT);
	n_act.sa_handler = my_SIGINT;
	n_act.sa_flags = 0;
	sigaction(SIGINT, &n_act, &o_act);
	while (1)
	{
		printf("hello~~~~~~~~\n");
		sleep(1);
	}
	return 0;
}

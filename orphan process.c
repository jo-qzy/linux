#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int val = 100;
    pid_t pid = fork();
    if (pid < 0)
    {
        printf("Create process failed!!\n");
        return -1;
    }
    else if (pid > 0)
    {
		//父进程先退出
        printf("This is parent process, it's pid is %d, prepare to exit in 15 seconds!\n", getpid());
        sleep(15);
        printf("Parent process exit now!\n");
        exit(0);
    }
	//子进程死循环，制造父进程先退出，子进程变为孤儿进程的情况
    printf("This id child process, it's pid is %d\n", getpid());
    while (1)
    {
        sleep(1);
    }
    return 0;
}

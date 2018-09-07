#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int pid = fork();
    if (pid < 0)
    {
        perror("Fork child process failed");
        return -1;
    }
    else if (pid == 0)
    {
		//子进程睡眠五秒退出
        printf("The subprocess will turn into a zobie in 5 seconds!\n");
        sleep(5);
        printf("The subprocess begin to turn to Z~~~\n");
        exit(0);
    }
    else
    {
		//父进程睡眠十秒
        printf("The parent process starts sleeping for 10 seconds!\n");
        sleep(10);
    }
	//在前五秒，父子进程均在睡眠，无异常
	//第五秒时，子进程退出，父进程仍睡眠未关心子进程的退出
	//5-10秒子进程处于僵尸进程状态
	//第十秒结束父进程醒来，调用了wait回收了子进程，结束

    int status;
    if (wait(&status))
    {
        printf("Subprocess exit success!\n");
    }
    return 0;
}

#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid;
    int i = 0;
    pid = fork();
    while (1)
    {
        if (pid > 0)
        {
            pid = fork();//创建新进程
            if (pid < 0)
            {
                printf("create fail!\n");
            }
            else
            {
				//创建成功就i++
                printf("new process %d!\n", i++);
            }
        }
    }
    return 0;
}

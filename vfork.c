#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main()
{
    int val = 100;
    int pid = vfork();
	//void vfork();
	//fork如果是个孝顺的孩子（子进程），当数据出现不同时，便单独开辟空间来管理数据
	//那么vfork相对于fork而言就是个逆子，vfork创建的子进程数据直接在父进程修改
	//就好比同样有个房子，fork另外买一个一模一样的来修改，而vfork则是在原来的房子里面乱搞，肆意破坏。 
	//当vfork出来的子进程执行时，父进程不运行，直到子进程执行结束才继续执行。 
    printf("pid -> %d\n", getpid());
    if (pid < 0)
    {
        printf("Vfork failed\n");
        exit(0);
    }
    else if (pid == 0)
    {
        val = 200;
        printf("Child process pid -> %d val -> %d\n", getpid(), val);
    }
    else
    {
        printf("Parent process pid -> %d val -> %d\n", getpid(), val);
    }
    return 0;
}

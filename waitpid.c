#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
    int pid = fork();
    if (pid < 0)
    {
        perror("Fork child process failed!\n");
        exit(-1);
    }
    else if (pid == 0)
    {
        sleep(5);
        exit(66);
    }

    int status;
	//pid_t wait(int* status);
	//wait：等待任意一个子进程退出，若没有紫禁城推出，则一直阻塞等待
	//阻塞：为了完成一个功能发起一个函数调用，如果没有完成这功能则一直挂起等待功能完成才返回
	//pid_t waitpid(pid_t pid, int* status, int options);
	//waitpid：可以等待指定子进程,也可以等待任意一个子进程退出，可以设置为非阻塞等待
	//非阻塞：为了完成一个功能发起一个函数调用，如果现在不具备完成的条件，则立即返回不等待
	//WNOHANG：如果没有子进程退出，则立刻报错返回，如果有则回收资源
	
	//进程退出状态码获取：在wait的参数中存储了子进程的退出原因以及退出码，而参数中只用了低16位两个字节用于存储这些信息
	//参数也就是int* status，在调用完成后status里存了退出码，我们可以用函数提供的宏来获取
	//在这低16位的信息中，高8位存储的是退出码，程序运行完毕退出才会有，低7位存储的是异常状况，第8位是core dump标志
	
	//举例几个宏
	//WIFEXTED(status) 这个宏用来获取是否正常退出，正常退出获得true
	//WEXITSTATUS(status) 该宏只可在WIFEXITED为true时使用，获取正常退出的状态码
	
	//WIFSIGNALED(status) 这个宏用来获取是否异常退出，异常退出获得true
	//WTERMSIG(status) 该宏只可在WIFSIGNALED为true时使用，获取异常退出的状态码
	
    while(waitpid(pid, &status, WNOHANG) == 0)
    {
        printf("No child process exit!\n");
        sleep(1);
    }
    printf("%d\n", status);

    if (WIFEXITED(status) == 1)
        printf("Exit num -> %d\n", WEXITSTATUS(status));

    return 0;
}

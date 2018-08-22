#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int argc = 0;
char* argv[32] = {NULL};

int param_parse(char* buff)
{
    if (buff == NULL)
        return -1;
    argc = 0;
    char* ptr = buff;
    char* tmp = NULL;
	//不断分割命令成指针数组
    while (*ptr != '\0')
    {
        while (*ptr == ' ')
            ptr++;
        if (*ptr == '\0')
            break;
        tmp = ptr;
        while (*tmp != ' ')
            tmp++;
        *tmp = '\0';
        argv[argc++] = ptr;
        ptr = ++tmp;
    }
    argv[argc++] = NULL;
    return 0;
}

int exec_cmd()
{
    int pid = 0;
    pid = fork();
    if (pid < 0)
        return -1;
    else if (pid == 0)
    {
		//替换进程
        execvp(argv[0], argv);
        exit(0);
    }
	//wait用法，等待子进程退出，退出后一个状态码会存入statu
	//用WIFEXITED这个宏来获取退出值是否是正常退出
    int statu;
    wait(&statu);
    if (WIFEXITED(statu) == 1)
        printf("%s\n", strerror(WEXITSTATUS(statu)));
    return 0;
}

int main()
{
    while (1)
    {
        char buff[1024] = {0};
        printf("[qzy@personal shell]$ ");
		//读取到\n为止，删除缓冲区数据
        scanf("%[^\n]%*c", buff);
        if (param_parse(buff) == -1)
            printf("wrong commond\n");
        exec_cmd();
    }
    return 0;
}

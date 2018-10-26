#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int argc = 0;
char* argv[32] = {NULL};

int div_commond(char* buf)
{
	if (buf == NULL)
		return -1;
	argc = 0;
	char* front = buf;
	char* back = buf;
	//不断分割命令成指针数组
	while (1)
	{
		while (*front == ' ')
			front++;
		if (*front == '\0')
			break;
		back = front;
		while (*back != ' ' && *back != '\0')
			back++;
		if (*back == '\0')
		{
			argv[argc++] = front;
			break;
		}
		*back = '\0';
		argv[argc++] = front;
		front = back + 1;
	}
	argv[argc++] = NULL;
	if (argc == 1)
		return -1;
	return 0;
}

int exec_cmd()
{
	int pid = fork();
	if (pid < 0)
	{
		perror("fork failed");
		return -1;
	}
	else if (pid == 0)
	{
		//替换进程
		execvp(argv[0], argv);
		exit(0);
	}
	else
	{
		//wait用法，等待子进程退出，退出后一个状态码会存入statu
		//用WIFEXITED这个宏来获取退出值是否是正常退出
		int status;
		wait(&status);
		if (WIFEXITED(status) == 1)
			printf("%s\n", strerror(WEXITSTATUS(status)));
	}
	return 0;
}
	
int main()
{
	char buf[1024];
	while (1)
	{
		memset(buf, 0x00, 1024);
		printf("------shell>>>>>>");
		//读取到\n为止，删除缓冲区数据
		scanf("%[^\n]%*c", buf);
		if (div_commond(buf) != 0)
		{
			char black_hall[100];
			fgets(black_hall, 100, stdin);
			printf("wrong commond\n");
			continue;
		}
		exec_cmd();
	}
	return 0;
}
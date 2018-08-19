#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t pid = fork();
	if (pid < 0)
	{
		printf("Create child process error!\n");
	}
	else if (pid > 0)
	{
		printf("This is parent process, pid->%d, begin to sleep!\n", getpid());
		sleep(30);
	}
	else
	{
		printf("Child process's pid->%d, begin Z...\n", getpid());
		sleep(5);
		exit(EXIT_SUCCESS);
	}
	return 0;
}
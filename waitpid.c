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

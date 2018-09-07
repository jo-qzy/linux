#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

int main()
{
	//分别用printf、write、sprintf、fprintf打印字符串到标准输出
    printf("printf ->%s%s", " Hey guys!", "\n");
    
    char w_buff[1024] = "write -> Hey guys!\n";
    int w_ret = write(1, w_buff, strlen(w_buff));
    if (w_ret < 0)
    {
        perror("write error");
        return -1;
    }

    char s_buff[1024] = {0};
    sprintf(s_buff, "sprintf -> %s\n", "Hey guys!");
    printf("%s", s_buff);
    
    fprintf(stdout, "fprintf -> %s\n", "Hey guys!");

    return 0;
}

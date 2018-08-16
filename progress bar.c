#include <stdio.h>
#include <unistd.h>

int main()
{
    char buff[109] = {0};
    int i = 0;
    buff[0] = '[';
    buff[102] = ']';
    for (i = 0; i <101; i++)
    {
        buff[i / 5] = '=';
        printf("[%-20s] [%d%%]\r",buff,i);
        fflush(stdout);
        usleep(10000);
    }
    printf("\n");
    return 0;
}

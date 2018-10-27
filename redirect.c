#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main()
{
    int fd = 0;
	//关闭文件描述符为1的文件，新打开的文件将占据1号描述符
	//1号描述符原来是属于stdout的，所以标准输出被分配给新打开的文件了
    close(1);
    fd = open("./redirect.txt", O_RDWR | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("Open file failed\n");
        return -1;
    }
	//printf本来应该输出到屏幕，看看结果如何
    printf("This is a file ostream redirect test\nFile fd -> %d\n", fd);
    fflush(stdout);
    close(fd);
    return 0;
}

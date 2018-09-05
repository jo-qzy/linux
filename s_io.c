#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	umask(0);
	int fd;
	//int open(const char *pathname, int flags, mode_t mode);
	//	pathname:要打开的文件的路径
	//	flags：标志选项
	//		必选项：(这个三个选项互相冲突，只能选择其中一个)
	//  		O_RDONLY    只读
	//  		O_WRONLY    只写
	//  		O_RDWR      读写
	//		非必选项：
	//  		O_CREAT     若文件存在在打开，不存在则创建
	//  		O_EXCL      与O_CREAT配合使用，若文件存在则报错
	//  		O_TRUNC     若文件存在则将文件长度截断为0
	//  		O_APPEND    追加(针对写入)
	//  mode:若文件不存在需要创建的时候，用于指定创建的文件权限
	//  返回值：
	//      成功 ：非负整数（文件描述符），后续操作都通过描述符完成
	//      失败 ：-1
	fd = open("./s_io.txt", O_RDWR | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		perror("file open error");
		return -1;
	}

	char w_buff[1024] = "Hi jo, long time no see!";
	//ssize_t write(int fd, const void *buf, size_t count);
	//  从buf中向fd所代表的文件写入count个字节的数据
	//  返回实际的写入长度，为long int型，出错返回-1
	ssize_t w_len = write(fd, w_buff, strlen(w_buff));
	if (w_len < 0)
	{
		perror("file write error");
		return -1;
	}
	printf("write length:%ld\n", w_len);

	lseek(fd, 0, SEEK_SET);
	//读写位置跳转，从头开始偏移0个字节

	char r_buff[1024] = { 0 };
	ssize_t r_len = read(fd, r_buff, 1024);
	//ssize_t read(int fd, void *buf, size_t count);
	//  从fd所代表的文件中读取count字节长度的数据放到r_buff中
	//  返回实际读取的数据长度，出错返回-1
	if (r_len < 0)
	{
		perror("file read error");
		return -1;
	}
	printf("read length:%ld\nread string->%s\n", r_len, r_buff);

	close(fd);
	return 0;
}
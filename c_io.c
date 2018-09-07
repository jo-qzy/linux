#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
	FILE *fp = NULL;
	size_t ret;
	fp = fopen("./c_io.txt", "w+");
	//FILE *fopen(const char *path, const char *mode);
	//  path 路径
	//  mode 打开方式
	//  r   只读打开已经存在的文件
	//  r+  读写打开已经存在的文件
	//  w   只写打开文件，文件不存在则创建，存在则长度截断为0
	//  w+  读写打开文件，文件不存在则创建，存在则长度截断为0
	//      若文件不存在，则创建文件，创建的文件权限默认为664
	//  a   追加模式(只写)打开文件，从文件末尾开始写入数据
	//  a+  读写(追加),文件读写位置刚打开的时候在起始位置，当写入的
	//      时候文件读写位置移动到文件末尾
	//      若文件不存在，则创建文件，创建的文件权限默认为664
	if (fp == NULL) {
		perror("fopen error");
		return -1;
	}
	
	size_t = fwrite("hello there!\n", 11, 1, fp);
	//size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
	//ptr    读取数据存入的目标地址
	//size   一次需要读取的字节
	//nmemb  读取的次数
	//stream 文件指针
	if (ret == 0) {
		perror("fwrite error");
	}
	
	if (feof(fp) == 1) {
		printf("file pointer in the end of file!\n");
	}
	
	
	char buff[1024] = {0};

	fseek(fp, 0, SEEK_SET);
	//fseek 
	//  SEEK_SET    从文件起始位置开始偏移
	//  SEEK_CUR    从当前读写位置开始偏移
	//  SEEK_END    从文件末尾位置开始偏移
	ret = fread(buff, 1, 11, fp);
	//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
	//ptr    读取数据存入的目标地址
	//size   一次需要读取的字节
	//nmemb  读取的次数
	//stream 文件指针
	
	//百思不得其解，第一次用fread居然啥也读不出来
	//加上下面的fprintf运行之后就能都出来了，屏蔽之后功能正常
	//我也不知道啥情况，而且不能重现，是我抽了还是电脑抽了
	if (ret == 0) {
		perror("read error");
	}
	printf("buff->%s\n", buff);

	//fseek(fp, 0, SEEK_END);
	//fprintf(fp, "%s-%d\n", "bit", 666);
	fclose(fp);
	return 0;
}
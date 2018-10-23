//实现网页输入IP地址和端口，回复一个http回复
//使用前先关闭防火墙
//centOS7关闭防火墙命令：systemctl stop firewalld.service
//centOS6关闭防火墙命令：servcie iptables stop
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	//使用说明
    if (argc != 3)
    {
        printf("Usage:./http_server IP_addr port");
        return -1;
    }
	//1.创建套接字
    int server_fd, client_fd, ret;
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0)
    {
        perror("socket error");
        return -1;
    }
    //防止意外断开，无法重新创建服务端的问题
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	//2.绑定地址
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    ret = bind(server_fd, (struct sockaddr*)&server_addr, server_len);
    if (ret < 0)
    {
        perror("bind error");
        return -1;
    }
	//3.开始监听
    ret = listen(server_fd, 5);
    if (ret < 0)
    {
        perror("listen error");
        return -1;
    }
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
		//4.accept等待连接
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0)
        {
            perror("accept error");
            continue;
        }
        char buf[2048] = {0};
		//5.接收http请求
        ret = recv(client_fd, buf, 2047, 0);
        if (ret > 0)
        {
            printf("request[\n%s]\n", buf);
        }
        char* rsp = "<h1>hello world<h1>";
        memset(buf, 0, 2048);
		//http请求/回复格式
		//首行	请求方法/协议版本 url/状态码 协议版本/状态码描述\r\n
		//协议头	一个一个的键值对(key: val)\r\n
		//空行	\r\n
		//正文	……
        sprintf(buf, "%s\r\n%s%d\r\n%s\r\n\r\n%s",
                "HTTP/1.1 200 OK",
                "Content-Length: ", strlen(rsp),
                "Content-Type: text/html; charset=UTF-8",
                rsp);
		//6.回应请求
        write(client_fd, buf, strlen(buf));
        //7.断开连接
		close(client_fd);
    }

    return 0;
}

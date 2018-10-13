#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char* argv[])
{
    int server_fd, client_fd, ret;
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0)
    {
        perror("socket error");
        return -1;
    }
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
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0)
        {
            perror("accept error");
            continue;
        }
        char buf[2048] = {0};
        ret = recv(client_fd, buf, 2047, 0);
        if (ret > 0)
        {
            printf("request[\n%s]\n", buf);
        }
        char* rsp = "<h1>hello world<h1>";
        memset(buf, 0, 2048);
        sprintf(buf, "%s\r\n%s%d\r\n%s\r\n\r\n%s",
                "HTTP/1.1 200 OK",
                "Content-Length: ", strlen(rsp),
                "Content-Type: text/html; charset=UTF-8",
                rsp);
        write(client_fd, buf, strlen(buf));
        close(client_fd);
    }

    return 0;
}

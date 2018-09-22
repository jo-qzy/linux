#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        perror("socket failed");
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t server_addr_len = sizeof(server_addr);
    int ret = bind(sockfd, (struct sockaddr*)&server_addr, server_addr_len);
    if (ret < 0)
    {
        perror("bind failed");
        return -1;
    }
    ret = listen(sockfd, 5);
    if (ret < 0)
    {
        perror("listen failed");
        return -1;
    }
    char buf[1024] = {0};
    while (1)
    {
        int new_sockfd;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        new_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (new_sockfd < 0)
        {
            perror("accept failed");
            continue;
        }
        while (1)
        {
            memset(buf, 0, 1024);
            ssize_t recv_len = recv(new_sockfd, buf, 1023, 0);
            if (recv_len < 0)
            {
                perror("recv error");
                continue;
            }
            else if (recv_len == 0)
            {
                printf("client link to termitate\n");
                close(new_sockfd);
                break;
            }
            else
            {
                printf("client[ip:%s, prot:%d]\n%s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
                memset(buf, 0, 1024);
                scanf("%s", buf);
                send(new_sockfd, buf, strlen(buf), 0);
            }
        }
        close(new_sockfd);
    }
    close(sockfd);
    return 0;
}

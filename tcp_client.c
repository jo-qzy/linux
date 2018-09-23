#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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
    int ret = connect(sockfd, (struct sockaddr*)&server_addr, server_addr_len);
    if (ret < 0)
    {
        perror("connect failed");
        return -1;
    }
    char buf[1024] = {0};
    while (1)
    {
        memset(buf, 0, 1024);
        scanf("%s", buf);
        send(sockfd, buf, strlen(buf), 0);
        memset(buf, 0, 1024);
        ssize_t recv_len = recv(sockfd, buf, 1023, 0);
        if (recv < 0)
        {
            perror("recv failed");
            continue;
        }
        else if (recv == 0)
        {
            perror("client link to teminate");
            break;
        }
        else
        {
            printf("server[message]\n%s\n", buf);
        }
    }
    return 0;
}

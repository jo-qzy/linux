#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>

void* thr_create_chat(void* id)
{
    int client_fd = (int)id;
    char buf[1024] = {0};
    while (1)
    {
        ssize_t recv_len = recv(client_fd, buf, 1024, 0);
        if (recv_len < 0)
        {
            perror("receive error");
            close(client_fd);
            break;
        }
        else if (recv_len == 0)
        {
            printf("peer shutdown\n");
            break;
        }
        else
        {
            printf("[client ip:%d]\n%s\n", client_fd, buf);
            send(client_fd, "server is busy", 14, 0);
        }
    }
    pthread_detach(pthread_self());
    return NULL;
}

void create_chat(int client_fd)
{
    pthread_t pid = -1;
    int ret = pthread_create(&pid, NULL, thr_create_chat, (void*)client_fd);
    if (ret < 0)
    {
        perror("create child thread error");
        return;
    }
}

int main(int argc, char* argv[])
{
    int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0)
    {
        perror("socket create error");
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    socklen_t server_len = sizeof(server_addr);
    int ret = bind(server_fd, (struct sockaddr*)&server_addr, server_len);
    if (ret < 0)
    {
        perror("bind failed");
        return -1;
    }
    ret = listen(server_fd, 5);   
    if (ret < 0)
    {
        perror("bind failed");
        return -1;
    }
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0)
        {
            perror("accept failed");
            continue;
        }
        create_chat(client_fd);
    }
    return 0;
}

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

void* listen_music(void* t)
{
	//此函数内使用线程退出，主线程使用线程等待
    int time = (int)t;
    int i = 0;
    for (; i < 100; i++)
    {
        printf("Listen to music~~\n");
        sleep(1);
        if (i == time)
        {
	        pthread_exit(0);
        }
    }
    return NULL;
}

void* do_homework(void* t)
{
	//写作业线程直接使用线程分离，不再需要等待
    int time = (int)t;
    while (time--)
    {
        printf("Do homework~~\n");
        sleep(1);
    }
    pthread_detach(pthread_self());
    return NULL;
}

int main()
{
    pthread_t music, homework;
    int ret;
    int time = 10;
    ret = pthread_create(&music, NULL, listen_music, (void*)time);
    if (ret < 0)
    {
        perror("pthread_create failed");
        return -1;
    }
    ret = pthread_create(&homework, NULL, do_homework, (void*)time);
    if (ret < 0)
    {
        perror("pthread_create failed");
        return -1;
    }
    //线程等待
    pthread_join(music);
    return 0;
}
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>

//创建一个线程和主线程同时做事
//子线程听歌，主线程做作业

void* listen_music(void* t)
{
    int time = (int)t;
    while (time--)
    {
        printf("Listen to music~~\n");
        sleep(1);
    }
    pthread_detach(pthread_self());
	//int pthread_detach(pthread_t tid);
	//	设置线程属性为detach
	//	用于线程分离，无需主线程回收资源
	//	当子线程运行结束自动回收资源
	//	也可以在主线程中调用来分离
    return NULL;
}

int main()
{
    pthread_t tid;
    int ret;
    int time = 10;
    ret = pthread_create(&tid, NULL, listen_music, (void*)time);
	//int pthread_create(pthread_t *thread, pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
    //  功能：线程的创建（创建用户线程）
    //  thread: 用于接收一个用户线程ID
    //  attr：  用于设置线程属性，一般置NULL
    //  start_routine：线程的入口函数
    //      线程运行的就是这个函数，这个函数退出了，线程也就退出了
    //  arg：   用于给线程入口函数传递参数
    //  返回值：0-成功      errno-失败
    if (ret < 0)
    {
        perror("pthread_create failed");
        return -1;
    }
    printf("main pthread ID -> %d\n", pthread_self());
    printf("child pthread ID -> %d\n", tid);
    while (time--)
    {
        printf("Do homework------\n");
        sleep(1);
    }
    return 0;
}

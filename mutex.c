//黄牛买票
//用互斥锁实现同步，当一个黄牛（线程）买票时，其他黄牛（线程）不能挤进来
//一个黄牛开始买票，互斥锁上锁，其他黄牛无法获得锁，等待
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

int ticket = 100;
pthread_mutex_t mutex;
//互斥锁的初始化有两种方式：
//  1. 定义时直接赋值初始化，最后不需要手动释放
//  2. 函数接口初始化，最后需要手动释放
//  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* sell_ticket(void* arg)
{
    int id = (int)arg;
    while (1)
    {
        usleep(100);
        pthread_mutex_lock(&mutex);
		//  int pthread_mutex_lock(pthread_mutex_t *mutex);
        //      阻塞加锁，如果获取不到锁则阻塞等待锁被解开
        //  int pthread_mutex_trylock(pthread_mutex_t *mutex);
        //      非阻塞加锁，如果获取不到锁则立即报错返回EBUSY
        //  int pthread_mutex_timedlock (pthread_mutex_t *mutex, struct timespec *t);
        //      限时阻塞加锁，如果获取不到锁则等待指定时间，在这段
        //      时间内如果一直获取不到，则报错返回，否则加锁
        if (ticket > 0)
        {
            ticket--;
            printf("Ticket scalper %d buy a ticket, tickets left %d\n", id, ticket);
        }
        else
        {
			//如果票卖完了也需要解锁
			//如果不解锁，那线程退出，锁未解锁，造成死锁
			//后面线程无法获取锁状态卡死
            printf("Ticket scalper %d want to buy a ticket, but no ticket left! -> %d\n", id, ticket);
            pthread_mutex_unlock(&mutex);
			//int pthread_mutex_unlock(pthread_mutex_t *mutex);
			//	解锁
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main()
{
    pthread_t tid[4];
    int i, ret;
	pthread_mutex_init(&mutex, NULL);
	//int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
    //      mutex： 互斥锁变量
    //      attr：互斥锁的属性，NULL；
    //  	返回值：0-成功      errno-失败
    printf("Train station has %d tickets left, start selling tickets\n", ticket);
    for (i = 0; i < 4; i++)
    {
        ret = pthread_create(&tid[i], NULL, sell_ticket, (void*)(i + 1));
        if (ret < 0)
        {
            perror("pthread_create failed\n");
            return -1;
        }
    }
    for (i = 0; i < 4; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}
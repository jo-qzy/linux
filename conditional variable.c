//使用pthread_cond和pthread_mutex实现同步
//生产者生产鸡腿，消费者买鸡腿
//生产者有生产计划，生产完了就不卖了
//消费者没鸡腿买了很伤心
//程序运行结束，干掉消费者和生产者

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mutex;
pthread_cond_t cond;
//1. 定义条件变量
//  条件变量的初始化有两种方式
//      1. 定义赋值初始化，不需释放
//      2. 函数接口初始化, 需要释放
//      pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int chicken_leg = 0;
int production_plan = 10;

void* produce()
{
    while (1)
    {
        //给生产者加互斥锁，没鸡腿的时候防止消费者进来买
        pthread_mutex_lock(&mutex);
        chicken_leg++;
        production_plan--;
        printf("Produce chicken leg~~\n");
        if (production_plan == 0)
        {
            //完成生产计划，休息，解锁，别忘了，不然死锁
            printf("All purduction plan complete, have a rest~~\n");
            pthread_mutex_unlock(&mutex);
            //解锁，并告诉生产者生产完毕
            pthread_cond_broadcast(&cond);
            break;
        }
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
        //int pthread_cond_broadcast(pthread_cond_t *cond);
        //  唤醒所有等待在条件变量上的线程
        //int pthread_cond_signal(pthread_cond_t *cond);
        //  唤醒第一个等待在条件变量上的线程
        sleep(2);
    }
    return NULL;
}

void* consume()
{
    //没有面就要等待
    //int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
    //  pthread_cond_wait就是用来睡眠的
    //  条件变量是和互斥锁搭配使用的
    //  是先对互斥锁做了一个判断是否加锁，如果加锁了就解锁
    //  然后陷入等待*******整个过程是原子操作
    //
    //  要防止的情况就是：假如没有鸡腿，但是消费者又速度比较快，
    //  先拿到锁了，那么生产者将拿不到锁，没法生产将会造成双方卡死
    //  所以如果消费者如果先获取到锁，那么在陷入等待之前需要解锁
    //  而这里的锁的存在是为了保护这个全局的条件的操作是受保护的。(basket)
    while (1)
    {
        //如果消费者进来，先加锁
        pthread_mutex_lock(&mutex);
        if (chicken_leg == 0)
        {
            //没鸡腿卖，那让消费者先等着，并解锁
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Producer buy a chicken leg~~\n");
        chicken_leg--;
        pthread_mutex_unlock(&mutex);
        if (production_plan == 0)
        {
            //鸡腿卖完了，伤心
            printf("No more chciken leg to buy -_-||, sad-----\n");
            break;
        }
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t consumer, producer;
    int ret;
	//初始化条件变量和互斥锁
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    ret = pthread_create(&producer, NULL, produce, NULL);
    if (ret < 0)
    {
        perror("Pthread_create failed");
        return -1;
    }
    ret = pthread_create(&consumer, NULL, consume, NULL);
    if (ret < 0)
    {
        perror("Pthread_create failed");
        return -1;
    }
    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
    return 0;
}

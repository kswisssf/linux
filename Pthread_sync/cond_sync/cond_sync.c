/*
条件变量的优点:
  相较于mutex而言，条件变量可以减少竞争。
  如直接使用mutex，除了生产者、消费者之间要竞争互斥量以外，消费者之间也需要竞争互斥量，
  但如果汇聚（链表）中没有数据，消费者之间竞争互斥锁是无意义的。有了条件变量机制以后，
  只有生产者完成生产，才会引起消费者之间的竞争。提高了程序效率。 

一、条件变量:
  1.与互斥锁不同，条件变量是用来等待而不是用来上锁的。条件变量用来自动阻塞一个线程，直到某特殊
	情况发生为止。通常条件变量和互斥锁同时使用。
  2.条件变量使我们可以睡眠等待某种条件出现。条件变量是利用线程间共享的全局变量进行同步的一种机制，
    主要包括两个动作：一个线程等待"条件变量的条件成立"而挂起；
    另一个线程使"条件成立"（给出条件成立信号）。
  3.条件的检测是在互斥锁的保护下进行的。如果一个条件为假，一个线程自动阻塞，并释放等待状态改变的互斥锁。
    如果另一个线程改变了条件，它发信号给关联的条件变量，唤醒一个或多个等待它的线程，重新获得互斥锁，
    重新评价条件。如果两进程共享可读写的内存，条件变量可以被用来实现这两进程间的线程同步。
  4.使用条件变量之前要先进行初始化。可以在单个语句中生成和初始化一个条件变量如：
    pthread_cond_t my_condition=PTHREAD_COND_INITIALIZER;（用于进程间线程的通信）。
    可以利用函数pthread_cond_init动态初始化。

二、条件变量分为两部分: 条件和变量。
  1.条件本身是由互斥量保护的。
  2.线程在改变条件状态前先要锁住互斥量。
  3.它利用线程间共享的全局变量进行同步的一种机制。

三、主要应用函数:
  1. 初始化一个条件变量
	 int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
	 参2：attr表条件变量属性，通常为默认值，传NULL即可，也可以使用静态初始化的方法，初始化条件变量：
	 pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

  2. 销毁一个条件变量
	 int pthread_cond_destroy(pthread_cond_t *cond);
  
  3. 阻塞等待一个条件变量
     int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
	 函数作用：
	 (1) 阻塞等待条件变量cond（参1）满足
	 (2) 释放已掌握的互斥锁（解锁互斥量）相当于pthread_mutex_unlock(&mutex); 1.2.两步为一个原子操作。
	 (3) 当被唤醒，pthread_cond_wait函数返回时，解除阻塞并重新申请获取互斥锁pthread_mutex_lock(&mutex);
	
  4. 限时等待一个条件变量
	 int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, const struct timespec *restrict abstime)
	 参3： 参看man sem_timedwait函数，查看struct timespec结构体。
	 struct timespec
	 {
    	time_t tv_sec;  // seconds  秒
    	long   tv_nsec; // nanosecondes 纳秒
	 }
    形参abstime：绝对时间。
	如：time(NULL)返回的就是绝对时间。而alarm(1)是相对时间，相对当前时间定时1秒钟。
	struct timespec t = {1, 0};
	pthread_cond_timedwait (&cond, &mutex, &t); 只能定时到 1970年1月1日 00:00:01秒(早已经过去) 
	
	>>正确用法：
	time_t cur = time(NULL); 获取当前时间。
	struct timespec t; 定义timespec 结构体变量t
	t.tv_sec = cur+1; 定时1秒
	pthread_cond_timedwait (&cond, &mutex, &t); 传参 参APUE.11.6线程同步条件变量小节
	
  5. 唤醒至少一个阻塞在条件变量上的线程
	 int pthread_cond_signal(pthread_cond_t *cond);

  7. 唤醒全部阻塞在条件变量上的线程
	 int pthread_cond_broadcast(pthread_cond_t *cond);

*/

/*
DEMO DESC:
	生产者消费者条件变量模型
	线程同步典型的案例即为生产者消费者模型，而借助条件变量来实现这一模型，是比较常见的一种方法。
	假定有两个线程，一个模拟生产者行为，一个模拟消费者行为。两个线程同时操作一个共享资源
	（一般称之为汇聚），生产向其中添加产品，消费者从中消费掉产品。

*/

/*借助条件变量模拟 生产者-消费者 问题*/
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
 
/*链表作为公享数据,需被互斥量保护*/
struct msg{
	struct msg *next;
	int num;
};

struct msg *head;
struct msg *mp;

/* 静态初始化 一个条件变量和一个互斥量*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;

void *producer(void *p)
{
	for(;;)
	{
		
		mp = (struct msg *)malloc(sizeof(struct msg));
		mp->num = rand()%1000 + 1;		 //模拟生产一个产品
		printf("\n---product ---:%d--\n", mp->num);

		pthread_mutex_lock(&mutex);
		mp->next = head;      			 //头插法
		head = mp;
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&has_product);//将等待在该条件变量上的一个线程唤醒
		sleep(rand() % 5);
	}
}

void *consumer_a(void *p)
{
	for(;;)
	{
		pthread_mutex_lock(&mutex);
		if(head == NULL)
		{
			//头指针为空,说明没有节点
			pthread_cond_wait(&has_product, &mutex);
		}
		mp = head;
		head = head->next;	//模拟消费掉一个产品
		pthread_mutex_unlock(&mutex);

		printf("===consumer_a---%d\n", mp->num);
		free(mp);
		mp = NULL;
		sleep(rand() % 5);
		
	}

}

void *consumer_b(void *p)
{
	for(;;)
	{
		pthread_mutex_lock(&mutex);
		if(head == NULL)
		{
			//头指针为空,说明没有节点 
			pthread_cond_wait(&has_product, &mutex);
		}
		mp = head;
		head = head->next; //模拟消费掉一个产品
		pthread_mutex_unlock(&mutex);

		printf("===consumer_b---%d\n", mp->num);
		free(mp);
		mp = NULL;
		sleep(rand() % 5);
	}
}

int main(int argc, char *argv[])
{
	pthread_t pid_pc, pid_c[2];
	srand(time(NULL));
	int ret;

	ret = pthread_create(&pid_pc, NULL, producer, NULL);
	if(ret != 0)
	{
		perror("create pid_pc\n");
		exit(1);
	}

	ret = pthread_create(&pid_c[0], NULL, consumer_a, NULL);
	if(ret != 0)
	{
		perror("create pid_c[0]\n");
		exit(1);
	}

	ret = pthread_create(&pid_c[1], NULL, consumer_b, NULL);
	if(ret != 0)
	{
		perror("create pid_c[1]\n");
		exit(1);
	}

	pthread_join(pid_pc, NULL);
	pthread_join(pid_c[0], NULL);
	pthread_join(pid_c[1], NULL);

	return 0;
}
/*
读写锁:
	读写锁适合于对数据结构的读次数比写次数多得多的情况。因为，读模式锁定时可以共享，
	以写模式锁住时意味着独占，所以读写锁又叫共享-独占锁（共享互斥锁）。

读写锁的行为:
	|----------|----------|------------|----------|
	|当前锁状态  |读锁请求	  |	写锁请求 	   |		  |
	|----------|----------|------------|----------|
	|   无锁    |   可以    |	  可以 	   |		  |          
	|----------|----------|------------|----------|
	|   读锁    |   可以	  |	  阻塞 	   |		  |
	|----------|----------|------------|----------|
	|   写锁    |   阻塞	  |	  阻塞 	   |		  |
	|----------|----------|------------|----------|	

读写互斥/写写互斥:
	即写的时候独占整个临界区，其他线程只能等待，读的实时是共享的。 
	如果要进行读和写，写的优先级高，因为只有写了，才可以读。
	写独占，读共享，写的优先级高.

读写锁特性:
	读写锁是“写模式加锁”时,解锁前，所有对该锁加锁的线程都会被阻塞。
	读写锁是“读模式加锁”时,如果线程以读模式对其加锁会成功；如果线程以写模式加锁会阻塞。
	读写锁是“读模式加锁”时,既有试图以写模式加锁的线程，也有试图以读模式加锁的线程。
	    那么读写锁会阻塞随后的读模式锁请求。优先满足写模式锁。读锁、写锁并行阻塞，写锁优先级高。
	读写锁也叫共享-独占锁。当读写锁以读模式锁住时，它是以共享模式锁住的；当它以写模式锁住时，
		它是以独占模式锁住的。写独占、读共享。读写锁非常适合于对数据结构读的次数远大于写的情况。

主要应用函数:
	1.初始化一把读写锁
	int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
	参2：attr表读写锁属性，通常使用默认属性，传NULL即可。

	2.销毁一把读写锁
	int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
	
	3.以读方式请求读写锁。（常简称为：请求读锁）
	int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);

	4.以写方式请求读写锁。（常简称为：请求写锁）
	int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

	5.解锁
	int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

	6.非阻塞以读方式请求读写锁（非阻塞请求读锁）
	int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);

	7.非阻塞以写方式请求读写锁（非阻塞请求写锁）
	int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);

	以上7 个函数的返回值都是：成功返回0， 失败直接返回错误号。
	pthread_rwlock_t类型 用于定义一个读写锁变量。
	pthread_rwlock_t rwlock;

*/

/*
DESC(description):
	3个线程不定时 "写" 全局资源，5个线程不定时 "读" 同一全局资源
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int  cir = 20;
int  counter;				//全局资源
pthread_rwlock_t rwlock;

void *pth_write(void *arg)
{
	int t;
	int i = (int)(arg);

	while((cir--) > 0)
	{
		t = counter;
		usleep(1000);

		pthread_rwlock_wrlock(&rwlock);		// 请求写锁
		printf("cir:%d=====write: %d: %lu: t counter = %d ++counter = %d\n",cir, i, pthread_self(),t, ++counter);
		pthread_rwlock_unlock(&rwlock);

		sleep(1);
	}

	return NULL;
}

void *pth_read(void *arg)
{
	int i = (int)(arg);

	while((cir--) > 0)
	{
		pthread_rwlock_rdlock(&rwlock);		// 请求读锁
		printf("cir:%d-----read %d: %lu: counter = %d\n", cir, i, pthread_self(), counter);
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}

	return NULL;
}

int main(void)
{
	int i;
	pthread_t ptd[8];

	pthread_rwlock_init(&rwlock, NULL);

	for(i = 0; i < 3; i++)
		pthread_create(&ptd[i], NULL, pth_write, (void *)i);

	for(i = 0; i < 5; i++)
		pthread_create(&ptd[i+3], NULL, pth_read, (void *)(i+3));

	for(i = 0; i < 8; i++)
		pthread_join(ptd[i], NULL);

	pthread_rwlock_destroy(&rwlock);		//释放读写琐

	return 0;
}


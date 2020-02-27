#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>
/*
一、同步概念:
	所谓同步，即同时起步，协调一致。不同的对象，对“同步”的理解方式略有不同。如:
	1)设备同步:  指在两个设备之间规定一个共同的时间参考；
	2)数据库同步:指让两个或多个数据库内容保持一致，或者按需要部分保持一致；
	3)文件同步: 指让两个或多个文件夹里的文件保持一致。

二、目	的:
	是为了避免数据混乱，解决与时间有关的错误。实际上，不仅线程间需要同步，进程间、信号间等等
	都需要同步机制。因此，所有“多个控制流，共同操作一个共享资源”的情况，都需要同步。

!!数据混乱原因:
	资源共享（独享资源则不会）
	调度随机（意味着数据访问会出现竞争）
	线程间缺乏必要的同步机制 (数据实时更新)

三、主要函数：
	pthread_mutex_init      初始化一个互斥锁(互斥量) ---> 初值可看作1	
	pthread_mutex_destroy	销毁一个互斥锁
	pthread_mutex_lock      加锁。可理解为将mutex--（或-1）
	pthread_mutex_trylock   尝试加锁
	pthread_mutex_unlock    解锁。可理解为将mutex ++（或+1）

	//以上5个函数的返回值都是：成功返回0， 失败返回错误号。
	//pthread_mutex_t 类型，其本质是一个结构体。为简化理解，应用时可忽略其实现细节，简单当成整数看待。
	//pthread_mutex_t mutex; 变量mutex只有两种取值1、0。

四、加锁与解锁 

lock与unlock：
	lock尝试加锁，如果加锁不成功，线程阻塞，阻塞到持有该互斥量的其他线程解锁为止。
	unlock主动解锁函数，同时将阻塞在该锁上的所有线程全部唤醒，至于哪个线程先被唤醒，
	取决于优先级、调度。
	默认：先阻塞、先唤醒。例如：T1、T2、T3、T4 使用一把mutex锁。
	T1加锁成功，其他线程均阻塞，直至T1解锁。T1解锁后，T2、T3、T4均被唤醒，并自动再次尝试加锁。
	
	!!可假想mutex锁init成功初值为1.
	  lock 功能是将mutex--, unlock将mutex++ .

lock与trylock：
	lock加锁失败会阻塞，等待锁释放。
	trylock加锁失败直接返回错误号（如：EBUSY），不阻塞。


*/

pthread_mutex_t mutex;	//定义锁
/*
五、函数原型：
int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
   其中参数 mutexattr 用于指定锁的属性（见下），如果为NULL则使用缺省属性。互斥锁的属性在创建锁的时候指定，
   在LinuxThreads实现中仅有一个锁类型属性，不同的锁类型在试图对一个已经被锁定的互斥锁加锁时表现不同。
   当前有四个值可供选择：
   （1）PTHREAD_MUTEX_TIMED_NP，这是缺省值，也就是普通锁。当一个线程加锁以后，其余请求锁的
       线程将形成一个等待队列，并在解锁后按优先级获得锁。这种锁策略保证了资源分配的公平性。
   （2）PTHREAD_MUTEX_RECURSIVE_NP，嵌套锁，允许同一个线程对同一个锁成功获得多次，并
       通过多次unlock解锁。如果是不同线程请求，则在加锁线程解锁时重新竞争。
   （3）PTHREAD_MUTEX_ERRORCHECK_NP，检错锁，如果同一个线程请求同一个锁，则返回EDEADLK，
       否则与PTHREAD_MUTEX_TIMED_NP类型动作相同。这样就保证当不允许多次加锁时不会出现
       最简单情况下的死锁。
   （4）PTHREAD_MUTEX_ADAPTIVE_NP，适应锁，动作最简单的锁类型，仅等待解锁后重新竞争。

*/

int tcnt = 5;

//将unlock挪至第二个sleep后，发现交替现象很难出现
#define LOCK_TEST		0
//模拟长时间操作共享资源，导致cpu易主，产生与时间有关的错误

void *func_1(void *arg)
{
	srand(time(NULL));

	while((tcnt--) > 0)
	{
		pthread_mutex_lock(&mutex);  //mutex--
		printf("tcnt: %d hello ",tcnt);
		sleep(rand() % 3);	//模拟长时间操作共享资源，导致cpu易主，产生与时间有关的错误
		printf("world\n");
		#if LOCK_TEST
		pthread_mutex_unlock(&mutex);  //mutex++
		sleep(rand() % 3);	//两个线程交替打印
		#else
		sleep(rand() % 3);	//将unlock挪至第二个sleep后，发现交替现象很难出现。	
		pthread_mutex_unlock(&mutex);  //mutex++
		#endif
	}

	return NULL;
} 

/*
    在访问共享资源前加锁，访问结束后立即解锁。锁的“粒度”应越小越好。
    在使用互斥量的时候，要避免死锁。
    死锁：（1）线程试图对同一个互斥量加锁两次（解决：写代码时加锁环节不写复杂）；
         （2）交叉锁：线程1拥有A锁，请求获得B锁；线程2拥有B锁，请求获得A锁
             （解决：每个线程申请锁的顺序要抑制，如果申请一把锁，申请另外一把锁的时候，申请失败，应该释放已经掌握的）。
*/

void *func_2(void *arg)
{	
	//srand(time(NULL));

	while((tcnt--) > 0)
	{
		pthread_mutex_lock(&mutex);   //mutex--
		printf("tcnt: %d HELLO ",tcnt);
		sleep(rand() %3);
		printf("WORLD\n");

		#if LOCK_TEST
		pthread_mutex_unlock(&mutex);  //mutex++
		sleep(rand() % 3);
		#else
		sleep(rand() % 3);		//将unlock挪至第二个sleep后，发现交替现象很难出现。
		pthread_mutex_unlock(&mutex);  //mutex++
		#endif
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t ptd1, ptd2;
	int ret;

	//mutex==1
	pthread_mutex_init(&mutex, NULL);	

	//ptd2线程放到ptd1之前起来，可以先抢到锁，否则进入ptd1死循环
	ret = pthread_create(&ptd2, NULL, func_2, NULL);
	if(ret != 0)
	{
		perror("pthread_create func_2");
		exit(1);
	}

	ret = pthread_create(&ptd1, NULL, func_1, NULL);
	if(ret != 0)
	{
		perror("pthread_create func_1");
		exit(1);
	}


	//ptd2使用计数结束线程，join阻塞等待，完成ptd2之后才能执行后面程序
	pthread_join(ptd2, NULL);

	//ptd1是用cancel取消回调函数循环的线程
	pthread_cancel(ptd1);
	pthread_join(ptd1, NULL);

	pthread_mutex_destroy(&mutex);

	return 0;
}
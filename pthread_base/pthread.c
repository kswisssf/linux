#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <pthread.h>

/*
一、pthread_t pthread_self(void);
	函数作用：获得线程自身的ID。
	pthread_t的类型为unsigned long int，所以在打印的时候要使用%lu方式，否则显示结果出问题。

二、int pthread_create(pthread_t *restrict tidp,const pthread_attr_t *restrict_attr,void*（*start_rtn)(void*),void *restrict arg);
	返回值:
	若成功则返回0，否则返回出错编号
	参数:
	第一个参数为指向线程标识符的指针，
	第二个参数用来设置线程属性，
	第三个参数是线程运行函数的起始地址， 
	最后一个参数是运行函数的参数。
	
	pthread_create是UNIX环境创建线程函数
	在编译时注意加上-lpthread参数，以调用静态链接库。因为pthread并非Linux系统的默认库

*/

int value = 0;

int pthread_func_a(char *msg)
{
	int i = 0;
	pthread_t tid = pthread_self();

	for(; i<10; i++)
	{
		printf("this is pthread func a " 
			   "and value = %d and i = %d\n"
			   "msg:%s\t tid= %u\n", ++value, i, msg, (unsigned int)tid);
		sleep(1);
	}

	return 0;
}

int pthread_func_b(char *msg)
{
	int i = 0;
	pthread_t tid = pthread_self();

	for(; i<10; i++)
	{
		if(i==6)
		{
			printf("pthread func b will exit\n");
			pthread_exit(NULL);

		}

		printf("this is pthread func b " 
			   "and value = %d and i = %d\n"
			   "msg:%s\t tid= %u\n", ++value, i, msg, (unsigned int)tid);
		sleep(1);
	}

	return 0;
}
/*
三、等待线程:
函数定义： 
	int pthread_join(pthread_t thread, void **retval);
描述 ：
	以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。
	如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的。
参数 ：
	thread: 线程标识符，即线程ID，标识唯一线程。retval: 用户定义的指针，用来存储被等待线程的返回值。
返回值 ： 
	0代表成功。 失败，返回的则是错误号。


四、终止线程:
方式1：在一个线程中return:
	如果线程通过return返回，那么retval所指向的单元里存放的是tread函数的返回值
	例：main函数创建一个新线程，新线程执行完自己的函数，使用return退出，那么返回值就是退出码

方式2：exit
　　 如果线程是自己调用exit终止的，那么就是直接退出，并且exit表示进程的退出
 	exit和return的区别：
 	return是函数的退出，exit是进程的退出

	return执行结束后会调用exit或和exit类似的函数，return会释放局部变量并且弹出栈桢，
	回到上一个函数继续执行

方式3：使用pthread_exit()
	void pthread_exit(void* retval);
	就如同进程在结束时调用exit函数一样。这个函数的作用是，终止调用它的线程并返回一个指向某个对象的指针。

	也就是说，一个线程可以隐式的退出，也可以显式的调用pthread_exit函数来退出。
	pthread_exit函数唯一的参数value_ptr是函数的返回代码，只要pthread_join中的
	第二个参数value_ptr不是NULL，这个值将被传递给value_ptr。

五、线程取消:
	int pthread_cancel(pthread_t thread)
	发送终止信号给thread线程，如果成功则返回0，否则为非0值。发送成功并不意味着thread会终止。
	若是在整个程序退出时，要终止各个线程，应该在成功发送CANCEL 指令后，使用pthread_join 函数，
	等待指定的线程已经完全退出以后，再继续执行；否则，很容易产生“段错误”。

六、线程分离
函数原型：
	int pthread_detach(pthread_t tid);
参   数：
tid：线程标识符

返回值：
	pthread_detach() 在调用成功完成之后返回零。其他任何返回值都表示出现了错误。
	如果检测到以下任一情况，pthread_detach()将失败并返回相应的值。
	EINVAL：tid是分离线程
	ESRCH：tid不是当前进程中有效的为分离线程
作  用:
	 由系统来回收线程所占用资源
功  能：
	pthread_join()函数的替代函数，可回收创建时detachstate属性设置为PTHREAD_CREATE_JOINABLE的
	线程的存储空间。该函数不会阻塞父线程。pthread_join()函数用于只是应用程序在线程tid终止时回收
	其存储空间。如果tid尚未终止，pthread_detach()不会终止该线程。
	当然pthread_detach(pthread_self())也是可以的.

有两种方式创建分离线程：
	（1）在线程创建时将其属性设为分离状态(detached)；
	（2）在线程创建后将其属性设为分离的(detached)

	创建一个线程默认的状态是joinable, 如果一个线程结束运行但没有被join,则它的状态类似于
	进程中的Zombie Process,即还有一部分资源没有被回收（退出状态码），所以创建线程者应该
	pthread_join来等待线程运行结束，并可得到线程的退出代码，回收其资源（类似于wait,waitpid)

*/

/*
七、线程清理:
函数声明:
	void pthread_cleanup_push(void (*rtn)(void *)，void *arg);
	void pthread_cleanup_pop(int execute);
参   数:
	rtn 处理程序入口地址
	arg 传递给处理函数的参数
编译链接参数:
	-pthread
注意事项:
	1）如果线程只是由于简单的返回而终止的，则清除函数不会被调用。
	2）如果pthread_cleanup_pop被传递0参数，则清除函数不会被调用，但是会清除处于栈顶的清理函数

pthread_cleanup_push来注册清理函数rtn,这个函数有一个参数arg。
   在以下三种情形之一发生时，注册的清理函数被执行：
　　1）调用pthread_exit。
　　2）作为对取消线程请求(pthread_cancel)的响应。
　　3）以非0参数调用pthread_cleanup_pop。

	一般来说，Posix的线程终止有两种情况：正常终止和非正常终止。
	线程主动调用pthread_exit()或者从线程函数中return都将使线程正常退出，这是可预见的退出方式；
	非正常终止是线程在其他线程的干预下，或者由于自身运行出错（比如访问非法地址）而退出，这种退出方式
	是不可预见的。

	不论是可预见的线程终止还是异常终止，都会存在资源释放的问题，
	!!在不考虑因运行出错而退出的前提下，如何保证线程终止时能顺利的释放掉自己所占用的资源，
	特别是锁资源，就是一个必须考虑解决的问题。

	pthread_cleanup_push()/pthread_cleanup_pop()采用先入后出的栈结构管理，
	void routine(void *arg)函数在调用pthread_cleanup_push()时压入清理函数栈，
	多次对pthread_cleanup_push()的调用将在清理函数栈中形成一个函数链，在执行该函数链时按照压栈的
	相反顺序弹出。execute参数表示执行到pthread_cleanup_pop()时是否在弹出清理函数的同时执行该函数，
	为0表示不执行，非0为执行；这个参数并不影响异常终止时清理函数的执行。
	仅仅在线程调用pthread_exit函数或者其它线程对本线程调用 pthread_cancel函数时，
	才在弹出“清理函数”的同时执行该“清理函数”
*/

void clean_fun1(void *arg)
{
	printf("this is clean fun1\n");
}

void clean_fun2(void *arg)	//先入后出的栈结构管理,clean_fun2会先执行!
{
	printf("this is clean fun2\n");
}

//屏蔽掉pthread_cancel响应条件之后,测试execute在弹出清理函数的同时执行该清理函数,为0表示不执行,非0为执行。
#define NO_PTHREAD_CANCEL	0	

void * thread_fun(void *arg)
{
	pthread_cleanup_push(clean_fun1,NULL);	//push和pop要成对出现.
	pthread_cleanup_push(clean_fun2,NULL);	//先入后出的栈结构管理,clean_fun2会先执行!
	sleep(5);
	//这里要注意,如果将sleep(100)换成while(1)的话,程序会一直暂停.
	//因为while(1);运行的太快,线程不接受cancel信号
	//while(1);
	
	#if NO_PTHREAD_CANCEL
	pthread_cleanup_pop(0);	//push和pop要成对出现.
	pthread_cleanup_pop(0);	//先入后出的栈结构管理,clean_fun2会先执行!
	#else
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);		
	#endif

	return NULL;
}

int pthread_clean_demo(void *arg)
{
	pthread_t tid1;
	int ret;

    ret = pthread_create(&tid1, NULL, thread_fun, NULL);
    if(ret != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	sleep(2);

	/*
	pthread_cleanup_push来注册清理函数rtn,这个函数有一个参数arg。
    在以下三种情形之一发生时，注册的清理函数被执行：
　　 1）调用pthread_exit。
　　 2）作为对取消线程请求(pthread_cancel)的响应。
　　 3）以非0参数调用pthread_cleanup_pop。

	屏蔽掉pthread_cancel响应条件之后,测试execute在弹出清理函数的同时执行该清理函数,为0表示不执行,非0为执行。
	*/
	#if NO_PTHREAD_CANCEL	
	printf("pthread cancel\n");
	ret = pthread_cancel(tid1);
	if(ret !=0 )
	{
		perror("cancel error:");
		exit(1);
	}
	#endif
	ret = pthread_join(tid1, NULL);
	if(ret != 0)
	{
		perror("pthread_joinerror:");
		exit(1);
	}
 
	return 0;
}


int main(int argc, char *argv[])
{
	pthread_t pthread_ta, pthread_tb;
	printf("pthread_t size is:%d\n",sizeof(pthread_t));

	if(argc!=3)
	{
		printf("usage :%s msg1 msg2\n", argv[0]);
		exit(1);
	}	

	if(pthread_create(&pthread_ta, NULL, (void *)pthread_func_a, argv[1]) == -1)
	{
		perror("pthread_ta\n");
		exit(1);
	}

	if(pthread_create(&pthread_tb, NULL, (void *)pthread_func_b, argv[2]) == -1)
	{
		perror("pthread_tb\n");
		exit(1);
	}

	pthread_join(pthread_ta, NULL);
	pthread_join(pthread_tb, NULL);

	printf("run pthread clean demo\n");
	pthread_clean_demo(NULL);

	return 0;
}

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include <signal.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>

#define MEMSIZE		1024
#define PERM					S_IRUSR|S_IWUSR

/*
Linux进程通信共享内存。共享内存函数由shmget、shmat、shmdt、shmctl四个函数组成。
所需头文件
#include <sys/types.h>
#include <sys/shm.h>
*/

//未使用函数补充说明(shmdt、shmctl)
/*
shmdt(断开共享内存连接)
函数说明:
	与shmat函数相反，是用来断开与共享内存附加点的地址，禁止本进程访问此片共享内存
函数原型:
	int shmdt(const void *shmaddr)
函数传入值:
	shmaddr：连接的共享内存的起始地址
函数返回值:
	成功：0
	出错：-1，错误原因存于error中
附加说明:
	本函数调用并不删除所指定的共享内存区，而只是将先前用shmat函数连接（attach）好的共享内存脱离（detach）目前的进程
错误代码:
	EINVAL：无效的参数shmaddr
*/

/*
shmctl(共享内存管理)
函数说明:
	完成对共享内存的控制
函数原型:
	int shmctl(int shmid, int cmd, struct shmid_ds *buf)
函数传入值:
	shmid共享内存标识符
cmd:
	IPC_STAT：得到共享内存的状态，把共享内存的shmid_ds结构复制到buf中
	IPC_SET：改变共享内存的状态，把buf所指的shmid_ds结构中的uid、gid、mode复制到共享内存的shmid_ds结构内
	IPC_RMID：删除这片共享内存
buf:
	共享内存管理结构体。具体说明参见共享内存内核结构定义部分
函数返回值:
	成功：0
	出错：-1，错误原因存于errno中
错误代码:
	EACCESS：参数cmd为IPC_STAT，确无权限读取该共享内存
	EFAULT：参数buf指向无效的内存地址
	EIDRM：标识符为shmid的共享内存已被删除
	EINVAL：无效的参数cmd或shmid
	EPERM：参数cmd为IPC_SET或IPC_RMID，却无足够的权限执行
*/

int main(int argc, char **argv)
{
	int shmid;
	char *p_addr, *c_addr;

	if(argc != 2)
	{
		fprintf(stderr, "Usage %s msg\n\a", argv[0]);
		exit(1);
	}
	/*
	int shmget(key_t key, size_t size, int shmflg)
	shmget(得到一个共享内存标识符或创建一个共享内存对象)
	函数传入值:
	key: 
	    1. 0(IPC_PRIVATE)：会建立新共享内存对象
		2. 大于0的32位整数：视参数shmflg来确定操作。通常要求此值来源于ftok返回的IPC键值
	size:
		1. 大于0的整数：新建的共享内存大小，以字节为单位
		2. 0：只获取共享内存时指定为0
	shmflg:
		1. 0：取共享内存标识符，若不存在则函数会报错
		2. IPC_CREAT：当shmflg&IPC_CREAT为真时，如果内核中不存在键值与key相等的共享内存，则新建一个共享内存；如果存在这样的共享内存，返回此共享内存的标识符
		3. IPC_CREAT|IPC_EXCL：如果内核中不存在键值与key相等的共享内存，则新建一个消息队列；如果存在这样的共享内存则报错
	*/

	shmid = shmget(IPC_PRIVATE, MEMSIZE, PERM);
	if(-1 == shmid)
	{
		printf("create shmem failed\n");
		exit(1);
	}

	//pid == 0 is child
	//pid >0 is father
	if(fork())	//create father
	{
		/*
		shmat(把共享内存区对象映射到调用进程的地址空间)
		函数传入值:
		shmid:共享内存标识符
		shmaddr:指定共享内存出现在进程内存地址的什么位置，直接指定为NULL让内核自己决定一个合适的地址位置
		shmflg:SHM_RDONLY：为只读模式，其他为读写模式
		函数返回值:
		成功：附加好的共享内存地址
		出错：-1，错误原因存于errno中
		*/
		p_addr = shmat(shmid, 0, 0); 
		memset(p_addr, 0, MEMSIZE);
		strncpy(p_addr, argv[1], MEMSIZE);
		printf("father write shmat:%s\n", argv[1]);
		wait(NULL);
		exit(0);
	}
	else
	{
		sleep(1);
		printf("child shmat\n");
		c_addr = shmat(shmid, 0, 0);
		printf("child mem get:%s\n", c_addr);
		exit(0);

	}


}
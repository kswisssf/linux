//comm.c
#include "common.h"
 
static int CommShm(int size,int flags)
{

/*
系统建立IPC通讯(消息队列、信号量和共享内存)时必须指定一个ID值。通常情况下,该id值通过ftok函数得到。

函数原型：
key_t ftok( const char * fname, int id )
	fname就是你指定的文件名（已经存在的文件名），一般使用当前目录，如：
	key = ftok(".", 1); 这样就是将fname设为当前目录。
	id是子序号。虽然是int类型，但是只使用8bits(1-255）。
	在一般的UNIX实现中，是将文件的索引节点号取出，前面加上子序号得到key_t的返回值。

*/
	key_t key = ftok(PATHNAME,PROJ_ID);
	if(key < 0)
	{
		perror("ftok");
		return -1;
	}
	int shmid = 0;
	if((shmid = shmget(key,size,flags)) < 0)
	{
		perror("shmget");
		return -2;
	}
	return shmid;
}

int DestroyShm(int shmid)
{
	if(shmctl(shmid,IPC_RMID,NULL) < 0)
	{
		perror("shmctl");
		return -1;
	}
	return 0;
}

int CreateShm(int size)
{
	return CommShm(size,IPC_CREAT | IPC_EXCL | 0666);
}

int GetShm(int size)
{
	return CommShm(size,IPC_CREAT | IPC_EXCL | 0666);
}

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include <signal.h>



//  SIGINT ---- CTRL+C
//  SIGQUIT ---- kill pid

int myfunc(int signo)
{
	if(SIGINT == signo)
		printf("get SIG_INT:%d\n",SIGINT);
	else if(SIGQUIT == signo)
		printf("get SIGQUIT:%d\n", SIGQUIT);	

	return 0;
}

// ps aux look for the pid
//ps如何通过进程名来查找进程号
//执行　ps -e | grep 进程名

/*
只查看该进程：ps -ef | grep pid
查看该进程打开的文件：lsof -p pid
查看内存分配：lcat /proc/pid/maps
查看堆栈：pstack pid
查看发出的系统调用:strace -p pid
查看调用库函数:ltrace -p pid

*/

int main(int argc, char *argv[])
{
	printf("wait for signo\n");

	signal(SIGQUIT, myfunc);
	signal(SIGINT, myfunc);

	pause();
	exit(0);

}
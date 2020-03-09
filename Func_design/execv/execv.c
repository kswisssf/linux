#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>

/*
相关函数： execl，execle，execlp，execv，execvp
表头文件： #include<unistd.h>
定义函数： int execve(const char * filename,char * const argv[ ],char * const envp[ ]);
函数说明： execve()用来执行参数filename字符串所代表的文件路径，第二个参数系利用数组指针来传递给执行文件，最后一个参数则为传递给执行文件的新环境变量数组。
返回值：   如果执行成功则函数不会返回，执行失败则直接返回-1，失败原因存于errno 中。

execv函数族：系统来调用某程序模块
    int execl(const char *path, const char *arg, ...);
    int execlp(const char *file, const char *arg, ...);
    int execle(const char *path, const char *arg,
              ..., char * const envp[]);
    int execv(const char *path, char *const argv[]);
    int execvp(const char *file, char *const argv[]);

    几个函数之间非常相似：
    字母p表示该函数取filename作为参数，并且用PATH环境变量寻找可执行文件.
    字母l表示该函数取一个参数表，它与字母v互斥.
    字母v表示该函数取一个argv[]矢量.
    字母e表示该函数取envp[]数组.

本质：1、调用此函数时（成功的执行），调用的进程模块会替换现有的进程信息。
     2、execve才是系统内核函数，上述都会调用此函数来完成的功能从指定路径下查找    
*/
#define SIZE     516
//DESC:当前目录调用ls -al 命令
int execve_demo(void)
{
	//获取新目录后的目录名
	char pathname[SIZE] = {0};
	getcwd(pathname, sizeof(pathname));

	char * argv[] = {"ls", "-al", pathname, (char *)0};
	char * envp[] = {"PATH=/bin",0};
	//execve支持传入环境变量参数，execv使用的环境变量只能通过environ获得
	execve("/bin/ls", argv, envp);

	return 0;
}

#define MAXARGS 31
/*
* DESC:execl 源码
* execl is called by
* execl(file, arg1, arg2, ..., (char *)(0));
*/
int execl_sc(const char *file, const char *args, ...)
{
    va_list ap;	//arg + p -> ap
    char *array[MAXARGS +1];
    int argno = 0;
     
    va_start(ap, args);
    while (args != 0 && argno < MAXARGS)
    {
        array[argno++] = args;
        args = va_arg(ap, const char *);
    }
    array[argno] = (char *) 0;
     
    va_end(ap);
    return 
    execv(file, array);
}

/*
定义函数： int system(const char * string);
函数说明： system()会调用fork()产生子进程，由子进程来调用/bin/sh-c string来执行
		 参数string字符串所代表的命令，此命>令执行完后随即返回原调用的进程。在调用
		 system()期间SIGCHLD 信号会被暂时搁置，SIGINT和SIGQUIT 信号则会被忽略。

返回值:  =-1:出现错误   
        =0:调用成功但是没有出现子进程   
        >0:成功退出的子进程的id

附加说明： 在编写具有SUID/SGID权限的程序时请勿使用system()，system()会继承环境变量，
		 通过环境变量可能会造成系统安全的问题。

范例：
main()
{
　　system(“ls -al /etc/passwd /etc/shadow”);
}

*/

//DESC:system源码
int system_sc(const char *cmdstring)
{
	pid_t pid;
	int status;

	if(cmdstring == NULL)
		return  1;

	if((pid = fork()) < 0){
		status = -1;
	}
	else if(pid == 0){
		execl("/bin/sh", "sh", "-c", cmdstring, (char *)0);
		_exit(127); //子进程正常执行则不会执行此_exit
	}
	else{
		while(waitpid(pid, &status, 0) < 0){
			if( errno != EINTER){
				status = -1;
				break;
			}
		}
	}
}

int main(int argc, char *argv[])
{

	printf("execve_demo running...\n");
	execve_demo();

	return 0;
}
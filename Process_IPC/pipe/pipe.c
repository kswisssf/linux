#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>


/*
1. 种类
  进程间通信主要包括管道, 系统IPC（包括消息队列,信号,共享存储), 套接字(SOCKET).

2. 管道包括三种:
  1)普通管道PIPE, 通常有两种限制,一是单工,只能单向传输;二是只能在父子或者兄弟进程间使用.
  2)流管道s_pipe: 去除了第一种限制,为半双工，可以双向传输.
  3)命名管道:name_pipe, 去除了第二种限制,可以在许多并不相关的进程之间进行通讯.

3. 命名管道(NamedPipe):
  是服务器进程和一个或多个客户进程之间通信的单向或双向管道。
  不同于匿名管道的是：命名管道可以在不相关的进程之间和不同计算机之间使用，服务器建立命名
  管道时给它指定一个名字，任何进程都可以通过该名字打开管道的另一端，根据给定的权限和服务器
  进程通信。命名管道提供了相对简单的编程接口，使通过网络传输数据并不比同一计算机上两进程
  之间通信更困难，不过如果要同时和多个进程通信它就力不从心了。

4.无名管道:
  是Linux系统内核的特殊文件，用于**父子进程之间的通信。
  无名管道相当于一个队列结构，fd[1]为写入端（入队），fd[0]为读出端（出队）。
  其中信息读出后即删除，再次读取时即为下一个信息。

  函数形式：int pipe(int fd[2])
  功能：创建无名管道文件。无名管道是一个特殊文件，不可由open函数创建。
  参数：fd[2]有两个成员fd[0]和fd[1]，他们都是文件描述符。管道有固定的读端 
       fd[0]和固定的写端 fd[1]。
  返回值：成功返回 0，出错返回-1。

说明：
  1.管道是创建在内存中，进程结束空间释放，管道不复存在。
  2.无名管道和有名管道都是半双工通信，实现双向通信需要建立两个管道。
  3.无名管道是linux特殊文件，不能在与windows共享的文件夹中创建。
  4.无名管道只用于**父子进程之间，有名管道可用于**无亲缘关系的进程之间。

*/
int pipe_fd[2];

int create_pipe(void)
{
	if(pipe(pipe_fd) < 0)
	{
		printf("create pipe failed \n");
		return -1;
	}else 
		{
			printf(" create sucessed!\n");
			printf(" pipe_fd:%d pipe_fd:%d\n", pipe_fd[0], pipe_fd[1]);
		}

	return 0;
}

void close_pipe()
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
/*
parent(fd[0])--->write--->child(fd[1])
child(fd[1]) --->read---> parent(fd[0])
*/


int pipe_talk(const char *pipe_buf, int pipe_len)
{
	pid_t pid;
	int rd_len;
	int len = pipe_len;
	char *tmp_buf = (char *)malloc(len);
	
	if((pid=fork()) == 0)
	{
		printf(" child pid\n");
		close(pipe_fd[1]);
		sleep(1);		//get out cpu

		if((rd_len=read(pipe_fd[0], tmp_buf, len)) > 0)
			printf("read pipe_fd 0 sucessed len: %d :buf :%s\n",  rd_len, tmp_buf);
		close(pipe_fd[0]);
		exit(0);
	}else if(pid >0)
		{
			printf("father pid\n");
			close(pipe_fd[0]);
			if(write(pipe_fd[1], pipe_buf , len))
				printf("write pipe_fd 1 sucessed ,msg:%s\n", pipe_buf);
			close(pipe_fd[1]);
			exit(0);
		}

	free(tmp_buf);	
	return 0;
}

int main(int argc, char *argv[])
{
	char *test_pipe = "hello child";

	create_pipe();
	pipe_talk(test_pipe, strlen(test_pipe));

	return 0;
}
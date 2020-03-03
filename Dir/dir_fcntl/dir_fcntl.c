/*

一、作用：
    改变已经打开文件的属性,
(1) 函数原型：       
    #include <unistd.h>
    #include <fcntl.h>
 
    int fcntl(int fd, int cmd, ... /arg / ); 这是一个可变长参数的函数
	fcntl()针对(文件)描述符提供控制.参数fd 是被参数cmd操作(如下面的描述)的描述符.
　　 针对cmd的值,fcntl能够接受第三个参数int arg

(2) 延展函数:
	int fcntl(int fd, int cmd); 
　　 int fcntl(int fd, int cmd, long arg); 

(3) 参数fd
	参数fd代表欲设置的文件描述符。
(4) 参数cmd
	参数cmd代表打算操作的指令。
(5) 有以下几种情况:
	1. F_DUPFD用来查找大于或等于参数arg的最小且仍未使用的文件描述符，并且复制参数fd的文件描述符。
	   执行成功则返回新复制的文件描述符。新描述符与fd共享同一文件表项，但是新描述符有它自己的一套
	   文件描述符标志，其中FD_CLOEXEC文件描述符标志被清除。请参考dup2()。
	2. F_GETFD取得close-on-exec旗标。若此旗标的FD_CLOEXEC位为0，代表在调用exec()相关
	   函数时文件将不会关闭。
	3. F_SETFD 设置close-on-exec 旗标。该旗标以参数arg 的FD_CLOEXEC位决定。
	4. F_GETFL 取得文件描述符状态旗标，此旗标为open（）的参数flags。
	5. F_SETFL 设置文件描述符状态旗标，参数arg为新旗标，但只允许O_APPEND、O_NONBLOCK
	   和O_ASYNC位的改变，其他位的改变将不受影响。
	6. F_GETLK 取得文件锁定的状态。
	7. F_SETLK 设置文件锁定的状态。此时flcok结构的l_type值必须是F_RDLCK、F_WRLCK或F_UNLCK。
	   如果无法建立锁定，则返回-1，错误代码为EACCES 或EAGAIN。
	8. F_SETLKW F_SETLK 作用相同，但是无法建立锁定时，此调用会一直等到锁定动作成功为止。
	   若在等待锁定的过程中被信号中断时，会立即返回-1，错误代码为EINTR。


二、延展函数:
　　 int fcntl(int fd, int cmd, struct flock *lock);

(1)参数lock指针
	参数lock指针为flock 结构指针，定义如下
	struct flock
	{
		short int l_type;
		short int l_whence;
		off_t l_start;
		off_t l_len;
		pid_t l_pid;
	};
	l_type 有三种状态:
		F_RDLCK 建立一个供读取用的锁定
		F_WRLCK 建立一个供写入用的锁定
		F_UNLCK 删除之前建立的锁定

	l_whence 也有三种方式:
		SEEK_SET 以文件开头为锁定的起始位置。
		SEEK_CUR 以目前文件读写位置为锁定的起始位置
		SEEK_END 以文件结尾为锁定的起始位置。
	
	l_start 表示相对l_whence位置的偏移量，两者一起确定锁定区域的开始位置。
	
	l_len表示锁定区域的长度，如果为0表示从起点(由l_whence和 l_start决定的开始位置)
	开始直到最大可能偏移量为止。即不管在后面增加多少数据都在锁的范围内。
	
(2)返回值
		成功返回依赖于cmd的值，若有错误则返回-1，错误原因存于errno.

*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fcntl_demo(void)
{

	int flag;
	int get_flag;
	int fd;

	//测试字符串
	char *msg1 = "china is good\n";
	char *msg2 = "you are lying\n";

	//以只写方式打开文件
	fd = open("test.txt", O_WRONLY|O_CREAT, 0766);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	//输入新的内容，该内容会覆盖原来的内容
	if(write(fd, msg1, strlen(msg1)) == -1)
	{
		perror("write");
		exit(1);
	}

	//使用 F_GETFL 命令得到文件状态标志
	get_flag = fcntl(fd, F_GETFL, 0);
	if(get_flag == -1)
	{
		perror("F_GETFL");
		exit(1);
	}

	printf("get_flag : %d\n", get_flag);

	//将文件状态标志添加 “追加写” 选项
	flag = get_flag|O_APPEND;
	// 将文件状态修改为追加写
	if(fcntl(fd, F_SETLK, flag) == -1)
	{
		perror("F_SETLK");
		exit(1);
	}

	//再次输入新的内容，该内容会追加到旧内容对的后面
	if( write(fd, msg2, strlen(msg2)) == -1)
	{
		perror("write msg2");
		exit(1);
	}

	return 0;
}


lockf、flock的区别

int main(int argc, char *argv[])
{
	printf("fcntl_demo running ...\n");

	fcntl_demo();

	return 0;
}


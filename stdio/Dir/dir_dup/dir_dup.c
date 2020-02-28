/*
在linux下，一切皆文件。当文件被打开时，会返回文件描述符用于操作该文件，从shell中运行一个进程，
  默认会有3个文件描述符存在(0、１、2)；）0表示标准输入，1表示标准输出，2表示标准错误。
  一个进程当前有哪些打开的文件描述符可以通过/proc/进程ID/fd目录查看。

一、dup和dup2函数:
    1. 作用：复制现有的文件描述符,使多个文件描述符指向同一个文件。
    		通常使用这两个系统调用来重定向一个打开的文件描述符。
    2. 函数原型：
	      int dup(int oldfd);
          int dup2(int oldfd, int newfd);
 	3. 返回值
　     成功：dup函数返回当前系统可用的最小整数值。 
　　　　
      dup2函数返回第一个不小于newfd的整数值。也就是分为两种情况： 
　　　　①、如果newfd已经打开，则先将其关闭，再复制文件描述符。 
　　　　②、如果newfd等于oldfd，则dup2返回newfd, 而不关闭它。


!!ps: 只需要close(fd[0])，第一个fd，即Open返回的最小值

*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
DEMO DESC:
多次使用dup, 传入上一次返回的fd，dup出新的fd。
只需要close(fd[0])，第一个fd，即Open返回的最小值
*/

#define PATHNAME	"tmp.txt"
#define NUM			5

int dup_demo(void)
{
	//多次使用dup, 传入上一次返回的fd，dup出新的fd。
	int fd[NUM] = {0};
	int ret;
	int i;

/*
!!ps：open函数具体使用那个，和具体应用场景相关，如目标文件存在，使用两个参数的open，
如果目标文件不存在，需要open创建，则第三个参数表示创建文件的默认权限
	
	int open(const char*pathname,int flags);
	int open(const char*pathname,int flags,mode_t mode);
参数说明：
	1.pathname
  		要打开或创建的目标文件
	2.flags
  		打开文件时，可以传入多个参数选项，用下面的
  		一个或者多个常量进行“或”运算，构成falgs
  		参数：
  		O_RDONLY:   只读打开
 		O_WRONLY:   只写打开
  		O_RDWR:     读，写打开
  		打开/创建文件时，至少得使用上述三个常量中的一个。以下常量是选用的：

		O_APPEND	每次写操作都写入文件的末尾
		O_CREAT		如果指定文件不存在，则创建这个文件
		O_EXCL		如果要创建的文件已存在，则返回-1，并且修改errno的值
		O_TRUNC		如果文件存在，并且以只写/读写方式打开，则清空文件全部内容(即将其长度截短为0)
		O_NOCTTY	如果路径名指向终端设备，不要把这个设备用作控制终端。
		O_NONBLOCK	如果路径名指向FIFO/块文件/字符文件，则把文件的打开和后继I/O              

返回值:
	成功：新打开的文件描述符
	失败：-1
	open返回的文件描述符一定是最小的而且没有被使用的

fopen与open的区别
  	以可写的方式fopen一个文件时，如果文件不存在则会自动创建，而open一个文件时必须明确O_CREAT才会创建文件，否则文件不存在就出错返回
*/

	fd[0] = open(PATHNAME, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if(fd[0] == -1)
	{
		perror("open");
		exit(1);
	}

	printf("file open fd[0]:%d\n", fd[0]);

    /* 
    找到进程文件描述表中第一个可用的文件描述符
    将参数指定的文件复制到该描述符后，返回这个描述符
    使ret和fd指向同一个文件
    
    dup可以多次使用，dup可以传入自己的返回值，dup出新的fd。
    多次使用dup, 每次传入没有dup过的fd，dup出新的fd。
    */
#if 1

	for(i = 0; i < NUM-1; i++)
	{
		fd[i+1] = dup(fd[i]);	//所有fd都指向同一个文件
		if(fd[i] < 0)
    	{
    		perror("dup");
    		exit(1);
    	}
    	printf("dup ret fd[%d] = %d\n", i+1, fd[i+1]);
	}
#else
	fd[1] = dup(fd[0]);
	printf("dup ret fd[1]:%d\n", fd[1]);
	fd[2] = dup(fd[1]);
	printf("dup ret fd[2]:%d\n", fd[2]);
	fd[3] = dup(fd[2]);
	printf("dup ret fd[3]:%d\n", fd[3]);
	fd[4] = dup(fd[3]);
	printf("dup ret fd[4]:%d\n", fd[4]);

#endif
    char *msg[5] = {
    				"你是猴子请来的救兵吗？？\n", 
    				"你大爷的，我是程序猿!!!\n",
    				"hello monkey\n",
    				"you are king\n",
    				"fire cat kill monkey king\n",
    				};

    for(i = 0; i < NUM; i++)
    {
		write(fd[i], msg[i], strlen(msg[i]));
		printf("send msg[%d]: %s\n", i, msg[i]);
	}

	//只需要close(fd[0])，第一个fd，即Open返回的最小值
    close(fd[0]);

    return 0;
}

/*
DEMO DESC:
dup2(fd, fd1) 让fd1和fd同时指向a.txt
*/
void dup2_demo(void)
{
    int fd = open("a.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

	int fd1 = open("b.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
	if(fd1 == -1)
	{
    	perror("open");
    	exit(1);
	}

	printf("fd = %d\n", fd);
	printf("fd1 = %d\n", fd1);

	int ret = dup2(fd, fd1);  //让fd1和fd同时指向a.txt
	if(ret == -1)
	{
    	perror("dup2");
    	exit(1);
	}
	printf("current fd = %d\n", ret);

	char* buf = "hello king \n";
	char* buf1 = "no world! just fire\n";

	write(fd, buf, strlen(buf));
	write(fd1, buf1 , strlen(buf1));

	close(fd);
	close(fd1);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("dup demo\n");
	dup_demo();

	printf("dup2 demo\n");
	dup2_demo();

	return 0;
}
/*
一、进程概述
	进程：  就是进行中的程序 
	程序：  存放指令的程序文件，存放在磁盘上，固定不变的，保存着指令的有序集合。 
	执行过程：将程序从硬盘导入到内存，内存上分为代码区、静态变量区、堆栈区等等 
	文本区：储存处理器执行的代码 
	数据区：存储变量和动态分配的内存：全局变量，局部变量，静态变量， 
	堆栈区：存储着活动进程调用的指令和本地变量，指针变量->栈区,动态分配的内存->堆区 
	进程为程序执行提供各种各样的条件，程序需要各种各样操作，磁盘操作、网络操作等等。需要各种管理，
	这种动态的管理行为就可以通俗的称之为进程。所谓程序开始活动起来了。进程是linux系统调度的最小单位

二、进程状态
	进程是程序的执行过程，根据他的生命周期可以分为3种状态。 
	执行态：该进程正在运行，即进程正在使用CPU. 
	就绪态：进程已经具备执行的一切条件，正在等待分配CPU的处理. 
	等待态：进程不能使用CPU，若等待事件发生（等待的资源分配到），将其唤醒.

三、ps
	process：是os的最小单元，地址空间为4g，其中1g给os，另外3g给进程.
	ps：查看活动的进程 .
	ps -aux：查看各个进程的状态，包括运行、就绪、等待等状态(S睡眠 T中断 R运行 Z僵尸）.
	ps -aux | grep `aa`:查看指定（aa）进程.
	ps -ef：查看所有进程的pid，ppid等信息.
	ps -aux ：查看cpu内存的使用状态.

四、进程标识
	1. 进程不能用程序区分,因为不同进程可能执行的是同一个程序. 所以，使用ID号来标识区分不同的进程 
	2. OS会为每个进程分配一个唯一的整型ID，作为进程的标识号（pid）进程0时调度进程，常被称为交换进程，
	   他不执行任何程序，是内核的一部分，因此被称为系统进程。 
	3. 进程除了自身ID外，还有父进程ID，每个进程都会有一个父进程，操作系统不会无缘无故产生一个新进程，
	   所有的进程的祖先进程是同一个进程，叫做init进程，进程号是1.
	4. init进程是内核自举后的第一个启动进程。init进程负责引导系统、启动守护（后台）进程并且运行
	   必要的程序。他不是系统进程，但它以系统的超级用户特权运行。父进程是负责子进程空间的清理

五、 获取自己的进程ID
	每个进程都有一个整型的ID号 PID
	每个进程都有一个父进程 PPID 
	一个进程想控制其他进程，想与其他进程通信，需要获取自己的ID
	pid_t getpid(void); 
	功能：获取自己的进程ID号 
	参数：无 
	返回值：本进程的ID号

六、pid_t getppid(void);
   功能：获取自己的父进程ID号 
   参数：无 
   返回值：本进程的父进程的ID号

七、 pid_t fork(void);
   功能：创建一个子进程 
   参数：无 
   返回值：有两次，子进程返回0，父进程返回子进程的进程号。 
   注意：子进程从fork函数之后执行，fork完全拷贝了父进程的地址空间给子进程，父子进程运行顺序不确定。

八、pid_t vfork(void);
   功能：创建一个子进程 
   参数：无 
   返回值：有两次，子进程返回0，父进程返回子进程的进程号。 
   注意：子进程从fork函数之后执行，子进程有自己的ID与父进程共享资源，子进程先执行，父进程后执行。
   价值并不大，不能实现任务并发执行.

九、错误打印函数
    void perror(const char *s);
	功能：打印错误信息（某些函数返回负值，表明发生错误，但是不知道具体类型),使用这个函数可以获得具体
	错误类型.

十、销毁进程
	进程的常见的终止方式有5种： 
主动：
	1. main函数的自然返回，注意：return不是结束，只是函数结束，当它刚好结束的是main函数，此时导致
	进程结束。造成return结束进程的错觉。
	2. 调用exit函数.
	3. 标准函数调用_exit函数.
	4. 系统调用abort函数，产生SIGABRT信号.

被动：
	5.接收到某个信号，如ctrl+c，SIGINT，ctrl+\ SIGOUT
    6.通过kill 向进程发信号 

	前四四种正常的终止，后两种非正常的终止，但无论哪种方式，进程终止都会执行相同的关闭打来的文件，
	释放占用的内存资源，后两种终止会导致程序有些代码不能正常执行，比如对象的析构、atexit函数的执行。

PS: exit和__exit区别:
	exit函数退出之前会检查文件的打开情况，把文件缓冲区的内容写回文件，而__exit直接退出，
	比如打开文件向文件写入内容，如果在文件没有关闭，也没有调用同步到磁盘的函数，文件并没有同步到磁盘，
	只存在缓冲区内，这时调用exit，那么进程结束时，缓冲区的内容可以同步到文件中，内容已经存在在文件之中了，
	调用__exit进程直接结束，文件不会有写入的内容。

十一、启动新进程
	int system(const char *command);
	功能：打开命令或者程序 
	参数：带路径的程序启动文件，或者在启动变量里声明的程序直接写程序名 
	返回值：-1失败 
	打开的程序是另一个进程，也可以成为此程序的子进程，因此子进程不一定和父进程视同一个程序，
	在成功打开所要执行的文件之后，父进程才能继续执行。 

	ex: 父进程程序
	int main(void)
	{
  		printf("调用子进程\n");
    	system("./son");
    	printf("调用子进程结束\n");
    	return 0;
	}

十二、进程替换(exec函数族)
重点学习四种：
	int execl(const char *path, const char *arg, ...);
	int execlp(const char *file, const char *arg, ...);
	int execv(const char *path, char *const argv[]);
	int execvp(const char *file, char *const argv[]);

作用：运行第一个exec函数族指定的可执行程序。
形式参数：
	path：包括执行文件名的全路径名，比如"/bin/ls"。
	file：既可是全路径名也可是可执行文件名，比如"ls"。
	arg：是可执行文件的全部命令行参数，可以用多个，包含命令名本身。注意最后一个参数必须为NULL。
	argv：是一个字符串数组。是可执行文件的全部命令行参数数组。
	比如，char *argv[]={“full path”,”param1”,”param2”,…NULL};

返回值：没有作用，因为程序跑去执行组函数了，没有办法处理返回值。

	这四个函数第一个参数都是可执行程序或者脚本的程序名，execl、execv需要带有完整的路径，第二参数
	为任意字符，起到占位作用，第三个或者后面的字符为调用者的参数，参数列表最后以NULL结尾，
	而execlp、execvp不需要只需要带有可执行程序的名称即可，系统自动去环境变量去寻找同名文件，
	execl、execlp需要NULL结尾.

函数后缀说明： 
	l/v：参数呈现形式 
	l: list 参数一个个的列出来 
	v：vector 参数用数组存储起来 
	p：目标程序，可以省略路径 
	e: 环境变量，不考虑

十三、进程同步
	#include <sys/wait.h>
等待子进程退出并且执行清理工作
pid_t wait(int *status);  //随机的等待一个子进程退出
pid_t waitpid(pid_t pid, int *status, int options);//等待指定的pid子进程退出；

形式参数：
pid：子进程id号；
status：传出参数，它会将子进程的结束状态值保存到status指向的变量里;
options：用于改变 waitpid 的行为。其中最常用的是WNOHANG，它表示无论子进程是否退出都将立即返回，
		 不会将调用者的执行挂起。
返回值：对应的子进程的id号；

pid_t wait(int *status);
	1. 函数返回值为结束子进程的进程号，当前进程中没有子进程则返回-1，参数为子进程结束状态指针.
	2. 如果单纯想等待子进程的结束而不关心进程结束状态，参数写入NULL即可；
	3. 若想获得子进程结束状态，将参数地址写入即可，例如定义int status存储子进程的结束状态，函数
	调用wait(&status)即可; 
	4. 一旦函数调用wait()，就会立即阻塞自己并自动判断当前进程中是否有某个子进程退出，wait()返回
	该子进程的状态，终止子进程的ID，所以它总能了解是哪一个子进程终止了。 

pid_t waitpid(pid_t pid, int *status, int options); 
	1. pid：从参数的名字pid和类型pid_t中就可以看出，这里需要的是一个进程ID。但当pid取不同的值时，
	   在这里有不同的意义。 
	2. pid> 0时，只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的
	   子进程还没有结束，waitpid就会一直等下去。 
	3. pid==-1时，等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用一模一样。 
	4. pid==0时，等待同一个进程组中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会
	   对它做任何理睬。 
	5. pid< -1时，等待一个指定进程组中的任何子进程，这个进程组的ID等于pid的绝对值。 
    
    6. 第二个参数与wait相同，存储制定子进程终止的状态信息。为整形指针类型。 
	
	7.options：options提供了一些额外的选项来控制waitpid，目前在Linux中只支持WNOHANG和WUNTRACED两个选项，
	  这是两个常数，可以用”|”运算符把它们连接起来使用。 
	  WNOHANG：若pid指定进程并不是立即可用（终止状态），waitpid不阻塞，返回值为0. 
	  WUNTRACED：若实现支持作业控制··· ···涉及到一些跟踪调试方面的知识，加之极少用到.

	返回值和错误 
	waitpid的返回值比wait稍微复杂一些，一共有3种情况：
	1. 当正常返回的时候，waitpid返回收集到的子进程的进程ID；
	2. 如果设置了选项WNOHANG，而调用中waitpid发现没有已退出的子进程可收集，则返回0；
	3. 如果调用中出错，则返回-1，这时errno会被设置成相应的值以指示错误所在；当pid所指示的子进程
	   不存在，或此进程存在，但不是调用进程的子进程，waitpid就会出错返回，这时errno被设置为ECHILD；

*/

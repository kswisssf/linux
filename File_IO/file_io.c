/*
【1】终端
        1、标准输入        stdin     ：scanf
        2、标准输出        stdout    ：printf
        3、标准出错        stderr    ：输入错误命
                         FILE*(流)
 【5】int main(int argc, const char *argv[])
        {
             return 0;
        }
      
      argc：参数个数
      argv： argv[0]   表示第一个参数：./a.out
             argv[1]   表示第二个参数
             argv[n]   表示第n个参数 
【2】缓存区
     （1）行缓冲：对于终端操作采用的缓存区 
                缓存区大小： 1024字节（1K）
                刷新缓存：程序正常结束、缓存区满、’\n’ 、使用fflush函数
     
     （2）全缓冲：对于文件操作采用的缓存区
                缓存区大小：4096字节（4K）
                刷新缓存：程序正常结束、缓存区满、使用fflush函数
     
     （3）无缓冲：对于终端操作采用的缓存区
                标准出错(stderr)
  
      好处:      
        1、提高数据的读写效率
        2、减少对磁盘的读写次数，提高磁盘的使用寿命
        

                
*/

/*
fopen /open区别:
UNIX环境下的C 对二进制流文件的读写有两套班子：
  1) fopen,fread,fwrite ; 
  2) open, read, write
区别:
  1. fopen 系列是标准的C库函数；open系列是 POSIX 定义的，是UNIX系统里的system call。
     也就是说，fopen系列更具有可移植性；而open系列只能用在 POSIX 的操作系统上。
  2. 使用fopen 系列函数时要定义一个指代文件的对象，被称为“文件句柄”（file handler），是一个结构体；
     而open系列使用的是一个被称为“文件描述符” （file descriptor）的int型整数。
  3. fopen 系列是级别较高的I/O，读写时使用缓冲；而open系列相对低层，更接近操作系统，读写时没有缓冲。
     由于能更多地与操作系统打交道，open系列可以访问更改一些fopen系列无法访问的信息，如查看文件的读写权限。
     这些额外的功能通常因系统而异。
  4. 使用fopen系列函数需要"#include <sdtio.h>"；使用open系列函数需要"#include <fcntl.h>" ，
     链接时要之用libc（-lc）
  5. fopen是标准c里定义的,open是POSIX中定义的. 
  6. fopen不能指定要创建文件的权限.open可以指定权限. 
  7. fopen返回指针,open返回文件描述符(整数). 

小结：
  总的来说，为了使程序获得更好的可移植性，未到非得使用一些fopen系列无法实现的功能的情况下，fopen系列是首选。


read/write和fread/fwrite区别:
  1. fread是带缓冲的,read不带缓冲. 
  2. fread可以读一个结构.read在linux/unix中读二进制与普通文件没有区别. 
  3. linux/unix中任何设备都是文件,都可以用open,read.
  4. 如果文件的大小是8k。你如果用read/write，且只分配了2k的缓存，则要将此文件读出需要
     做4次系统调用来实际从磁盘上读出。如果你用fread/fwrite，则系统自动分配缓存，则读出此文件
     只要一次系统调用从磁盘上读出。也就是用read/write要读4次磁盘，而用fread/fwrite则
     只要读1次磁盘。效率比read/write要高4倍。
  5. 如果程序对内存有限制，则用read/write比较好。都用fread 和fwrite,它自动分配缓存,速度会很快,
     比自己来做要简单。如果要处理一些特殊的描述符,用read 和write,如套接口,管道之类的.
  6. 系统调用write的效率取决于你buf的大小和你要写入的总数量，如果buf太小，你进入内核空间的次数大增，
     效率就低下。
  7. fwrite会替你做缓存，减少了实际出现的系统调用，所以效率比较高。
  8. 如果只调用一次(可能吗?)，这俩差不多，严格来说write要快一点点(因为实际上fwrite最后还是
     用了write做真正的写入文件系统工作)，但是这其中的差别无所谓。

*/

/*
基于文件描述符的输入输出函数：
   open： 打开一个文件，并指定访问该文件的方式，调用成功后返回一个文件描述符。
   creat：打开一个文件，如果该文件不存在，则创建它，调用成功后返回一个文件描述符。
   close：关闭文件，进程对文件所加的锁全都被释放。
   read：从文件描述符对应的文件中读取数据，调用成功后返回读出的字节数。
   write：向文件描述符对应的文件中写入数据，调用成功后返回写入的字节数。
   ftruncate：把文件描述符对应的文件缩短到指定的长度，调用成功后返回0。
   lseek：在文件描述符对应的文件里把文件指针设定到指定的位置，调用成功后返回新指针的位置。
   fsync:将所有已写入文件中的数据真正写到磁盘或其他下层设备上，调用成功后返回0。
   fstat：返回文件描述符对应的文件的相关信息，把结果保存在struct stat中，调用成功后返回0。
   fchown：改变与打开文件相关联的所有者和所有组，调用成功后返回0。
   fchmod：把文件描述符对应的文件的权限位改为指定的八进制模式，调用成功后返回0。
   flock：用于向文件描述符对应的文件施加建议性锁，调用成功后返回0。
   fcntl：既能施加建议性锁也能施加强制性锁，能建立记录锁、读取锁和写入锁，调用成功后返回0。
   dup： 复制文件描述符，返回没使用的文件描述符中最小的编号。
   dup2：由用户指定返回的文件描述符的值，用来重新打开或重定向一个文件描述符。
   select：同时从多个文件描述符读取数据或向多个文件描述符写入数据。

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h> 
#include <unistd.h> 

#define NUM   5

typedef struct student{
  long sno;
  char name[100];
  float score[3];
}STU;

FILE *fp;

/*修改覆盖学生数据*/
void modify_func(char *filename, STU stu)
{
/*
函数：FILE *fopen(const char *path, const char *mode);
        功能：按照指定的权限，打开指定的文件
        参数：path 指定文件的路径名及文件名
             mode: (注意:: b代表是binary二进制文件, t代表是文xt文本文件)
                r   以只读方式打开文件,该文件必须存在。
                r+  以可读可写方式打开文件,该文件必须存在。
                rb+ 读写打开一个二进制文件，只允许读写数据。
                rt+ 读写打开一个文本文件，允许读和写。

                w   打开只写文件，若文件存在则先清空为0。若文件不存在则建立该文件。
                w+  打开可读写文件，若文件存在则先清空为0。若文件不存在则建立该文件。
                a   以附加的方式打开只写文件。若文件不存在，则建立该文件，如果文件存在，
                    写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
                a+  以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，
                    写入的数据会被加到文件尾后，即文件原先的内容会被保留.（原来的EOF符不保留）
                wb  只写打开或新建一个二进制文件；只允许写数据。
                wb+ 读写打开或建立一个二进制文件，允许读和写。
                wt+ 读写打开或着建立一个文本文件；允许读写。
                
                at+ 读写打开一个文本文件，允许读或在文本末追加数据。
                ab+ 读写打开一个二进制文件，允许读或在文件末追加数据。
*/
  fp = fopen(filename, "rt+");

/*
函数：int fseek(FILE *stream, long offset, int whence);
         参数stream为文件指针
         参数offset为偏移量，以字节为单位,正数表示正向偏移，负数表示负向偏移
         参数whence设定从文件的哪里开始偏移,
              SEEK_SET： 文件开头
              SEEK_CUR： 当前位置
              SEEK_END： 文件结尾
              其中SEEK_SET,SEEK_CUR和SEEK_END依次为0，1和2.

ps:-1L的类型是long int，1U的类型是unsigned int。
*/
  fseek(fp, -1L*sizeof(STU), SEEK_END); 

/*
size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
  返回值：返回实际写入的数据块数目
  （1）buffer：是一个指针，对fwrite来说，是要获取数据的地址；
  （2）size：要写入内容的单字节数；
  （3）count:要进行写入size字节的数据项的个数；
  （4）stream:目标文件指针；
  （5）返回实际写入的数据项个数count。

说明：写入到文件的哪里？ 这个与文件的打开模式有关，如果是w+，则是从file pointer指向的地址开始写，
     替换掉之后的内容，文件的长度可以不变，stream的位置移动count个数；如果是a+，则从文件的末尾
     开始添加，文件长度加大。
fseek对此函数有作用，但是fwrite函数写到用户空间缓冲区，并未同步到文件中，所以修改后要将内存与文件同步可以用fflush（FILE *fp）函数同步。

*/
  fwrite(&stu, sizeof(STU), 1, fp);

/*
函数：int fclose(FILE *fp);
    功能：关闭一个文件
    参数：fp 指定要关闭的文件
    返回：成功返回0，失败返回EOF
*/
  fclose(fp);

}



/*
select()函数以及FD_ZERO、FD_SET、FD_CLR、FD_ISSET

select函数用于在非阻塞中，当一个套接字或一组套接字有信号时通知你，系统提供select函数来实现
多路复用输入/输出模型，原型：
#include <sys/time.h> 
#include <unistd.h> 
int select(int maxfd,fd_set *rdset,fd_set *wrset,fd_set *exset,struct timeval *timeout);

参数maxfd是需要监视的最大的文件描述符值+1；
rdset,wrset,exset分别对应于需要检测的可读文件描述符的集合，可写文件描述符的集 合及异常文件描述符的集合。

struct timeval结构用于描述一段时间长度，如果在这个时间内，需要监视的描述符没有事件发生则函数返回，返回值为0。
fd_set（它比较重要所以先介绍一下）是一组文件描述字(fd)的集合，它用一位来表示一个fd（下面会仔细介绍），

对于fd_set类型通过下面四个宏来操作：
    FD_ZERO(fd_set *fdset):将指定的文件描述符集清空，在对文件描述符集合进行设置前，必须对其进行初始化，
                           如果不清空，由于在系统分配内存空间后，通常并不作清空处理，所以结果是不可知的。
    FD_SET(fd_set *fdset):用于在文件描述符集合中增加一个新的文件描述符。 
    FD_CLR(fd_set *fdset):用于在文件描述符集合中删除一个文件描述符。 
    FD_ISSET(int fd,fd_set *fdset):用于测试指定的文件描述符是否在该集合中。


过去，一个fd_set通常只能包含<32的fd（文件描述字），因为fd_set其实只用了一个32位矢量来表示fd；
现在,UNIX系统通常会在头文件<sys/select.h>中定义常量FD_SETSIZE，它是数据类型fd_set的描述字数量，
其值通常是1024，这样就能表示<1024的fd。

根据fd_set的位矢量实现，我们可以重新理解操作fd_set的四个宏：
    fd_set set;
    FD_ZERO(&set);     
    FD_SET(0, &set);   
    FD_CLR(4, &set);     
    FD_ISSET(5, &set);   
―――――――――――――――――――――――――――――――――――――――
注意fd的最大值必须<FD_SETSIZE。
―――――――――――――――――――――――――――――――――――――――

2、select函数的接口比较简单：
    int select(int nfds, fd_set *readset, fd_set *writeset,fd_set* exceptset, struct tim *timeout);
功能：
    测试指定的fd可读？可写？有异常条件待处理？     
参数：
    nfds    
      需要检查的文件描述字个数（即检查到fd_set的第几位），数值应该比三组fd_set中所含的最大fd值更大，
      一般设为三组fd_set中所含的最大fd值加1（如在readset,writeset,exceptset中所含最大的fd为5，
      则nfds=6，因为fd是从0开始的）。设这个值是为提高效率，使函数不必检查fd_set的所有1024位。
    readset   
      用来检查可读性的一组文件描述字。
    writeset
      用来检查可写性的一组文件描述字。
    exceptset
      用来检查是否有异常条件出现的文件描述字。(注：错误不包括在异常条件之内)
    timeout
      用于描述一段时间长度，如果在这个时间内，需要监视的描述符没有事件发生则函数返回，返回值为0。 
    有三种可能：
      1.timeout=NULL（阻塞：select将一直被阻塞，直到某个文件描述符上发生了事件）
      2.timeout所指向的结构设为非零时间（等待固定时间：如果在指定的时间段里有事件发生或者时间耗尽，函数均返回）
      3.timeout所指向的结构，时间设为0（非阻塞：仅检测描述符集合的状态，然后立即返回，并不等待外部事件的发生）

返回值：     
    返回对应位仍然为1的fd的总数。

Remarks：
    三组fd_set均将某些fd位置0，只有那些可读，可写以及有异常条件待处理的fd位仍然为1。

>>>举个例子，比如recv(),在没有数据到来调用它的时候,你的线程将被阻塞,如果数据一直不来,你的线程就要
阻塞很久.这样显然不好. 所以采用select来查看套节字是否可读(也就是是否有数据读了)  
步骤如下:
socket   s;   
.....   
fd_set   set;   
while(1)   
{       
      FD_ZERO(&set);    //将你的套节字集合清空   
      FD_SET(s,   &set);//加入你感兴趣的套节字到集合,这里是一个读数据的套节字s   
      select(0,&set,NULL,NULL,NULL);//检查套节字是否可读,   
                                    //很多情况下就是是否有数据(注意,只是说很多情况)  
                                    //这里select是否出错没有写   
      if(FD_ISSET(s, &set)   //检查s是否在这个集合里面,   
      {                      //select将更新这个集合,把其中不可读的套节字去掉   
                             //只保留符合条件的套节字在这个集合里面                         
          recv(s,...);   
      }   
      //do something here   
}

理解select模型的关键在于理解fd_set,为说明方便，取fd_set长度为1字节，fd_set中的每一bit可以对应
一个文件描述符fd。则1字节长的fd_set最大可以对应8个fd。
    （1）执行fd_set set; FD_ZERO(&set);则set用位表示是0000,0000。
    （2）若fd＝5,执行FD_SET(fd,&set);后set变为0001,0000(第5位置为1)
    （3）若再加入fd＝2，fd=1,则set变为0001,0011
    （4）执行select(6,&set,0,0,0)阻塞等待
    （5）若fd=1,fd=2上都发生可读事件，则select返回，此时set变为0000,0011。
注意：没有事件发生的fd=5被清空。

*/

/*

 使用select函数的过程一般是：
    先调用宏FD_ZERO将指定的fd_set清零，然后调用宏FD_SET将需要测试的fd加入fd_set，接着调用函数select测试fd_set中的所有fd，最后用宏FD_ISSET检查某个fd在函数select调用后，相应位是否仍然为1。
    以下是一个测试单个文件描述字可读性的例子：

*/

#include <sys/time.h>
#include <time.h>

typedef enum BOOL{
  TRUE = 1,
  FALSE = 0,
}bool;

struct tim {
    time_t      tv_sec;    
    suseconds_t tv_usec;   
};

int isready(int fd)
{
  int rc;
  fd_set fds;
  struct tim tv;    
  FD_ZERO(&fds);
  FD_SET(fd,&fds);

  tv.tv_sec = tv.tv_usec = 0;    
   
  rc = select(fd+1, &fds, NULL, NULL, &tv);
  if (rc < 0)   //error
    return -1;    
  
  return FD_ISSET(fd,&fds) ? 1 : 0;
}

/*
下面还有一个复杂一些的应用：
这段代码将指定测试Socket的描述字的可读可写性，因为Socket使用的也是fd
*/

unsigned int SocketWait(int socketfd,bool rd,bool wr,unsigned int timems)    
{
     fd_set rfds,wfds;
     struct tim tv;
     
     FD_ZERO(&rfds);
     FD_ZERO(&wfds); 
     if (rd)     //TRUE
        FD_SET(socketfd, &rfds);   //添加要测试的描述字 
     if (wr)     //FALSE
        FD_SET(socketfd, &wfds); 
     tv.tv_sec  = timems/1000;     //second
     tv.tv_usec = timems%1000;     //ms 
     for (;;) //如果errno==EINTR，反复测试缓冲区的可读性
          switch(select(socketfd+1, &rfds, &wfds, NULL,
                  (timems==0?NULL:&tv))) //测试在规定的时间内套接口接收缓冲区中是否有数据可读
          {                                              //0－－超时，-1－－出错
          case 0:    
              return 0;
          break;

          case (-1):            
              return 0; //有错但不是EINTR 
          break;

          default:
              if (FD_ISSET(socketfd, &rfds)) //如果s是fds中的一员返回非0，否则返回0
                  return 1;
              if (FD_ISSET(socketfd, &wfds))
                  return 2;
              return 0;
          break;
          };
}


#define FILENAME    "student"

int main(int argc, char *argv[])
{
  int i, j;

  STU stt[NUM] = {
    {10001, "Mayun", 90,91,92},
    {10002, "Zhifubao", 80,81,82},
    {10003, "Taobao", 70,71,72},
    {10004, "Alli", 60,61,62},
    {10005, "Tianmao", 50,51,52},
  };

  STU boy = {10006, "Mahuateng", 40,41,42};
  STU ss[NUM];

  fp = fopen(FILENAME, "wt+");
  fwrite(stt, sizeof(STU), NUM, fp);
  fclose(fp);

/*
函数：
int fprintf(FILE *stream, const char *format, ...);
        头文：#include <stdio.h>
        功能：向指定文件写入数据
        参数：stream 指定写入的文件
             format 书写格式与printf完全一样
        返回：成功返回写入的字节数，失败返回负数

fprintf()函数根据指定的format(格式)发送信息(参数)到由stream(流)指定的文件.
  因此fprintf()可以使得信息输出到指定的文件。
    char name[20] = "Mary";
    FILE *out;
    out = fopen( "output.txt", "w" );//若不存在该文件，则将在应用程序目录下新建一个文件
    if( out != NULL )
    fprintf( out, "Hello %s\n", name );
对于其输出格式参数,和printf()一样.

函数：
int fflush(FILE *stream);
    头文：#include <stdio.h>
    功能：将写入缓存区的数据刷新到指定文件内
    参数：stream 指定要刷新的文件
    返回：成功返回0，失败返回EOF(-1)
        
    fflush()会强迫将缓冲区内的数据写回参数stream指定的文件中。
    如果参数stream为NULL， fflush()会将所有打开的文件数据更新。
用途：
    在使用多个输出函数连续进行多次输出时，有可能发现输出错误。因为下一个数据在上一个数据
    还没输出完毕，还在输出缓冲区中时，下一个printf就把另一个数据加入输出缓冲区，结果冲掉
    了原来的数据，出现输出错误。 在 printf()后加上fflush(stdout), 强制马上输出.

fflush(stdin)刷新标准输入缓冲区，把输入缓冲区里的东西丢弃[非标准]
fflush(stdout)刷新标准输出缓冲区，把输出缓冲区里的东西打印到标准输出设备上
printf("。。。。。。。。。。。");后面加fflush(stdout),可提高打印效率.
*/
  //行缓冲：对于终端操作采用的缓存区 ,缓存区大小： 1024字节（1K）
  fprintf(stdout, "The original data:\n");
  
  //刷新缓存：使用fflush函数
  fflush(stdout);

  fp = fopen(FILENAME, "rt+");
  fread(ss, sizeof(STU), NUM, fp);
  fclose(fp);

  for(i = 0; i < NUM; i++)
  {
    printf("No: %ld  Name:%-8s  Scores:\n", ss[i].sno, ss[i].name);
      for(j = 0; j<3; j++)
        printf("%6.2f\n", ss[i].score[j]);
    printf("\n");
  }

  modify_func(FILENAME, boy);

  //行缓冲：对于终端操作采用的缓存区 ,缓存区大小： 1024字节（1K）
  printf("modify_func data:\n");
  
  //刷新缓存：使用fflush函数
  fflush(stdout);

  fp = fopen(FILENAME, "rt+");
  fread(ss, sizeof(STU), NUM, fp);
  fclose(fp);

  for(i = 0; i < NUM; i++)
  {
    printf("No: %ld  Name:%-8s  Scores:\n", ss[i].sno, ss[i].name);
      for(j = 0; j<3; j++)
        printf("%6.2f\n", ss[i].score[j]);
    printf("\n");
  }

  fp = fopen("testfile", "wt+");

/*
【3】函数：int fgetc(FILE *stream);
        头文：#include <stdio.h>
        功能：从指定文件中读取一个字符
        参数：stream 指定输入的文件
        返回：成功返回字符的ASCII码，读到文件末尾或读取失败返回EOF

【4】函数：int fputc(int c, FILE *stream);
        功能：向指定文件输出一个字符
        参数：c 要输出的字符
             stream 指定输出的文件
        返回：成功返回字符的ASCII码，失败返回EOF
*/
  char *msg1 = "hello fire cat, ";

  for(i = 0; i<strlen(msg1); i++)
  {
    fputc(*(msg1+i), fp);
    printf("%c", fgetc(fp));
  }
  printf("\n");

/*
【6】 函数：char *fgets(char *s, int size, FILE *stream);
        功能：从stream文件中期望读取size个字符，并存储到s中
        参数：stream：指定读取的文件
             size：指定读取的期望个数
             s：存储读到的数据
        返回：成功返回s，失败或读到文件末尾返回NULL                            
        注意：至多读取size-1个字符
             遇到‘\n’和EOF停止读取
             同时'\n'也会被读取
             读取结束后主动在读到的数据末尾加'\0'
                
【7】函数：int fputs(const char *s, FILE *stream);
        功能：向指定文件内输出字符串
        参数：stream 指定输出的文件
              s 要输出的字符串
        返回：成功返回非负数，失败返回EOF

*/
  char *msg2 = "dota2 game only game\n";
  //文件操作完成位置停在文件最后，如果不关闭stream，直接操作就读取的数据为空
  fputs(msg2, fp);
  
  int seek = strlen(msg1)+ strlen(msg2);
  char *g_msg = (char *)malloc(seek * sizeof(int)); 
  //上一次文件操作位置停在文件最后，如果不关闭stream，直接操作就读取的数据为空
  //重新设置读取的起始位置
  fseek(fp,0,SEEK_SET);
  fgets(g_msg, seek, fp);
  printf("len :%d g_msg :%s\n", seek, g_msg);

  fclose(fp);
  free(g_msg);

  return 0;

}

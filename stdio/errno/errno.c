#include <stdio.h>
#include <string.h>
#include <errno.h>

/*
定义函数:
	void perror ( const char * str );
	用来将上一个函数发生错误的原因输出到标准设备(stderr)。
	参数str 所指的字符串会先打印出，后面再加上错误原因字符串。

	此错误原因依照全局变量errno的值来决定要输出的字符串。
    在库函数中有个errno变量，每个errno值对应着以字符串表示的错误类型。
    当你调用"某些"函数出错时，该函数已经重新设置了errno的值。
    perror函数只是将你输入的一些信息和errno所对应的错误一起输出。

perror() 函数显示您传给它的字符串,后跟一个冒号.一个空格和当前errno 值的文本表示形式。
*/


//让我们来模拟一种错误情况，尝试打开一个不存在的文件
int perror_demo(void)
{
	FILE *fp;
	fp = fopen("root/noexxxxx","r+");
	if(NULL == fp)
	{
		perror("root/noexxxxx");
		return 1;
	}

	close(fp);

	return 0;
}


/*
函数名:
	strerror, _strerror, _wcserror, __wcserror
	通过标准错误的标号，获得错误的描述字符串 ，将单纯的错误标号转为字符串描述，方便用户查找错误。
	
	函数原型编辑
	char*strerror(int errnum);
	char*_strerror(const char*strErrMsg);
	wchar_t*_wcserror(int errnum);
	wchar_t*__wcserror(const wchar_t*strErrMsg)
参数：
	errnum：错误标号，通常用errno(标准错误号，定义在errno.h中)
	Error number.
	strErrMsg
	User-supplied message.
返回：
	指向错误信息的指针（即：错误的描述字符串）。

函数作用:
	获取系统错误信息或打印用户程序错误信息
*/

int strerror_demo(void)
{

	int fd;
	int errno;
	char *mesg;

	if((fd = open("/dev/dsp",'wr+')) < 0)
	{
		perror("/dev/dsp");
		printf("errno=%d\n",errno);
		mesg = strerror(errno);
		printf("Mesg:%s\n",mesg);
	}

	return 0;
}

/*
errno的一些错误定义编辑:
以下主要来自2.6.32的内核代码中的/usr/include/asm-generic/errno.h及errno-base.h，
输出错误原因定义归纳整理如下：
*/   
   #define EPERM 1 /* Operation not permitted */
　　#define ENOENT 2 /* No such file or directory */
　　#define ESRCH 3 /* No such process */
　　#define EINTR 4 /* Interrupted system call */
　　#define EIO 5 /* I/O error */
　　#define ENXIO 6 /* No such device or address */
　　#define E2BIG 7 /* Argument list too long */
　　#define ENOEXEC 8 /* Exec format error */
　　#define EBADF 9 /* Bad file number */
　　#define ECHILD 10 /* No child processes */
　　#define EAGAIN 11 /* Try again */
　　#define ENOMEM 12 /* Out of memory */
　　#define EACCES 13 /* Permission denied */
　　#define EFAULT 14 /* Bad address */
　　#define ENOTBLK 15 /* Block device required */
　　#define EBUSY 16 /* Device or resource busy */
　　#define EEXIST 17 /* File exists */
　　#define EXDEV 18 /* Cross-device link */
　　#define ENODEV 19 /* No such device */
　　#define ENOTDIR 20 /* Not a directory */
　　#define EISDIR 21 /* Is a directory */
　　#define EINVAL 22 /* Invalid argument */
　　#define ENFILE 23 /* File table overflow */
　　#define EMFILE 24 /* Too many open files */
　　#define ENOTTY 25 /* Not a typewriter */
　　#define ETXTBSY 26 /* Text file busy */
　　#define EFBIG 27 /* File too large */
　　#define ENOSPC 28 /* No space left on device */
　　#define ESPIPE 29 /* Illegal seek */
　　#define EROFS 30 /* Read-only file system */
　　#define EMLINK 31 /* Too many links */
　　#define EPIPE 32 /* Broken pipe */
　　#define EDOM 33 /* Math argument out of domain of func */
　　#define ERANGE 34 /* Math result not representable */
　　#define EDEADLK 35 /* Resource deadlock would occur */
　　#define ENAMETOOLONG 36 /* File name too long */
　　#define ENOLCK 37 /* No record locks available */
　　#define ENOSYS 38 /* Function not implemented */
　　#define ENOTEMPTY 39 /* Directory not empty */
　　#define ELOOP 40 /* Too many symbolic links encountered */
　　#define EWOULDBLOCK EAGAIN /* Operation would block */
　　#define ENOMSG 42 /* No message of desired type */
　　#define EIDRM 43 /* Identifier removed */
　　#define ECHRNG 44 /* Channel number out of range */
　　#define EL2NSYNC 45 /* Level 2 not synchronized */
　　#define EL3HLT 46 /* Level 3 halted */
　　#define EL3RST 47 /* Level 3 reset */
　　#define ELNRNG 48 /* Link number out of range */
　　#define EUNATCH 49 /* Protocol driver not attached */
　　#define ENOCSI 50 /* No CSI structure available */
　　#define EL2HLT 51 /* Level 2 halted */
　　#define EBADE 52 /* Invalid exchange */
　　#define EBADR 53 /* Invalid request descriptor */
　　#define EXFULL 54 /* Exchange full */
　　#define ENOANO 55 /* No anode */
　　#define EBADRQC 56 /* Invalid request code */
　　#define EBADSLT 57 /* Invalid slot */
　　#define EDEADLOCK EDEADLK
　　#define EBFONT 59 /* Bad font file format */
　　#define ENOSTR 60 /* Device not a stream */
　　#define ENODATA 61 /* No data available */
　　#define ETIME 62 /* Timer expired */
　　#define ENOSR 63 /* Out of streams resources */
　　#define ENONET 64 /* Machine is not on the network */
　　#define ENOPKG 65 /* Package not installed */
　　#define EREMOTE 66 /* Object is remote */
　　#define ENOLINK 67 /* Link has been severed */
　　#define EADV 68 /* Advertise error */
　　#define ESRMNT 69 /* Srmount error */
　　#define ECOMM 70 /* Communication error on send */
　　#define EPROTO 71 /* Protocol error */
　　#define EMULTIHOP 72 /* Multihop attempted */
　　#define EDOTDOT 73 /* RFS specific error */
　　#define EBADMSG 74 /* Not a data message */
　　#define EOVERFLOW 75 /* Value too large for defined data type */
　　#define ENOTUNIQ 76 /* Name not unique on network */
　　#define EBADFD 77 /* File descriptor in bad state */
　　#define EREMCHG 78 /* Remote address changed */
　　#define ELIBACC 79 /* Can not access a needed shared library */
　　#define ELIBBAD 80 /* Accessing a corrupted shared library */
　　#define ELIBSCN 81 /* .lib section in a.out corrupted */
　　#define ELIBMAX 82 /* Attempting to link in too many shared libraries */
　　#define ELIBEXEC 83 /* Cannot exec a shared library directly */
　　#define EILSEQ 84 /* Illegal byte sequence */
　　#define ERESTART 85 /* Interrupted system call should be restarted */
　　#define ESTRPIPE 86 /* Streams pipe error */
　　#define EUSERS 87 /* Too many users */
　　#define ENOTSOCK 88 /* Socket operation on non-socket */
　　#define EDESTADDRREQ 89 /* Destination address required */
　　#define EMSGSIZE 90 /* Message too long */
　　#define EPROTOTYPE 91 /* Protocol wrong type for socket */
　　#define ENOPROTOOPT 92 /* Protocol not available */
　　#define EPROTONOSUPPORT 93 /* Protocol not supported */
　　#define ESOCKTNOSUPPORT 94 /* Socket type not supported */
　　#define EOPNOTSUPP 95 /* Operation not supported on transport endpoint */
　　#define EPFNOSUPPORT 96 /* Protocol family not supported */
　　#define EAFNOSUPPORT 97 /* Address family not supported by protocol */
　　#define EADDRINUSE 98 /* Address already in use */
　　#define EADDRNOTAVAIL 99 /* Cannot assign requested address */
　　#define ENETDOWN 100 /* Network is down */
　　#define ENETUNREACH 101 /* Network is unreachable */
　　#define ENETRESET 102 /* Network dropped connection because of reset */
　　#define ECONNABORTED 103 /* Software caused connection abort */
　　#define ECONNRESET 104 /* Connection reset by peer */
　　#define ENOBUFS 105 /* No buffer space available */
　　#define EISCONN 106 /* Transport endpoint is already connected */
　　#define ENOTCONN 107 /* Transport endpoint is not connected */
　　#define ESHUTDOWN 108 /* Cannot send after transport endpoint shutdown */
　　#define ETOOMANYREFS 109 /* Too many references: cannot splice */
　　#define ETIMEDOUT 110 /* Connection timed out */
　　#define ECONNREFUSED 111 /* Connection refused */
　　#define EHOSTDOWN 112 /* Host is down */
　　#define EHOSTUNREACH 113 /* No route to host */
　　#define EALREADY 114 /* Operation already in progress */
　　#define EINPROGRESS 115 /* Operation now in progress */
　　#define ESTALE 116 /* Stale NFS file handle */
　　#define EUCLEAN 117 /* Structure needs cleaning */
　　#define ENOTNAM 118 /* Not a XENIX named type file */
　　#define ENAVAIL 119 /* No XENIX semaphores available */
　　#define EISNAM 120 /* Is a named type file */
   #define EREMOTEIO 121 /* Remote I/O error */
   #define EDQUOT 122 /* Quota exceeded */
   #define ENOMEDIUM 123 /* Nomedium found */
   #define EMEDIUMTYEP 124 /*Wrongmedium found */
   #define ECANCELED 125 /* Operation Canceled */
　　#define ENOKEY 126 /* Required key not available */
　　#define EKEYEXPIRED 127 /* Key has expired */
　　#define EKEYREVOKED 128 /* Key has been revoked */
　　#define EKEYREJECTED 129 /* Key was rejected by service */
　　#define EOWNERDEAD 130 /* Owner died */
　　#define ENOTRECOVERABLE 131 /* State not recoverable */
　　#define ERFKILL 132 /* Operation not possible due to RF-kill */
　　#define EHWPOISON 133 /* Memory page has hardware error */

/*
	EACCES
	 1. 欲执行的文件不具有用户可执行的权限。
	 2. 欲执行的文件所属的文件系统是以noexec 方式挂上。
	 3.欲执行的文件或script翻译器非一般文件。
	 
	 EPERM
	 1.进程处于被追踪模式，执行者并不具有root权限，欲执行的文件具有SUID 或SGID 位。
	 2.欲执行的文件所属的文件系统是以nosuid方式挂上，欲执行的文件具有SUID 或SGID 位元，但执行者并不具有root权限。
	 
	 E2BIG 参数数组过大
	 ENOEXEC 无法判断欲执行文件的执行文件格式，有可能是格式错误或无法在此平台执行。
	 EFAULT 参数filename所指的字符串地址超出可存取空间范围。
	 ENAMETOOLONG 参数filename所指的字符串太长。
	 ENOENT 参数filename字符串所指定的文件不存在。
	 ENOMEM 核心内存不足
	 ENOTDIR 参数filename字符串所包含的目录路径并非有效目录
	 EACCES 参数filename字符串所包含的目录路径无法存取，权限不足
	 ELOOP 过多的符号连接
	 ETXTBUSY 欲执行的文件已被其他进程打开而且正把数据写入该文件中
	 EIO I/O 存取错误
	 ENFILE 已达到系统所允许的打开文件总数。
	 EMFILE 已达到系统所允许单一进程所能打开的文件总数。
	 EINVAL 欲执行文件的ELF执行格式不只一个PT_INTERP节区
	 EISDIR ELF翻译器为一目录
	 ELIBBAD ELF翻译器有问题。

*/

int main(int argc, char *argv[])
{
	printf("perror demo\n");
	perror_demo();

	printf("strerror demo\n");
	strerror_demo();
	
	return 0;
}
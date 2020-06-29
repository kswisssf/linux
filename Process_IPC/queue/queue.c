#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
 
#include <signal.h>
#include <sys/stat.h>

#define SIZE    1024

struct msg_buf
{
	int mtype;
	char data[SIZE];
};

typedef struct  msg_info
{
	int qid;
	struct msg_buf *msgpbuf;
	int msgsz;
	int msgflag;
}MSG_INFO;


key_t msg_ftok(char *path, char proj_id)
{
	key_t keyval;

	if(NULL==path)
	{
		printf("no pathname\n");
		return -1;
	}
/*
系统建立IPC通讯(消息队列、信号量和共享内存)时必须指定一个ID值。通常情况下,该id值通过ftok函数得到。

函数原型：
key_t ftok( const char * fname, int id )
	fname就是你指定的文件名（已经存在的文件名），一般使用当前目录，如：
	key = ftok(".", 1); 这样就是将fname设为当前目录。
	id是子序号。虽然是int类型，但是只使用8bits(1-255）。
	在一般的UNIX实现中，是将文件的索引节点号取出，前面加上子序号得到key_t的返回值。

*/
	keyval = ftok(path, proj_id);

	return keyval;
}

/*
msgget()函数:
  key：消息队列关联的键。
       消息队列对象的关键字(key)，函数将它与已有的消息队列对象的关键字进行比较来判断消息队列对象是否已经创建。
  msgflg：消息队列的建立标志和存取权限。
        IPC_CREAT:如果消息队列对象不存在，则创建之，否则则进行打开操作;
　　     IPC_EXCL：和IPC_CREAT 一起使用（用”|”连接），如果消息对象不存在则创建之，否则产生一个错误并返回.
　　如果单独使用IPC_CREAT标志,msgget()函数要么返回一个已经存在的消息队列对象的标识符，要么返回一个新建立的消息队列对象的标识符。

返回说明：
	成功执行时，返回消息队列标识值。失败返回-1，errno被设为以下的某个值 ，有时也会返回0，这个时候也是可以正常使用的
	EACCES：指定的消息队列已存在，但调用进程没有权限访问它，而且不拥有CAP_IPC_OWNER权能
	EEXIST：key指定的消息队列已存在，而msgflg中同时指定IPC_CREAT和IPC_EXCL标志
	ENOENT：key指定的消息队列不存在同时msgflg中不指定IPC_CREAT标志
	ENOMEM：需要建立消息队列，但内存不足
	ENOSPC：需要建立消息队列，但已达到系统的最大消息队列容量
*/
int open_queue(key_t keyval)
{
	int qid;

	if((qid = msgget(keyval, IPC_CREAT|0666)) == -1)
	{
		return -1;
	}

	return qid;
}

/*
功能描述:
	msgrcv()可以从消息队列中读取消息，msgsnd()将一个新的消息写入队列。
	在消息队列上进行收发消息。为了发送消息，调用进程对消息队列进行写入时必须有写权能。接收消息时必须有读权能。
用法:
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
函数原型:
	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
msqid：消息队列的识别码。
msgp：指向消息缓冲区的指针，此位置用来暂时存储发送和接收的消息，是一个用户可定义的通用结构，形态如下

struct msgbuf {
	long mtype;  	//消息类型，必须>0 
	char mtext[1];  //消息文本 
};
msgsz：消息的大小。

msgtyp：消息类型
	msgtyp等于0,  则返回队列的最早的一个消息。
	msgtyp大于0,  则返回其类型为msgtyp的第一个消息。
	msgtyp小于0,  则返回其类型小于或等于mtype参数的绝对值的最小的一个消息。

参数 msgflg：可选项。
	如果为 0 表示没有消息就阻塞。
	IPC_NOWAIT：如果指定类型的消息不存在就立即返回，同时设置 errno 为 ENOMSG
	MSG_EXCEPT：仅用于 msgtyp > 0 的情况。表示获取类型不为 msgtyp 的消息
	MSG_NOERROR：如果消息数据正文内容大于 msgsz，就将消息数据截断为 msgsz


*/

/*
成功执行时，msgsnd()返回0，msgrcv()返回拷贝到mtext数组的实际字节数。
失败两者都返回-1，errno被设为以下的某个值.

[对于msgsnd]:
	EACCES：调用进程在消息队列上没有写权能，同时没有CAP_IPC_OWNER权能
	EAGAIN：由于消息队列的msg_qbytes的限制和msgflg中指定IPC_NOWAIT标志，消息不能被发送
	EFAULT：msgp指针指向的内存空间不可访问
	EIDRM：消息队列已被删除
	EINTR：等待消息队列空间可用时被信号中断
	EINVAL：参数无效
	ENOMEM：系统内存不足，无法将msgp指向的消息拷贝进来

[对于msgrcv]:
	E2BIG：消息文本长度大于msgsz，并且msgflg中没有指定MSG_NOERROR
	EACCES：调用进程没有读权能，同时没具有CAP_IPC_OWNER权能
	EAGAIN：消息队列为空，并且msgflg中没有指定IPC_NOWAIT
	EFAULT：msgp指向的空间不可访问
	EIDRM：当进程睡眠等待接收消息时，消息已被删除
	EINTR：当进程睡眠等待接收消息时，被信号中断
	EINVAL：参数无效
	ENOMSG：msgflg中指定了IPC_NOWAIT，同时所请求类型的消息不存在
*/
int send_message(int qid, struct msg_buf *send_buf)
{
	int ret, length;
	length = sizeof(struct msg_buf) - sizeof(int);
	ret = msgsnd(qid, send_buf, length, IPC_NOWAIT);
	if(ret == -1)
	{
		printf("send_message err\n");
		return -1;
	}
	
	return ret;	
};

int read_message(int qid, int type, struct msg_buf *read_buf) 
{
	int ret, length;
	length = sizeof(struct msg_buf) - sizeof(int);
	
	 if((ret=msgrcv(qid, read_buf, length, type, 0)) == -1)	
	 {
	 	printf("read read_message err\n");
		return -1;
	 }
	 
	return ret;
};


int main(int argc, char * argv[])
{

	key_t key;
	int msgid;
	int ret;
	char *pathname ="/home/duobao/linux_base/linux_app/IPC/queue";
	char proj = "a"; 
	struct msg_buf msgbuf;

	if(argc !=2 )
	{
		printf("usge:%s msg\n", argv[0]);
		return -1;
	}

	key = msg_ftok(pathname, proj);
	printf("key = [%d]\n", key);
	msgid = open_queue(key);
	if(msgid == -1)
	{
		printf("create error\n");
		return -1;
	}

	//getpid是一种函数，功能是取得进程识别码，许多程序利用取到的此值来建立临时文件，以避免临时文件相同带来的问题
	msgbuf.mtype = getpid();
	strcpy(msgbuf.data, argv[1]);
	ret = send_message(msgid, &msgbuf);
	if(ret == -1)
	{
		printf("send_message err\n");
		return -1;
	}

	memset(&msgbuf, 0, sizeof(msgbuf));

	ret = read_message(msgid, msgbuf.mtype, &msgbuf);
	if(ret == -1)
	{ 
		printf("read_message err\n");
		return -1;
	}

	printf("read_message :%s\n", &msgbuf.data);


	return 0;

}
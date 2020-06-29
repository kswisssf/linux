/*
1、消息队列简介
	消息队列本质上是位于内核空间的链表，链表的每个节点都是一条消息。每一条消息都有自己的消息类型，
	消息类型用整数来表示，而且必须大于 0。每种类型的消息都被对应的链表所维护：

其中数字 1 表示类型为 1 的消息，数字2、3、4 类似。彩色块表示消息数据，它们被挂在对应类型的链表上。
值得注意的是，刚刚说过没有消息类型为 0 的消息，实际上，消息类型为 0 的链表记录了所有消息加入队列的顺序，
其中红色箭头表示消息加入的顺序。

2、消息队列相关的函数
	创建和获取 ipc 内核对象
		int msgget(key_t key, int flags);

	将消息发送到消息队列
		int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

	从消息队列获取消息
		ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
	
	查看、设置、删除 ipc 内核对象（用法和 shmctl 一样）
		int msgctl(int msqid, int cmd, struct msqid_ds *buf);

3、消息数据格式
	无论你是发送还是接收消息，消息的格式都必须按照规范来。简单的说，它一般长成下面这个样子：	
	struct Msg{
    long type; // 消息类型。这个是必须的，而且值必须 > 0，这个值被系统使用
    // 消息正文，多少字节随你而定
    // ...
	};	

	所以，只要你保证首4字节(32 位 linux 下的 long)是一个整数就行了。
	举个例子：
	struct Msg {
	    long type;
	    char name[20];
	    int age;
	} msg;

	struct Msg {
	    long type;
	    int start;
	    int end;
	} msg;
	从上面可以看出，正文部分是什么数据类型都没关系，因为消息队列传递的是 2 进制数据，不一定非得是文本。


4、msgsnd 函数
	msgsnd 函数用于将数据发送到消息队列。如果该函数被信号打断，会设置 errno 为 EINTR。
	int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
	参数 msqid：ipc 内核对象 id
	参数 msgp：消息数据地址
	参数 msgsz：消息正文部分的大小（不包含消息类型）
	参数 msgflg：可选项
	该值为 0：如果消息队列空间不够，msgsnd 会阻塞。
	IPC_NOWAIT：直接返回，如果空间不够，会设置 errno 为 EAGIN.

	返回值：0 表示成功，-1 失败并设置 errno。

5、msgrcv 函数
	msgrcv 函数从消息队列取出消息后，并将其从消息队列里删除。
	ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
	参数 msqid：ipc 内核对象 id
	参数 msgp：用来接收消息数据地址
	参数 msgsz：消息正文部分的大小（不包含消息类型）
	参数 msgtyp：指定获取哪种类型的消息

	msgtyp = 0：获取消息队列中的第一条消息
	msgtyp > 0：获取类型为 msgtyp 的第一条消息，除非指定了 msgflg 为MSG_EXCEPT，这表示获取除了 msgtyp 类型以外的第一条消息。
	msgtyp < 0：获取类型 ≤|msgtyp|≤|msgtyp| 的第一条消息。
	参数 msgflg：可选项。
	如果为 0 表示没有消息就阻塞。
	IPC_NOWAIT：如果指定类型的消息不存在就立即返回，同时设置 errno 为 ENOMSG
	MSG_EXCEPT：仅用于 msgtyp > 0 的情况。表示获取类型不为 msgtyp 的消息
	MSG_NOERROR：如果消息数据正文内容大于 msgsz，就将消息数据截断为 msgsz

*/

/*
6.msgctl函数
	获取和设置消息队列的属性
	int msgctl(int msqid, int cmd, struct msqid_ds *buf);
	msqid：消息队列标识符
	cmd：控制指令
		IPC_STAT
		读取消息队列的数据结构msqid_ds，并将其存储在b u f指定的地址中。
		IPC_SET
		设置消息队列的数据结构msqid_ds中的ipc_perm元素的值。这个值取自buf参数。
		包括：msg_perm.uid、msg_perm.gid、msg_perm.mode以及msg_qbytes
		IPC_RMID
		从系统内核中移走消息队列。
----------------------------------------------------------------------------
Ps: System V IPC为每个IPC对象设置了一个ipc_perm结构体并在创建IPC对象的时候进行初始化。
	这个结构体中定义了IPC对象的访问权限和所有者:
    struct ipc_perm{
       uid_t uid;   //所有者的用户id
       gid_t gid;   //所有者的组id
       uid_t cuid;  //创建者的用户id
       gid_t cgid;  //创建者的组id
       mode_t mode; //访问模式
       …
    };
----------------------------------------------------------------------------
	buf：消息队列管理结构体。

	返回值：
		成功：0
		出错：-1，错误原因存于error中
	EACCESS：参数cmd为IPC_STAT，确无权限读取该消息队列
	EFAULT：参数buf指向无效的内存地址
	EIDRM：标识符为msqid的消息队列已被删除
	EINVAL：无效的参数cmd或msqid
	EPERM：参数cmd为IPC_SET或IPC_RMID，却无足够的权限执行
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <error.h>

struct msgbuf{
    long mtype ;
    char mtext[] ;
}  ;

int main(int argc, char **argv){
    int msqid ;
    struct msqid_ds info;
    struct msgbuf buf;
    struct msgbuf buf1;
    int flag;
    int sendlength, recvlength;

    msqid = msgget( IPC_PRIVATE, 0666 );

    if ( msqid < 0 ){
        perror("get ipc_id error");
        return -1 ;
    }

    buf.mtype = 1;
    strcpy(buf.mtext, "happy new year!");
    sendlength = sizeof(struct msgbuf) - sizeof(long) ;
    flag = msgsnd( msqid, &buf, sendlength , 0 );

    if ( flag < 0 ){
        perror("send message error");
        return -1;
    }

    buf.mtype = 3 ;
    strcpy(buf.mtext, "good bye!");
    sendlength = sizeof(struct msgbuf) - sizeof(long);
    flag = msgsnd( msqid, &buf, sendlength , 0 );

    if ( flag < 0 ){
        perror("send message error");
        return -1 ;
    }

    flag = msgctl(msqid, IPC_STAT, &info);

    if ( flag < 0 ){
        perror("get message status error");
        return -1 ;
    }

    printf("uid:%d, gid = %d, cuid = %d, cgid= %d\n",
           info.msg_perm.uid, info.msg_perm.gid, info.msg_perm.cuid, info.msg_perm.cgid);

    printf("read-write:%03o, cbytes = %lu, qnum = %lu, qbytes= %lu\n",
           info.msg_perm.mode&0777, info.msg_cbytes, info.msg_qnum, info.msg_qbytes);

    system("ipcs -q");
    recvlength = sizeof(struct msgbuf) - sizeof(long);
    memset(&buf1, 0x00, sizeof(struct msgbuf)) ;

    flag = msgrcv( msqid, &buf1, recvlength ,3,0 );
    if ( flag < 0 ){
        perror("recv message error");
        return -1 ;
    }
    printf("type=%d, message=%s\n", buf1.mtype, buf1.mtext);

    flag = msgctl( msqid, IPC_RMID,NULL);
    if ( flag < 0 ){
        perror("rm message queue error");
        return -1 ;
    }
    system("ipcs -q");

    return 0 ;
}



/*
信号量是一个计数器，常用于处理进程和线程的同步问题，特别是对临界资源访问的同步。
获取一次信号量的操作就是对信号量减一，而释放一次信号量的操作就是对信号量加一。
Linux内核为每个信号集提供了一个semid_ds数据结构.该结构定义如下(linux/sem.h):
// Obsolete, used only for backwards compatibility and libc5 compiles 
    struct semid_ds {
        struct ipc_perm    sem_perm;        // 对信号操作的许可权 
        __kernel_time_t    sem_otime;        //对信号操作的最后时间 
        __kernel_time_t    sem_ctime;        //对信号进行修改的最后时间 
        struct sem    *sem_base;        //指向第一个信号 
        struct sem_queue *sem_pending;        // 等待处理的挂起操作 
        struct sem_queue **sem_pending_last;    // 最后一个正在挂起的操作 
        struct sem_undo    *undo;            // 撤销的请求 
        unsigned short    sem_nsems;        // 数组中的信号数 
    };
(1)Linux下使用系统函数创建和打开信号集.这个函数定义在头文件sys/sem.h中，函数原型如下:
    int semget(key_t key, int nsems, int semflg);
    该函数执行成功则返回一个信号集的标识符，失败返回－1。函数第一个参数由ftok()得到键值。第二个参数nsems指明要创建的
    信号集包含的信号个数，如果只是打开信号集，把nsems设置为0即可;第三个参数semflg为操作标志，可以取如下值。
    IPC_CREAT:调用semget时，它会将此值与其他信号集的key进行比较，如果存在相同的Key,说明信号集已经存在，此时返回给
    信号集的标识符，否则新建一个信号集并返回其标识符。
    IPC_EXCL:该宏须和IPC_CREAT一起使用。使用IPC_CREAT|IPC_EXCL时，表示如果发现信号集已经存在，则返回错误，
    错误码是EEXIST。
(2)信号量的操作，信号量的值和相应资源的使用情况有关，当它的值大于0时，表示当前可用资源的数量，当它的值小于0时，其绝对值
表示等待使用该资源的进程个数.信号量的值仅能由PV操作来改变。在Linux下，PV操作通过调用函数semop实现。该函数定义在
文件/sys/sem.h，原型如下：

    int semop(int semid, struct sembuf *sops, size_t nsops);
    参数semid为信号集的标识符；参数sops指向进行操作的结构体数组首地址；参数nsops指出将要进行操作的信号的个数。
    semop函数调用成功返回0，否则返回－1.
    sops参数的定义如下：（linux/sem.h）

struct  sembuf {
        ushort  sem_num; //信号在信号集的索引
        short   sem_op;  //操作类型
        short   sem_flg; //操作标志
};

表 1     sem_op的取值和意义
取值范围   操作意义 
sem_op > 0  信号加上sem_op.表示进程释放控制的资源
sem_op = 0  如果没有设置IPC_NOWAIT，则进程进入睡眠，直到信号值为0；否则进程不会睡眠，直接返回EAGAIN
sem_op < 0 信号加上sem_op的值，若没有设置IPC_NOWAIT，则进程进入睡眠，直到资源可用。否则直接返回EAGAIN 

(3)信号量的控制，使用信号量时，往往需要对信号集进行一些控制操作，比如删除信号集、对内核维护的信号集的数据结构
semid_ds进行设置，获取信号集中信号值等。通过semctl可以操作：(sys/sem.h)
int semctl(int semid, int semnum, int cmd,...);

函数中，参数semid为信号集的标识符，参数semnum标识一个特定的信号；cmd指明控制操作的类型。最后的“...”说明函数
的参数是可选的，它依赖于第3个参数cmd,它通过共用体变量semun选择要操作的参数.semun定义在Linux/sem.h:
// arg for semctl system calls. 
union semun {
    int val;            // value for SETVAL 
    struct semid_ds *buf;    // buffer for IPC_STAT & IPC_SET 
    unsigned short *array;    // array for GETALL & SETALL 
    struct seminfo *__buf;    // buffer for IPC_INFO 
    void *__pad;
};

以上各个字段含义如下：
    1）val:仅用于SETVAL操作类型，设置某个信号的值等于val
    2）buf:用于IPC_STAT和IPC_SET，存取semid_ds结构
    3）array:用于SETALL和GETALL操作
    4）buf:为控制IPC_INFO提供的缓存
cmd的宏含义如下：
    IPC_SET:对信号量的属性进行设置
    IPC_RNID:删除semid指定的信号集
    GETPID:返回最后一个执行semop操作的进程ID
    GETVAL:返回信号集semnum指定信号的值。
    GETALL:返回信号集中所用信号的值.
    GETNCNT:返回正在等待资源的进程的数量.
    GETZCNT:返回正在等待完全空闲资源的进程的数量.
    SETVAL:设置信号集中semnum指定的信号的值
    SETALL:设置信号集中所用信号的值.

共享内存：
    共享内存就是分配一块能被其它进程访问的内存。每个共享内存段在内核中维护着一个内部结构shmid_ds:(linux/shm.h)
    // Obsolete, used only for backwards compatibility and libc5 compiles 
    struct shmid_ds {
        struct ipc_perm        shm_perm;    // 操作许可 
        int            shm_segsz;    // 共享内存大小，字节为单位 
        __kernel_time_t        shm_atime;    // 最后一个进程访问共享内存的时间 
        __kernel_time_t        shm_dtime;    // 最后一个进程离开共享内存的时间 
        __kernel_time_t        shm_ctime;    // 最后一次修改共享内存的时间 
        __kernel_ipc_pid_t    shm_cpid;    // 创建共享内存的进程ID 
        __kernel_ipc_pid_t    shm_lpid;    // 最后操作共享内存的进程ID 
        unsigned short        shm_nattch;    // 当前使用该贡献内存的进程数量 
        unsigned short         shm_unused;    // compatibility 
        void             *shm_unused2;    // ditto - used by DIPC 
        void            *shm_unused3;    // unused 
    };


(1)创建共享内存：(linux/shm.h)

int shmget(key_t key, size_t size, int shmglf);
参数key和shmflg可以参考semflg的参数。size是以字节为单位指定的内存的大小。

(2)共享内存的操作：（linux/shm.h）
    void *shmat (int shmid, const void *shmaddr, int shmflg);
    在使用共享内存前，必须通过shmat函数将其附加到进程的地址空间。shmat调用成功后会返回一个指向共享内存区域的指针，
    使用该指针就可以访问共享内存了。如果失败返回－1。
    shmat参数shmid是shmget的返回值。参数shmflg为存取权限标志;参数shmaddr为共享内存的附加点。参数shmaddr
    不同取值情况说明如下：
        1）如果为空，则由内核选取一个空闲的内存区;否则，返回地址取决于调用者是否给shmflg设置了SHM_RND值，
        如果没有指定，则共享内存区附加到由shmaddr指定的地址，否则附加地址为shmaddr向下舍入一个共享内存底端边界
        地址后的地址。
    当进程结束使用共享内存时，要通过函数断开与共享内存的链接。
    (sys/shm.h):
    int shmdt(const void *shmaddr);
参数shmaddr为shmat的返回值，该函数调用成功后，返回0，否则返回－1。

（3）共享内存的控制。(sys/shm.h):
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
参数shmid为共享内存区的标识符，即shmget函数的返回值.buf是指向shmid_ds结构体的指针；cmd为操作标志位，
支持以下3种操作：
    1)IPC_RMID : 从系统中删除由shmid指向的共享内存区
    2)IPC_SET:设置共享内存的shmid_ds结构
    3)IPC_STAT:读取共享内存区的shmid_ds机构，并将其存储到buf指向的地址。

这里是以简单的读者和写者为例，来学习信号量和共享内存。
整个程序的设计规定如下：
    1、首先让写者获取信号量，去写临界区，这是的临界区就是共享内存。完成后释放掉信号量。
    2、当读者获取信号量后，就去读临界区的数据，读出数据完成后，释放掉信号量。

以上读者和写者分别是两个进程，代码如下:
src/shm_write.c (写者代码):

*/

#include "shm_mem.h"

int main(int argc, char** argv)
{
    int semid, shmid;
    char *shmaddr;
    char write_str[SHM_SIZE];
    char *ret;

    //创建或者获取共享内存
    if((shmid = creatshm(".", 57, SHM_SIZE)) == -1) 
        return -1;

    /*建立进程和共享内存连接*/
    if((shmaddr = shmat(shmid, (char*)0, 0)) == (char *)-1){
        perror("attch shared memory error!\n");
        exit(1);
    }    
    //创建信号量
    if((semid = creatsem("./", 39, 1, 1)) == -1)
        return -1;

    while(1){
        wait_sem(semid, 0);//等待信号量可以被获取
        sem_p(semid, 0);   //获取信号量

/***************写共享内存***************************************************/
        printf("write : ");
        ret = fgets(write_str, 1024, stdin);
        if(write_str[0] == '#') // '#'结束读写进程
            break;
        int len = strlen(write_str);
        write_str[len] = '\0';
        strcpy(shmaddr, write_str);

/****************************************************************************/
        sem_v(semid, 0); //释放信号量
        usleep(1000);   //本进程睡眠.
    }

    sem_delete(semid); //把semid指定的信号集从系统中删除
    deleteshm(shmid);   //从系统中删除shmid标识的共享内存

    return 0;
}
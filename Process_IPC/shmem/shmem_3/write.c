#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*
用于Linux进程通信(IPC)共享内存。共享内存函数由shmget、shmat、shmdt、shmctl四个函数组成。

1.shmget函数原型
    shmget(得到一个共享内存标识符或创建一个共享内存对象)
所需头文件
    #include <sys/ipc.h>
    #include <sys/shm.h>
函数说明
    得到一个共享内存标识符或创建一个共享内存对象并返回共享内存标识符
函数原型
    int shmget(key_t key, size_t size, int shmflg)
函数传入值
    key: 
        0(IPC_PRIVATE)：会建立新共享内存对象
        大于0的32位整数：视参数shmflg来确定操作。通常要求此值来源于ftok返回的IPC键值
    size:
        大于0的整数：新建的共享内存大小，以字节为单位
        0：只获取共享内存时指定为0
    shmflg:
        0：取共享内存标识符，若不存在则函数会报错
        IPC_CREAT：当shmflg&IPC_CREAT为真时，如果内核中不存在键值与key相等的共享内存，则新建一个共享内存；
        如果存在这样的共享内存，返回此共享内存的标识符
        IPC_CREAT|IPC_EXCL：如果内核中不存在键值 与key相等的共享内存，则新建一个共享内存；如果存在这样的
        共享内存则报错
函数返回值
    成功：返回共享内存的标识符
    出错：-1，错误原因存于errno中
附加说明
    上述shmflg参数为模式标志参数，使用时需要与IPC对象存取权限（如0600）进行|运算来确定信号量集的存取权限
错误代码
    EINVAL：参数size小于SHMMIN或大于SHMMAX
    EEXIST：预建立key所指的共享内存，但已经存在
    EIDRM：参数key所指的共享内存已经删除
    ENOSPC：超过了系统允许建立的共享内存的最大值(SHMALL)
    ENOENT：参数key所指的共享内存不存在，而参数shmflg未设IPC_CREAT位
    EACCES：没有权限
    ENOMEM：核心内存不足

    在Linux环境中，对开始申请的共享内存空间进行了初始化，初始值为0x00。
    如果用shmget创建了一个新的消息队列对象时，则shmid_ds结构成员变量的值设置如下：
        shm_lpid、shm_nattach、shm_atime、shm_dtime设置为0。
        msg_ctime设置为当前时间。
        shm_segsz设成创建共享内存的大小。
        shmflg的读写权限放在shm_perm.mode中。
        shm_perm结构的uid和cuid成员被设置成当前进程的有效用户ID，gid和cuid成员被设置成当前进程的有效组ID。
        其他共享内存函数

2.shmat
    shmat(把共享内存区对象映射到调用进程的地址空间)
所需头文件
    #include <sys/types.h>
    #include <sys/shm.h>
函数说明
    连接共享内存标识符为shmid的共享内存，连接成功后把共享内存区对象映射到调用进程的地址空间，随后可像本地空间一样访问
函数原型
    void *shmat(int shmid, const void *shmaddr, int shmflg)
函数传入值
    shmid
    共享内存标识符
    shmaddr
    指定共享内存出现在进程内存地址的什么位置，直接指定为NULL让内核自己决定一个合适的地址位置
    shmflg
    SHM_RDONLY：为只读模式，其他为读写模式
    函数返回值
    成功：附加好的共享内存地址
    出错：-1，错误原因存于errno中
附加说明
    fork后子进程继承已连接的共享内存地址。exec后该子进程与已连接的共享内存地址自动脱离(detach)。进程结束后，
    已连接的共享内存地址会自动脱离(detach)
错误代码
    EACCES：无权限以指定方式连接共享内存
    EINVAL：无效的参数shmid或shmaddr
    ENOMEM：核心内存不足


3.shmdt函数原型
    shmdt(断开共享内存连接)
所需头文件
    #include <sys/types.h>
    #include <sys/shm.h>
函数说明
    与shmat函数相反，是用来断开与共享内存附加点的地址，禁止本进程访问此片共享内存
函数原型
    int shmdt(const void *shmaddr)
函数传入值
    shmaddr：连接的共享内存的起始地址
函数返回值
    成功：0
    出错：-1，错误原因存于errno中
附加说明
    本函数调用并不删除所指定的共享内存区，而只是将先前用shmat函数连接（attach）好的共享内存脱离（detach）目前的进程
错误代码
    EINVAL：无效的参数shmaddr

4.shmctl函数原型
    shmctl(共享内存管理)
所需头文件
    #include <sys/types.h>
    #include <sys/shm.h>
函数说明
    完成对共享内存的控制
函数原型
    int shmctl(int shmid, int cmd, struct shmid_ds *buf)
函数传入值
    shmid
        共享内存标识符
    cmd
        IPC_STAT：得到共享内存的状态，把共享内存的shmid_ds结构复制到buf中
        IPC_SET：改变共享内存的状态，把buf所指的shmid_ds结构中的uid、gid、mode复制到共享内存的shmid_ds结构内
        IPC_RMID：删除这片共享内存
    buf
        共享内存管理结构体。具体说明参见共享内存内核结构定义部分
函数返回值
    成功：0
    出错：-1，错误原因存于errno中
错误代码
    EACCES：参数cmd为IPC_STAT，却无权限读取该共享内存
    EFAULT：参数buf指向无效的内存地址
    EIDRM：标识符为shmid的共享内存已被删除
    EINVAL：无效的参数cmd或shmid
    EPERM：参数cmd为IPC_SET或IPC_RMID，却无足够的权限执行
*/

#define SIZE   10

typedef struct Stu
{ 
     int age;
     char name[10];
}Stu;
 
int main( void)
{ 
    int i =0;
    Stu s;
    strcpy(s.name, "jack");

    //创建共享内存段
    int id = shmget(1234, 8, IPC_CREAT|0644);
    if( id == -1){
        perror("shmget");
        exit(1);      
    }

    //挂载到进程的地址空间
    Stu* p = (Stu*)shmat(id, NULL, 0);

    while(1)
    {    
        s.age = i++;
        memcpy(p, &s, sizeof(Stu));  //写到共享段中
        sleep(2);
    }

    return 0;
}

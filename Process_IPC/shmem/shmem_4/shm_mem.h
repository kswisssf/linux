#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <error.h>

#define SHM_SIZE     1024

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *buf_info;
    void *pad;
};

/* 创建信号量函数*/
int creatsem(const char *pathname, int proj_id, int members, int init_val);

int opensem(const char *pathname, int proj_id);

/* p操作, 获取信号量*/
int sem_p(int semid, int index);

/* V操作, 释放信号量*/
int sem_v(int semid, int index);

/* 删除信号量几*/
int sem_delete(int semid);

/* 等待信号量为1*/
int wait_sem(int semid, int index);

/* 创建共享内存*/
int creatshm(char *pathname, int proj_id, size_t size);

/* 删除共享内存*/
int deleteshm(int sid);

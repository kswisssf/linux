#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <error.h>

#include "shm_mem.h"

/* 创建信号量函数*/
int creatsem(const char *pathname, int proj_id, int members, int init_val)
{
    key_t msgkey;
    int index, sid;
    union semun semopts;
    
    if((msgkey = ftok(pathname, proj_id)) == -1){
        perror("ftok error!\n");
        return -1;
    }
    if((sid = semget(msgkey, members, IPC_CREAT|0666)) == -1){
        perror("semget call failed.\n");
        return -1;
    }
    semopts.val = init_val;
    for(index = 0; index < members; index++){
        semctl(sid, index, SETVAL, semopts);
    }
    
    return sid;
}

int opensem(const char *pathname, int proj_id)
{
    key_t msgkey;
    int sid;
    
    if((msgkey = ftok(pathname, proj_id)) == -1){
        perror("ftok error!\n");
        return -1;
    }
    
    if((sid = semget(msgkey, 0, 0666)) == -1){
        perror("open semget call failed.\n");
        return -1;
    }
    return sid;
}

/* p操作, 获取信号量*/
int sem_p(int semid, int index)
{
    //struct sembuf sbuf = {0, -1, SEM_UNDO};

    struct sembuf sbuf = {0, -1, IPC_NOWAIT};
    if(index < 0){
        perror("index of array cannot equals a minus value!\n");
        return -1;
    }
    sbuf.sem_num = index;
    if(semop(semid, &sbuf, 1) == -1){
        perror("A wrong operation to semaphore occurred!\n");
        return -1;
    }
    return 0;
}

/* V操作, 释放信号量*/
int sem_v(int semid, int index)
{
    //struct sembuf sbuf = {0, 1, SEM_UNDO};

    struct sembuf sbuf = {0, 1, IPC_NOWAIT};
    if(index < 0){
        perror("index of array cannot equals a minus value!\n");
        return -1;
    }
    sbuf.sem_num = index;
    if(semop(semid, &sbuf, 1) == -1){
        perror("A wrong operation to semaphore occurred!\n");
        return -1;
    }
    return 0;
}

/* 删除信号量几*/
int sem_delete(int semid)
{
    return (semctl(semid, 0, IPC_RMID));
}

/* 等待信号量为1*/
int wait_sem(int semid, int index)
{
    while(semctl(semid, index, GETVAL, 0) == 0)
    {
        //wait_num++;

        usleep(500);
    } 
    //printf("wait_num = %x\n", wait_num);

    return 1;

}

/* 创建共享内存*/
int creatshm(char *pathname, int proj_id, size_t size)
{
    key_t shmkey;
    int sid;
    
    if((shmkey = ftok(pathname, proj_id)) == -1){
        perror("ftok error!\n");
        return -1;
    }
    if((sid = shmget(shmkey, size, IPC_CREAT|0666)) == -1){
        perror("shm call failed!\n");
        return -1;
    }
    return sid;
}

/* 删除共享内存*/
int deleteshm(int sid)
{
    void *p = NULL;
    return (shmctl(sid, IPC_RMID, p));
}
#include "shm_mem.h"

int main(int argc, char** argv)
{
    int semid, shmid;
    char *shmaddr;
    printf("What!!!!!!!!!!!\n");
    if((shmid = creatshm(".", 57, SHM_SIZE)) == -1)
        return -1;
    if((shmaddr = shmat(shmid, (char*)0, 0)) == (char *)-1){
        perror("attch shared memory error!\n");
        exit(1);
    }
    if((semid = opensem("./", 39)) == -1)
        return -1;
    printf("read start....................\n");        
    while(1){
        printf("read : ");
        wait_sem(semid, 0);  //等待信号量可以获取
        if(sem_p(semid, 0) == -1) //获取信号量失败退出。当写者写入'#'时表示退出
            break;
        printf("%s", shmaddr);

        sem_v(semid, 0);
        usleep(1000);
    }    
    return 0;
}
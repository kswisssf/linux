#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
6、实例
程序 msg_send 和 msg_recv 分别用于向消息队列发送数据和接收数据。

6.2 msg_recv
msg_recv 程序接收一个参数，表示接收哪种类型的消息。
比如./msg_recv 4 表示接收类型为 4 的消息，并打印在屏幕。

*/

typedef struct {
    char name[20];
    int age;
}Person;

typedef struct {
    long type;
    Person person;
}Msg;

void printMsg(Msg *msg) {
    printf("{ type = %ld, name = %s, age = %d }\n",
           msg->type, msg->person.name, msg->person.age);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s <type>\n", argv[0]);
        return -1;
    }
    
    // 要获取的消息类型
    long type = atol(argv[1]);
    
    // 获取 ipc 内核对象 id
    int id = msgget(0x8888, 0);
   
    
    Msg msg;
    int res;
    
    while(1) {
        // 以非阻塞的方式接收类型为 type 的消息
        res = msgrcv(id, &msg, sizeof(Person), type, IPC_NOWAIT);
        if (res < 0) {
            // 如果消息接收完毕就退出，否则报错并退出
            if (errno == ENOMSG) {
                printf("No message!\n");
                break;
            }

        }
        // 打印消息内容
        printMsg(&msg);
    }
    return 0;
}
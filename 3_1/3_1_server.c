#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXT 100
#define MAX_PRIOR 5

typedef struct{
    long mtype;
    char mtext[MAX_TEXT];
}msg_buf;

int create_queue(key_t key);
void process_message(msg_buf msg, int msgid);

int main(){
    key_t server_key = ftok("server_key", 'q');
    int server_msgid = create_queue(server_key);

    key_t client_key = ftok("client_key", 'q');
    int client_msgid = create_queue(client_key);

    if(server_msgid < 0 || client_msgid < 0){
        perror("Error: queues have not created.\n");
        exit(EXIT_FAILURE);
    }

    printf("Server is running.\n");
    while(1){
        msg_buf msg;
        if(msgrcv(server_msgid, &msg, sizeof(msg), -MAX_PRIOR, IPC_NOWAIT) < 0){ //���������� ����� ����. ��� 0 - ����� ��� �������
            continue;
        }
        printf("Server received message.\n");
        if(msg.mtype == 1){ //�������� ���� �� ���������� ���������
            printf("Received exit message. Server closing.\n");
            break;
        }
        process_message(msg, client_msgid);
    }

    msgctl(server_msgid, IPC_RMID, 0);
    printf("Server queue is destroyed.\n");
    msgctl(client_msgid, IPC_RMID, 0);
    printf("Client queue is destroyed.\n");

    printf("Server is over.\n");
    return 0;
}

int create_queue(key_t key){
    int msgid = msgget(key, 0666 | IPC_CREAT);
    return msgid;
}

void process_message(msg_buf msg, int msgid){
    int length = strlen(msg.mtext);
    char temp[MAX_TEXT];
    strcpy(temp, msg.mtext);
    char prev = '\0';
    char cur;
    for(int i = 0; i < length; i++){
        cur = temp[i];
        if(prev == cur && (cur == ' ' || cur == '\t')){
            for(int j = i; j < length; j++){
                temp[j] = temp[j+1];
            }
            i--;
        }
        prev = cur;
    }
    strcpy(msg.mtext, temp);
    msgsnd(msgid, &msg, sizeof(msg), IPC_NOWAIT);//�������� � �������
}
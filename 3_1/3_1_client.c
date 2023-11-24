#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_TEXT 100
#define MAX_PRIOR 5

typedef struct{
    long mtype;
    char mtext[MAX_TEXT];
}msg_buf;

int main(int argc, char *argv[]){
    key_t server_key = ftok("server_key", 'r');
    int server_msgid = msgget(server_key, 0);
    if(server_msgid < 0){
        perror("Server does not exist.\n");
        exit(1);
    }

    key_t client_key = ftok("client_key", 'r');
    int client_msgid = msgget(client_key, 0);
    if(client_msgid < 0){
        perror("Client does not exist.\n");
        exit(1);
    }
    
    char *line = NULL;
    size_t len = 0;
    char str[MAX_TEXT];
    int count = 0;
    int prior = 0;
    for(int i = 1; i < argc; i++){
        int j = 0;
        int k = 0;
        int flag = 0;
        FILE *file = fopen(argv[i], "r");
        if(!file){
            perror("File is not open.\n");
            exit(1);
        }
        while(getline(&line, &len, file) != -1){
            msg_buf msg;
            sscanf(line, "prior=%d", &prior);
            msg.mtype = prior;
            while(flag != 2){
                if(line[j] == '='){
                    flag++;
                }
                j++;
            }
            while(k < MAX_TEXT && line[j] != '\"'){
                str[k] = line[j];
                k++;
                j++;
            }
            strcpy(msg.mtext, str);
            msgsnd(server_msgid, &msg, sizeof(msg), 0); //отправка в очередь
            printf("Message is sent.\n");
            count++;
        }
        fclose(file);
    }
    sleep(1);
    msg_buf msg_2;
    while(count){
        client_msgid = msgget(client_key, 0);
        if(client_msgid < 0){
            break;
        }
        if(msgrcv(client_msgid, &msg_2, sizeof(msg_2), -MAX_PRIOR, IPC_NOWAIT) < 0){
            continue;
        }
        printf("Processed message %s with priority %ld.\n", msg_2.mtext, msg_2.mtype);
        count--;
    }

    printf("Client is over.\n");
    return 0;
}
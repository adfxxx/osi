#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    int pid;
    pid = fork();
    printf("%d\n", pid);
    if (pid == 0){
        printf("This is child process\n");
    }
    else if(pid > 0){
        printf("This is parent process\n");
    }
    else{
        printf("Error");
    }
    // fork();
    // printf("Hi\n");
    // fork();
    // printf("Hi\n");
}
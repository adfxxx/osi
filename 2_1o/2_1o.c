#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgrp = getpgrp();
    pid_t uid = getuid();
    pid_t euid = geteuid();
    pid_t gid = getgid();
    pid_t egid = getegid();
    printf("\tPid: %d\n", pid);
    printf("\tarent pid: %d\n", ppid);
    printf("\tGroup process: %d\n", pgrp);
    printf("\tUID: %d\n", uid); //идентификатор пользователя, запустившего процесс
    printf("\tGID: %d\n", gid); //реальный идекнтификатор группы = идентификатор текущей группы
    printf("\tEUID: %d\n", euid); //определяет права пользователя 
    printf("\tEGID: %d\n", egid);//определяет права группы
    return 0;
}
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
    printf("\tUID: %d\n", uid); //������������� ������������, ������������ �������
    printf("\tGID: %d\n", gid); //�������� �������������� ������ = ������������� ������� ������
    printf("\tEUID: %d\n", euid); //���������� ����� ������������ 
    printf("\tEGID: %d\n", egid);//���������� ����� ������
    return 0;
}
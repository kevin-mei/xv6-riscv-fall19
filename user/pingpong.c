#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parient_fds[2], child_fds[2];
    // create a pipe, with two FDs in fds[0], fds[1].
    pipe(parient_fds);
    pipe(child_fds);
    int pid;
    pid = fork();
    char buff[100];
    if(0 == pid)
    {
        // 子进程 这里不关闭不用的文件描述符，本身这个程序使用不多，就没关，等程序退出时自行处理
        //printf("child send ping\n");
        write(parient_fds[1], "ping\n", 5);
        read(child_fds[0], buff, sizeof(buff)); // 阻塞，等待父进程写入pang
        printf("3: received %s", buff);// 写入pong的时候已经加\n，所以这里打印的时候，不用加\n
        exit();
    }else
    {
        // 父进程
        read(parient_fds[0], buff, sizeof(buff));
        printf("4: received %s", buff);
        write(child_fds[1], "pong\n", 5);
        wait();
        //printf("parent received child exit, then parent exit.\n");
    }
    
    exit();
}

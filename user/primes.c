#include "kernel/types.h"
#include "user/user.h"

void createprocess(int parent_fds[2])
{
    // 要做的事情
    int fixNum = 0; // 保存当前进程的数，当前的进程，只留存自己第一次接收到的数字，然后判断之后接收到的数字，能不能被第一次接受到的数组整除，如果可以，则忽略，不可以，则传递到下一个进程
    // 从传入的parent_fds的读端，读取一个数字, 如果本地没有
    close(parent_fds[1]); // 用不着写端，直接关闭
    read(parent_fds[0], fixNum, 1);
    printf("prime %d\n", fixNum);
    int child_fds[2];
    pipe(child_fds);
    int curNum = 0;
    while (1)
    {
        if (read(parent_fds[0], curNum, 1))
        {
            if (0 != curNum % fixNum)
            {
                int pid = fork();
                if (0 == pid)
                {
                    createprocess(child_fds);
                }
                else
                {
                    // 父进程，需要将当前进程的curNum传递给子进程
                    close(child_fds[0]); // 关闭读端
                    write(child_fds[1], curNum, 1);
                }
            }
        }
        else
            break; // 读取失败
    }
    exit();
}

int main(int argc, char *argv[])
{
    // main 函数这里，直接写创建子进程，在父进程写2-35的数字给子进程
    int parent_fds[2];
    pipe(parent_fds);

    int pid = fork();
    if(0 == pid)
    {
        createprocess(parent_fds);
    }else
    {
        close(parent_fds[0]);
        for(int i = 2;i < 36;i++)
        {
            write(parent_fds[1], i, 1);
        }
        close(parent_fds[1]);
    }
    exit();
}
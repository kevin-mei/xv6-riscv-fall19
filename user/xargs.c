#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // 第一次传进来的xagrs echo bye
    // 然后要从标准输入中读取数据，直到遇到\n
    char buff[1024];
    int offset = 0;
    int len = 0;
    while((len = read(0, buff+offset, sizeof(buff)-offset)) > 0)
    {
        // 判断遇到\n
        if('\n' == buff[offset+len-1])
        {
            break;
        }
        offset+=len;
    }
    // 把buff添加进argv[], 然后exec
    // 这里抄下echo的作业，不行，应该是fork，然后子进程执行argv[1]，然后argv[2]及以后的数据+buff是新的argv
    int pid = fork();
    if(pid < 0)
    {
        exit();
    }else if(0 == pid)
    {
        int i =2;
        while(i +1 < argc)
        {
            argv[i] = argv[i+1];
            i++;
        }
        argv[i+1] = buff; 
        exec(argv[1], &argv[2]);
    }else
    {
        wait();
    }

    exit();
}
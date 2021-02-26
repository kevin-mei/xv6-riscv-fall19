#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	// 第一次传进来的xagrs echo bye
	// 然后要从标准输入中读取数据，直到遇到\n
	char buff[1024];
	int offset = 0;
	int len = 0;
	int i =0;
	char ch;
	while(i +1 < argc)
	{
		argv[i] = argv[i+1];
		//printf("argv[%d] = [%s]\n", i, argv[i]);            
		i++;
	}
	while((len = read(0, &ch, sizeof(ch))) > 0)
	{
		// 判断遇到\n
		if('\n' == ch)
		{
			// 把buff添加进argv[], 然后exec
			// fork，然后子进程执行argv[1]，然后argv[2]及以后的数据+buff是新的argv
			argv[i] = buff;
			int pid = fork();
			if(pid < 0)
			{
				exit();
			}else if(0 == pid)
			{
				exec(argv[0], &argv[0]);
			}else
			{
				wait();
			}
			memset(buff,0x00,sizeof(buff));
			offset = 0;
		}else
		{

			buff[offset++]=ch;
		}
	}
	exit();
}

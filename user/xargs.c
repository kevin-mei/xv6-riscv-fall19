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
  // 传给执行程序依旧使用argv，因为考虑到argc不一定是三个值，可能5，6个都可能，单独创建新的argv需要动态分配内存，比较麻烦
  // argv[0] = “xargs”；我们将argv[0]剔除掉，后面的参数往前挪一位，空出最后一个元素，接受新输入的一行数据即可组成新的argv
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
			// fork，然后子进程执行argv[0]，传入的参数从argv[0]开始算
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
      // 执行完这一行的数据，重置接收区
			memset(buff,0x00,sizeof(buff));
			offset = 0;
		}else
		{
			buff[offset++]=ch;
		}
	}
	exit();
#include "kernel/types.h"
#include "user/user.h"

// createprocess 要做的事情: 保存当前进程的数，当前的进程，只留存自己第一次接收到的数字
// 然后判断之后接收到的数字，能不能被第一次接受到的数组整除
// 如果可以，则忽略，不可以，则传递到下一个进程
void createprocess(int parent_fds[2])
{
	int fixNum = 0; // 只留存自己第一次接收到的数字
	// 从传入的parent_fds的读端，读取一个数字, 如果本地没有
	close(parent_fds[1]); // 用不着paren_fds的写端，直接关闭
	int curNum = 0;
	if (read(parent_fds[0], &curNum, sizeof(curNum)))
	{
		fixNum = curNum;
		printf("prime %d\n", fixNum);
	}
	int child_fds[2];
	pipe(child_fds);
	if (0 != read(parent_fds[0], &curNum, sizeof(curNum)))
	{
		int pid = fork();
		if(pid < 0)
		{
			exit();
		}else if (0 == pid)
		{ // 子进程中递归调用createprocess
			createprocess(child_fds);
		}
		else
		{ // 父进程
			close(child_fds[0]); // 关闭child_fds读端
			do{
				// 需要将当前进程的curNum传递给子进程
				if(0 != curNum % fixNum) // 判断读到的curNum能否被当前进程的素数整除
					write(child_fds[1], &curNum, sizeof(curNum));
			}while(read(parent_fds[0], &curNum, sizeof(curNum)));
    }
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
		exit();
	}else
	{
		close(parent_fds[0]);
		for(int i = 2;i < 36;i++)
		{
			write(parent_fds[1], &i, sizeof(i));
		}
		close(parent_fds[1]);
	}
	wait();
	exit();
}
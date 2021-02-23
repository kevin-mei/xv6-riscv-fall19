#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int fd, i;

    if (argc < 1)
    {
        printf("error: too few argvment!\n");
        exit();
    }

    int nsleep = atoi(argv[1]);
    sleep(nsleep);
    exit();
}
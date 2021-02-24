#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("usage: sleep seconds!\n");
        exit();
    }

    int nsleep = atoi(argv[1]);
    sleep(nsleep);
    exit();
}
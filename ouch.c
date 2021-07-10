#include <stdio.h>
#include <signal.h>

static void sigHandler()
{
    printf("catch signal!\r\n");
}

int main(int argc, char const *argv[])
{
    int j;

    if (signal(SIGINT, sigHandler) == SIG_ERR)
    {
        printf("signal() error!\r\n");
    }

    for (j = 0; j < 1; j++)
    {
        printf("%d ", j);
        sleep(1);
    }

    return 0;
}

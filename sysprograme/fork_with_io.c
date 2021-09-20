/*fork与stdio缓冲区的交互*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/*./test 和 ./test > a 的结果不一样*/

int main(int argc, char const *argv[])
{
    printf("Hello,World!\n");
    write(STDOUT_FILENO, "Ciao\n", 5);
    if (fork() == -1)
    {
        printf("fork error!\r\n");
    }

    exit(EXIT_SUCCESS);
    return 0;
}

/*父子进程文件共享例子*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{

    int fd, flags;
    char tempfile[] = "/tmp/testXXXXXX";    //创建临时文件还是有规范的

    setbuf(stdout, NULL); //关闭输出流 函数用来设置缓冲区大小

    fd = mkstemp(tempfile); //mkstemp()打开临时文件
    if (fd == -1)
    {
        printf("Open temp file false!\r\n");
        exit(EXIT_FAILURE);
    }

    printf("Before fork file lseek = %lld\r\n", (long long)lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
    {
        printf("Get file flags false!\r\n");
        exit(EXIT_FAILURE);
    }

    printf("Before Fork File Flags = %s\r\n", (flags & O_APPEND) ? "on" : "off");

    /*创建子进程*/
    switch (fork())
    {
    case -1:
        printf("Fork error\r\n");
        exit(EXIT_FAILURE);
        break;
    case 0:
        if (lseek(fd, 1000, SEEK_SET) == -1)
        {
            printf("child lseek error!\r\n");
        }

        flags = fcntl(fd, F_GETFL);
        flags |= O_APPEND;
        if (fcntl(fd, F_SETFL, flags) == -1)
        {
            printf("child setflags error!\r\n");
        }
        _exit(EXIT_SUCCESS);
        break;

    default:
        if (wait(NULL) == -1)
        { /* 等待子进程 */
            printf("wait child error!\r\n");
        }
        printf("child has exited!");
        printf("Before fork file lseek = %lld\r\n", (long long)lseek(fd, 0, SEEK_CUR));

        flags = fcntl(fd, F_GETFL);
        if (flags == -1)
        {
            printf("Get file flags false!\r\n");
            exit(EXIT_FAILURE);
        }

        printf("Before Fork File Flags = %s\r\n", (flags & O_APPEND) ? "on" : "off");
         _exit(EXIT_SUCCESS);
        break;
    }

    return 0;
}

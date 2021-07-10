/*exec会将子进程的执行内容替换成exec中的执行程序,子进程的退出函数需要手写，要是有需要的话*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    pid_t child_pid;

    switch (child_pid = fork())
    {
    case -1:
        printf("Fork error\r\n");
        exit(EXIT_FAILURE);
        break;
    case 0:
        execlp("ls","-l","-a",NULL);    //NULL表示结束
        // char *argv[] = {"ls","-l","-a",NULL};
        // execvp("ls",argv);
        break;
    default:
        break;
    }
    return 0;           //exit(0) 0 =EXIT_SUCCESS
}




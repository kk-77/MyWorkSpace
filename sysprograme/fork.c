#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/*使用fork创建子进程*/
static int idata = 111;

int main(int argv,char *argc[]){
    
    int istack = 222;
    pid_t childPid;

    switch (childPid = fork())
    {
    case -1:
        printf("Creat Child Process error\r\n");
        break;
    case 0:         /* Child Process*/
        idata *= 3;
        istack *= 3;
        break;
    default:        /* Parent Process*/
        sleep(3);
        break;
    }
    /* Both exec here!*/
    printf("PID = %ld %s idata = %d istack = %d \r\n",(long)getpid(),(childPid == 0)?("Child"):("Parent"),
        idata,istack);
    exit(EXIT_SUCCESS);

    return 0;
}

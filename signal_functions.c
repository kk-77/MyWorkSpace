#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
//extern const char *const sys_siglist[];

/*Print list of signals within a signal set*/
void printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
    int sig, cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++)
    {
        if (sigismember(sigset, sig))
        {
            cnt++;
            fprintf(of, "%s %d (%s)\n", prefix, sig, strsignal(sig));
        }
    }

    if (cnt == 0)
    {
        fprintf(of, "%s <empty signal set>\n", prefix);
    }
}

int printSigMask(FILE *of, const char *msg)
{
    sigset_t currMask;

    if (msg != NULL)
    {
        fprintf(of, "%s", msg);
    }

    if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
    {
        return -1;
    }

    printSigset(of, "\t\t", &currMask);
}

int printPendingSigs(FILE *of, const char *msg)
{
    sigset_t pendingSig;

    if (msg != NULL)
    {
        fprintf(of, "%s", msg);
    }

    if (sigpending(&pendingSig) == -1)
    {
        return -1;
    }

    printSigset(of, "\t\t", &pendingSig);
}

int main(int argc, char const *argv[])
{
    int j;

    sigset_t set, sigpend, sigmask;

    sigemptyset(&set);
    sigemptyset(&sigmask);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    printSigset(stdout, "\t\t", &set);

    printf("\r\n");
    if (sigprocmask(SIG_BLOCK, &set, &sigmask) == -1)
    {
        printf("error sigprocmask\r\n");
    } /*阻塞SIGINT*/
    //printSigset(stdout, "\t\t", &sigmask);
    printSigMask(stdout,"GIGMASK");

    printPendingSigs(stdout,"GIGPending");
    //sigpending(&set);
    //printSigMask(stdout, "SIG");
    // sigdelset(&set, SIGUSR1);
   // printSigset(stdout, "\t\t", &set);
    return 0;
}

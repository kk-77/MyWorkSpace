#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <syslog.h>
#include <signal.h>
#include <stdint.h>
#include <netinet/in.h>
#include <time.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <regex.h>
/*
1.主循环使用select，可以用来监听网络socket,超时，还有信号。
2.接收到信号之后，优先调用 signal_handler 函数写入到管道中。
3.select 会依次监听管道中的数据是否可读来一一处理短时间之内收到的全部信号。
    看到select 可以依次处理管道中的信号。
*/

/*declaration function*/
void xpipe(int filedes[2]);
int close_on_exec_on(int fd);
int ndelay_on(int fd);

#define RENYJ_TRACE(format, args...)                                                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        fprintf(stderr, "RENYJ_TRACE >>> %s->%s()->line.%d : " format "\n", __FILE__, __FUNCTION__, __LINE__, ##args); \
    } while (0)

static struct fd_pair signal_pipe;
struct fd_pair
{
    int rd;
    int wr;
};
#define piped_pair(pair) pipe(&((pair).rd))
#define xpiped_pair(pair) xpipe(&((pair).rd))

static void signal_handler(int sig)
{
    unsigned char ch = sig; /* use char, avoid dealing with partial writes */
    if (write(signal_pipe.wr, &ch, 1) != 1)
    {
        RENYJ_TRACE("cannot send signal");
    }
    RENYJ_TRACE("Write Sig %d", ch);
}

int safe_read(int fd, void *buf, size_t count)
{
    int n;

    do
    {
        n = read(fd, buf, count);
    } while (n < 0 && errno == EINTR);

    return n;
}

int sig_pip_read(const fd_set *rfds)
{
    unsigned char sig;

    if (!FD_ISSET(signal_pipe.rd, rfds))
        return 0;

    if (safe_read(signal_pipe.rd, &sig, 1) != 1)
        return -1;

    return sig;
}

int sig_pip_set(fd_set *rfds, int extra_fd)
{
    FD_ZERO(rfds);
    printf("signal_pipe.rd = %d\r\n",signal_pipe.rd);
    FD_SET(signal_pipe.rd, rfds);
    if (extra_fd >= 0)
    {
        close_on_exec_on(extra_fd);
        FD_SET(extra_fd, rfds);
    }
    return signal_pipe.rd > extra_fd ? signal_pipe.rd : extra_fd;
}

void xpipe(int filedes[2])
{
    if (pipe(filedes))
    {
        RENYJ_TRACE("Can't create pipe");
        exit(-1);
    }
}

int close_on_exec_on(int fd)
{
    return fcntl(fd, F_SETFD, FD_CLOEXEC);
}

int ndelay_on(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int ndelay_off(int fd)
{
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
}

int do_wait_ns(int s)
{
    while (s--)
    {
        RENYJ_TRACE("waiting s=%d", s);
        sleep(1);
    }
    return 0;
}

int sig_setup()
{
    xpiped_pair(signal_pipe);
    close_on_exec_on(signal_pipe.rd);
    close_on_exec_on(signal_pipe.wr);
    ndelay_on(signal_pipe.wr);
    signal(SIGUSR1, signal_handler);
    signal(SIGALRM, signal_handler);
    signal(SIGUSR2, signal_handler);

    return 0;
}

int main(int argc, char *argv[])
{
    int ret = -1;
    struct timeval tv;
    int max_fd = 0;
    int cur_sig = -1;
    fd_set rfds;
    sig_setup();
    while (1)
    {
        max_fd = sig_pip_set(&rfds, -1);
        printf("max_fd = %d\r\n", max_fd);
        ret = select(max_fd + 1, &rfds, NULL, NULL, NULL);
        if (ret < 0)
        {
            if (errno == EINTR)
                continue;
        }
        cur_sig = sig_pip_read(&rfds);
        switch (cur_sig)
        {
        case SIGUSR1:
        {
            RENYJ_TRACE("SIG_IO Recieve SIGUSR1");
            do_wait_ns(5);
            break;
        }
        case SIGUSR2:
        {
            RENYJ_TRACE("SIG_IO Recieve SIGUSR2");
            do_wait_ns(5);
            break;
        }
        case SIGALRM:
        {
            RENYJ_TRACE("SIG_IO Recieve SIGALRM");
            do_wait_ns(5);
            break;
        }

        default:
            break;
        }
    }
}

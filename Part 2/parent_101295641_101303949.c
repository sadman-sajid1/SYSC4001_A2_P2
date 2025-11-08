// Sadman Sajid 101303949
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

volatile sig_atomic_t keep_running = 1;

void handle_sigterm(int sig)
{
    (void)sig;
    keep_running = 0;
}

int main(void)
{
    if (signal(SIGTERM, handle_sigterm) == SIG_ERR)
    {
        perror("signal");
        return 1;
    }

    setbuf(stdout, NULL); // so output is readable and not delayed

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        execl("./child", "child", (char *)NULL);
        perror("execl failed"); // if exec fails
        _exit(1);
    }
    else
    {
        long counter = 0;
        long cycle = 0;
        const useconds_t delay_us = 400000;

        printf("Parent started. PID=%d, Child PID=%d\n", (int)getpid(), (int)pid);

        while (keep_running)
        {
            if (counter % 3 == 0)
            { // multiple of 3
                printf("[Parent %d] Cycle %ld — %ld is a multiple of 3\n", getpid(), cycle, counter);
            }
            else
            {
                printf("[Parent %d] Cycle %ld\n", getpid(), cycle);
            }
            counter++;
            cycle++;
            usleep(delay_us);
        }

        printf("Parent (PID=%d) exiting cleanly.\n", getpid());
    }

    return 0;
}

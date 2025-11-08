// Sadman Sajid 101303949
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    setbuf(stdout, NULL);

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        execl("./child", "child", (char *)NULL);
        perror("execl failed");
        _exit(1);
    }
    else
    {
        // increment while waiting for child
        long counter = 0;
        long cycle = 0;
        const useconds_t delay_us = 100000;

        printf("Parent started. PID=%d, Child PID=%d\n", (int)getpid(), (int)pid);

        int status;
        while (1)
        {
            if (counter % 3 == 0)
                printf("[Parent %d] Cycle %ld — %ld is a multiple of 3\n", getpid(), cycle, counter);
            else
                printf("[Parent %d] Cycle %ld\n", getpid(), cycle);

            counter++;
            cycle++;
            usleep(delay_us);

            pid_t pStatus = waitpid(pid, &status, WNOHANG);
            if (pStatus == pid)
            {
                printf("Child finished. Parent exiting.\n");
                break;
            }
        }
    }

    return 0;
}


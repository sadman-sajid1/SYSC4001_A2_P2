// Sadman Sajid 101303949
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

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

    setbuf(stdout, NULL); // clean and undelayed output

    long counter = 0;
    long cycle = 0;
    const useconds_t delay_us = 400000;

    printf("Child started. PID=%d\n", getpid());

    while (keep_running)
    {
        if (counter % 3 == 0)
        {
            printf("[Child %d] Cycle %ld — %ld is a multiple of 3\n", getpid(), cycle, counter);
        }
        else
        {
            printf("[Child %d] Cycle %ld\n", getpid(), cycle);
        }
        counter--; // decreases timer
        cycle++;
        usleep(delay_us);
    }

    printf("Child (PID=%d) exiting cleanly.\n", getpid());
    return 0;
}

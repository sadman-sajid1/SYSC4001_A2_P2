// Sadman Sajid 101303949
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    setbuf(stdout, NULL);

    long counter = 0;
    long cycle = 0;
    const useconds_t delay_us = 100000;

    printf("Child started. PID=%d\n", (int)getpid());

    while (counter >= -500)
    {
        if (counter % 3 == 0)
            printf("[Child %d] Cycle %ld — %ld is a multiple of 3\n", getpid(), cycle, counter);
        else
            printf("[Child %d] Cycle %ld\n", getpid(), cycle);

        counter--;
        cycle++;
        usleep(delay_us);
    }

    printf("Child (PID=%d) reached counter < -500. Exiting.\n", getpid());
    return 0;
}

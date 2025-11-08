// Sadman Sajid 101303949
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

struct sharedData
{
    int multiple;
    int counter;
};

int main(int argc, char *argv[])
{
    setbuf(stdout, NULL);

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <shmid>\n", argv[0]);
        exit(1);
    }

    int shmid = atoi(argv[1]);

    struct sharedData *sh = (struct sharedData *)shmat(shmid, NULL, 0);
    if (sh == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    // until counter > 100
    while (sh->counter <= 100)
    {
        usleep(100000);
    }

    // until counter > 500
    while (sh->counter <= 500)
    {
        if (sh->counter % sh->multiple == 0)
            printf("[Child %d] counter=%d is a multiple of %d\n", getpid(), sh->counter, sh->multiple);
        else
            printf("[Child %d] counter=%d\n", getpid(), sh->counter);

        usleep(100000);
    }

    shmdt(sh);
    printf("Child exiting.\n");
    return 0;
}

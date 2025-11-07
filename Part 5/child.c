// Sadman Sajid 101303949
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct Shared
{
    int multiple;
    int counter;
};

static struct sembuf P = {0, -1, 0};
static struct sembuf V = {0, 1, 0};

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <shmid> <semid>\n", argv[0]);
        return 1;
    }

    setbuf(stdout, NULL);

    int shmid = atoi(argv[1]);
    int semid = atoi(argv[2]);

    struct Shared *sh = (struct Shared *)shmat(shmid, NULL, 0);
    if (sh == (void *)-1)
    {
        perror("shmat failed");
        return 1;
    }

    while (1)
    {
        semop(semid, &P, 1);
        int c = sh->counter;
        semop(semid, &V, 1);
        if (c > 100)
            break;
        usleep(100000);
    }

    while (1)
    {
        semop(semid, &P, 1);
        sh->counter++;
        int c = sh->counter;
        int m = sh->multiple;
        semop(semid, &V, 1);

        if (c % m == 0)
            printf("[CHILD ] counter=%d (multiple=%d)\n", c, m);

        if (c > 500)
            break;
        usleep(100000);
    }

    shmdt(sh);
    return 0;
}

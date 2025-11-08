// Sadman Sajid 101303949
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct SharedMem {
    int multipleNum;
    int mainCounter;
};

static struct sembuf lockOp = {0, -1, 0};
static struct sembuf unlockOp = {0, 1, 0};

int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <shmid> <semid>\n", argv[0]);
        return 1;
    }

    setbuf(stdout, NULL);

    int sharedId = atoi(argv[1]);
    int semId = atoi(argv[2]);

    struct SharedMem *shared = (struct SharedMem *)shmat(sharedId, NULL, 0);
    if (shared == (void *)-1) {
        perror("shmat failed");
        return 1;
    }

    // wait until counter > 100
    while (1) {
        semop(semId, &lockOp, 1);
        int counterVal = shared->mainCounter;
        semop(semId, &unlockOp, 1);

        if (counterVal > 100)
            break;

        usleep(100000); // tiny delay
    }

    while (1) {
        semop(semId, &lockOp, 1);
        shared->mainCounter++;
        int current = shared->mainCounter;
        int multi = shared->multipleNum;
        semop(semId, &unlockOp, 1);

        if (current % multi == 0)
            printf("[CHILD ] counter=%d (multiple=%d)\n", current, multi);

        if (current > 500)
            break;

        usleep(100000); // tiny delay
    }

    shmdt(shared);
    return 0;
}

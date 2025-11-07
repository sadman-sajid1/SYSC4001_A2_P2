// Sadman Sajid 101303949
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

struct SharedMem
{
    int multipleNum;
    int mainCounter;
};

union sem_union
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main()
{
    setbuf(stdout, NULL);

    int sharedId = shmget(IPC_PRIVATE, sizeof(struct SharedMem), IPC_CREAT | 0600);
    struct SharedMem *shared = (struct SharedMem *)shmat(sharedId, NULL, 0);
    shared->multipleNum = 3;
    shared->mainCounter = 0;

    int semId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    union sem_union semOpt;
    semOpt.val = 1;
    semctl(semId, 0, SETVAL, semOpt);

    struct sembuf lock = {0, -1, 0};
    struct sembuf unlock = {0, 1, 0};

    pid_t kidPid = fork();
    if (kidPid == 0)
    {
        char sidStr[32], semStr[32];
        snprintf(sidStr, sizeof(sidStr), "%d", sharedId);
        snprintf(semStr, sizeof(semStr), "%d", semId);
        char *args[] = {"./childProg", sidStr, semStr, NULL};
        execv(args[0], args);
        perror("exec failed");
        _exit(1);
    }

    while (1)
    {
        semop(semId, &lock, 1);
        shared->mainCounter++;
        int current = shared->mainCounter;
        int multi = shared->multipleNum;
        semop(semId, &unlock, 1);

        if (current % multi == 0)
            printf("[PARENT] counter=%d (multiple=%d)\n", current, multi);

        if (current > 500)
            break;

        usleep(100000); 
    }

    wait(NULL);
    shmdt(shared);
    shmctl(sharedId, IPC_RMID, NULL);
    semctl(semId, 0, IPC_RMID);

    return 0;
}

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

struct Shared
{
    int multiple;
    int counter;
};
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(void)
{
    setbuf(stdout, NULL);

    int shmid = shmget(IPC_PRIVATE, sizeof(struct Shared), IPC_CREAT | 0600);
    struct Shared *sh = (struct Shared *)shmat(shmid, NULL, 0);
    sh->multiple = 3;
    sh->counter = 0;

    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    union semun su;
    su.val = 1;
    semctl(semid, 0, SETVAL, su);

    struct sembuf P = {0, -1, 0};
    struct sembuf V = {0, 1, 0};

    pid_t pid = fork();
    if (pid == 0)
    {
        char arg1[32], arg2[32];
        snprintf(arg1, sizeof(arg1), "%d", shmid);
        snprintf(arg2, sizeof(arg2), "%d", semid);
        char *args[] = {"./child", arg1, arg2, NULL};
        execv(args[0], args);
        perror("execv failed");
        _exit(1);
    }

    while (1)
    {
        semop(semid, &P, 1);
        sh->counter++;
        int c = sh->counter, m = sh->multiple;
        semop(semid, &V, 1);

        if (c % m == 0)
            printf("[PARENT] counter=%d (multiple=%d)\n", c, m);
        if (c > 500)
            break;

        usleep(100000);
    }

    wait(NULL);
    shmdt(sh);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}

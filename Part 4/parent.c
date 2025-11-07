// Sadman Sajid 101303949
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct sharedData
{
    int multiple;
    int counter;
};

int main()
{
    setbuf(stdout, NULL);

    // init shared memory
    int shmid = shmget(IPC_PRIVATE, sizeof(struct sharedData), IPC_CREAT | 0666);
    if (shmid < 0)
    {
        perror("shmget failed");
        exit(1);
    }

    // attach shared memory
    struct sharedData *sh = (struct sharedData *)shmat(shmid, NULL, 0);
    if (sh == (void *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    // shared memory
    sh->multiple = 3;
    sh->counter = 0;

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0)
    {
        char arg[20];
        sprintf(arg, "%d", shmid);
        execl("./child", "child", arg, (char *)NULL);
        perror("execl failed");
        _exit(1);
    }
    else
    {
        // loop for the parent
        while (sh->counter <= 500)
        {
            if (sh->counter % sh->multiple == 0)
                printf("[Parent %d] counter=%d is a multiple of %d\n", getpid(), sh->counter, sh->multiple);
            else
                printf("[Parent %d] counter=%d\n", getpid(), sh->counter);

            sh->counter++;
            usleep(100000);
        }

        wait(NULL);

        // remove the shared memory
        shmdt(sh);
        shmctl(shmid, IPC_RMID, NULL);

        printf("Parent exiting.\n");
    }

    return 0;
}

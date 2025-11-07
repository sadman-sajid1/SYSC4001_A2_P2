// Sadman Sajid 101303949
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork(); // Creates new process

    if (pid < 0)
    {
        perror("Fork failed");
        exit(1);
    }

    int counter = 0;

    // Child process
    if (pid == 0)
    {
        while (1)
        {
            printf("Child Process (PID=%d) Counter: %d\n", getpid(), counter++);
            usleep(400000);
        }
    }
    // Parent process
    else
    {
        while (1)
        {
            printf("Parent Process (PID=%d) Counter: %d\n", getpid(), counter++);
            usleep(400000);
        }
    }

    return 0;
}

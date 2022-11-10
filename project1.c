#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

int main()
{

    bool loop = true;
    int processes;

    int fileLength = 0;

    while (loop)
    {

        // input for # of processese
        printf("Enter the number 1, 2, or 4 for the number of processes to run\n");

        scanf("%d", &processes);
        if (processes == 1 || processes == 2 || processes == 4)
        {
            loop = false;
        }
        else
        {
            printf("\n%d is not a valid processes\n", processes);
        }
    }

    bool loop2 = true;
    int file;
    // input for which file
    while (loop2)
    {

        // input for # of processes
        printf("Enter the number 1, 2, or 3 for the file to run\n");

        scanf("%d", &file);
        if (file == 1 || file == 2 || file == 3)
        {
            loop2 = false;
        }
        else
        {
            printf("\n%d is not a valid file\n", file);
        }
    }

    int finalNum = 0; // final sum of the file

    int fd[processes][2];

    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    suseconds_t timer = current_time.tv_usec;
    // loop for all processes
    for (int i = 0; i < processes; i++)
    {

        if (pipe(fd[i]) == -1)
        {
            printf("Pipe failed to create");
            return -1;
        }
        if (fork() == 0) // if not parent processes
        {

            FILE *fp; // open the file
            switch (file)
            {
            case 1:
                fp = fopen("file1.dat", "r"); // open file read only
                fileLength = 1000;
                break;
            case 2:
                fp = fopen("file2.dat", "r");
                fileLength = 10000;
                break;
            case 3:
                fp = fopen("file3.dat", "r");
                fileLength = 100000;
                break;
            }

            if (fp == NULL)
            { // if file not found
                printf("File not located\n");
                return -1;
            }

            int offset = (i / (float)processes) * fileLength * 5; // Size of the area each child must read
            fseek(fp, offset, SEEK_SET); // jump to an offset in the file so the child can scan that section

            int blockTotal = 0;                              // Sum of the block each processes reads
            for (int j = 0; j < (fileLength / processes); j++) // scan each line within the childs designated index
            {
                int num;
                if (fscanf(fp, "%d", &num) == 1) // scan line and add num to blockTotal
                {
                    blockTotal += num;
                }
                else
                {
                    break;
                }
            }

            // close(fd[0]);
            write(fd[i][1], &blockTotal, sizeof(int));
            fclose(fp);
            exit(0);
        }
    }

    wait(NULL); //wait for child processes
    //read child process data
    for(int i =0; i < processes; i++){
        // close(fd[1]);
        int childTotal;
        read(fd[i][0], &childTotal, sizeof(int));
        // close(fd[0]);
        printf("\nChild %d sum is: %d", i, childTotal);

        finalNum += childTotal;
    }
    printf("\nTotal sum is: %d", finalNum);

    gettimeofday(&current_time, NULL);
    timer = current_time.tv_usec - timer;

    printf("\nTime taken (microseconds): %ld", timer);

    return 0;
}
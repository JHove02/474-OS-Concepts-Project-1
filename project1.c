#include <stdio.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{

    bool loop = true;
    int process;
    FILE *fp;
    int fileLength = 0;
    int fd[2];

    pipe(fd);

    while (loop)
    {

        // input for # of processes
        printf("Enter the number 1, 2, or 4 for the number of processes to run\n");

        scanf("%d", &process);
        if (process == 1 || process == 2 || process == 4)
        {
            loop = false;
        }
        else
        {
            printf("\n%d is not a valid process\n", process);
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

    int finalNum; // final sum of the file

    for (int i = 0; i < process; i++)
    {
        if (fork() == 0)
        {
            // open the file
            switch (file)
            {
            case 1:
                fp = fopen("P1 Data\file1.dat", "r");
                break;
            case 2:
                fp = fopen("P1 Data\file2.dat", "r");
                break;
            case 3:
                fp = fopen("P1 Data\file3.dat", "r");
                break;
            }

            int temp;
            // scan the file to find its length
            while (fscanf(fp, "%d", &temp) == 1)
            {
                fileLength += 1;
            }

            int blockSize = fileLength / process; // Size of the area each child must read
            fseek(fp, blockSize * i, SEEK_SET);   // jump to an offset in the file so the child can scan that section

            int blockTotal;                     // Sum of the block each process reads
            for (int j = 0; j < blockSize; i++) // scan each line within the childs designated index
            {
                int num;
                if (fscanf(fp, "%d", &num)) // scan line and add num to blockTotal
                {
                    blockTotal += num;
                }
                else
                {
                    break;
                }
            }
            printf("\nChild %d sum is: %d", i, blockTotal); // DOES NOT CURRENTYL WORK

            close(fd[0]);
            write(fd[1], &blockTotal, sizeof(int));
            close(fd[1]);
        }
    }
    close(fd[1]);
    read(fd[0], &finalNum, sizeof(finalNum));
    close(fd[0]);
    printf("\nTotal sum is: %d", finalNum);

    return 0;
}
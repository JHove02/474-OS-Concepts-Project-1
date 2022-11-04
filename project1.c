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
   
    int fileLength = 0;
    

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

    int finalNum = 0; // final sum of the file

    for (int i = 0; i < process; i++)
    {
        int fd[2];
        if(pipe(fd)==-1){
                return -1;
        }
        if (fork() == 0)
        {
            
             FILE *fp;// open the file
            switch (file)
            {
            case 1:
                fp = fopen("file1.dat", "r");
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
            
            if(fp == NULL){
                printf("File not located\n");
                return -1;
            }

            int offset = (i / (float)process) * fileLength * 5; // Size of the area each child must read
            printf("blocksize, %d\n", offset);
            fseek(fp, offset, SEEK_SET);   // jump to an offset in the file so the child can scan that section
            
            int blockTotal = 0;                     // Sum of the block each process reads
            for (int j = 0; j < (fileLength/process); j++) // scan each line within the childs designated index
            {
                int num;
                if (fscanf(fp, "%d", &num)==1) // scan line and add num to blockTotal
                {
                    blockTotal += num;
                }
                else
                {
                    break;
                }
            }

            //close(fd[0]);
            write(fd[1], &blockTotal, sizeof(int));
            fclose(fp);
            exit(0);
        }
        else{
            //close(fd[1]);
            int childTotal;
            read(fd[0], &childTotal, sizeof(int));
            //close(fd[0]);
            printf("\nChild %d sum is: %d", i, childTotal);
            
            finalNum += childTotal;
            printf("\nFinal num: %d\n", finalNum);
        }
    }

    printf("\nTotal sum is: %d", finalNum);

    return 0;
}
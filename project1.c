#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

int main()
{

    bool loop = true;
    int process;
    FILE *fp;
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
            printf("\n %d is not a valid process", process);
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
            printf("\n %d is not a valid file", file);
        }
    }

    if (fork() == 0)
    {

        
        int finalNum;
        for (int i = 0; i < process; i++)
        {

            switch (file)
            {
            case 1:
                fp = fopen("file1.dat", "r");
                break;
            case 2:
                fp = fopen("file2.dat", "r");
                break;
            case 3:
                fp = fopen("file3.dat", "r");
                break;
            }

            int temp;
            while (fscanf(fp, "%d", &temp) == 1)
            {
                fileLength += 1;
            }
            
            int blockSize = fileLength / process;
            fseek(fp, blockSize * i, SEEK_Set);
            
            int blockTotal = 0;
            for(int j = 0; j < blockSize; i++){
                int num;
                fscanf(fp, "%d", &num);
                num += blockTotal;
            }
        }
    }

    return 0;
}
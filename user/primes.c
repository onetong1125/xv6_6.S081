#include "kernel/types.h"
#include "user/user.h"

void findPrime(int p, int lastPipe[]);
int nextPrime(int p);

int
main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }

    int pipeF2Two[2];
    pipe(pipeF2Two);

    if (fork() == 0) {
        close(pipeF2Two[0]);
        //write 2~35
        for (int i = 2; i < 36; i++) {
            write(pipeF2Two[1], &i, 4);
        }
        close(pipeF2Two[1]);

        int sts;
        wait(&sts);
        if (sts == 0) 
        {
            exit(0);
        }
        exit(1);
    }
    else {
        close(pipeF2Two[1]);

        int num;

        int pipeTwo2Three[2];
        pipe(pipeTwo2Three);

        while (read(pipeF2Two[0], &num, 4) != 0)
        {
            if (num == 2)
            {
                printf("prime %d\n", num);
            }
            else if (num % 2 != 0)
            {
                write(pipeTwo2Three[1], &num, 4);
            } 
        }

        close(pipeF2Two[0]);
        findPrime(3, pipeTwo2Three);

        exit(0);
    }
}

void findPrime(int p, int lastPipe[]) {
    if (fork() == 0)
    {
        close(lastPipe[0]);
        close(lastPipe[1]);
        int sts;
        wait(&sts);
        if (sts == 0) 
        {
            exit(0);
        }
        exit(1);
    }
    else {
        close(lastPipe[1]);

        int num;

        int nextPipe[2];
        pipe(nextPipe);

        while (read(lastPipe[0], &num, 4) != 0)
        {
            if (num == p)
            {
                printf("prime %d\n", num);
            }
            else if (num % p != 0)
            {
                write(nextPipe[1], &num, 4);
            } 
        }

        close(lastPipe[0]);
        if (nextPrime(p) != 0)
        {
            findPrime(nextPrime(p), nextPipe);
        }
        else
        {
            exit(0);
        }
        
    }
}

int nextPrime(int p)
{
    if (p == 3)
    {
        return 5;
    }
    else if (p == 5)
    {
        return 7;
    }
    else if (p == 7)
    {
        return 11;
    }
    else if (p == 11)
    {
        return 13;
    }
    else if (p == 13)
    {
        return 17;
    }
    else if (p == 17)
    {
        return 19;
    }
    else if (p == 19)
    {
        return 23;
    }
    else if (p == 23)
    {
        return 29;
    }
    else if (p == 29)
    {
        return 31;
    }
    else
    {
        return 0;
    }
    
    
    
}
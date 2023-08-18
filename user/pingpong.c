#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: pingpong\n");
        exit(1);
    }

    int pipeF2C[2];
    int pipeC2F[2];
    pipe(pipeF2C);
    pipe(pipeC2F);

    if (fork() == 0) {
        //ping
        close(1);
        dup(pipeF2C[1]);
        write(1, "i", 1);
        close(pipeF2C[1]);
        close(pipeF2C[0]);
        //pong
        close(0);
        dup(pipeC2F[0]);
        char c;
        read(0, &c, 1);
        fprintf(2, "%d: received pong\n", getpid());
    }
    else {
        //ping
        close(0);
        dup(pipeF2C[0]);
        char c;
        read(0, &c, 1);
        fprintf(2, "%d: received ping\n", getpid());
        //pong
        close(1);
        dup(pipeC2F[1]);
        write(1, "o", 1);
        close(pipeC2F[1]);
        close(pipeC2F[0]);
        sleep(3);
    }

    exit(0);
}
#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char* argv[])
{
    if(argc != 2){
        fprintf(2, "Usage: sleep integer...\n");
        exit(1);
    }
    int t;
    t = atoi(argv[1]);
    sleep(t);
    fprintf(2, "(nothing happens for a little while)\n");
    exit(0);
}
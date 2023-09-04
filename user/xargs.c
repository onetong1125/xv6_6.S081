#include "kernel/types.h"
#include "user/user.h"

#include "kernel/param.h"

char* getnewline(char *buf, int max);

int
main(int argc, char *argv[])
{
    char *commbuf[MAXARG+1], p;
    char buf[512];
    int i;
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs ...");
        exit(1);
    }

    for (i = 0; i < argc-1; i++)
        {
            commbuf[i] = malloc(sizeof p);
            commbuf[i] = strcpy(commbuf[i], argv[i+1]);
        }

    getnewline(buf, sizeof buf);
    while (strlen(buf) > 0)
    {
        if(argc + 1> sizeof commbuf)
        {
            fprintf(2, "xargs: command too long\n");
            exit(1);
        }  
        
        // add arguments to the command
        // for (int j = 0; j < words(buf); j++,i++)
        // {
        //     strcpy(commbuf[i], getword(buf, j));
        // }
        // strcpy(commbuf[i], buf);
        // printf("%s\n", commbuf[i]);
        commbuf[i] = malloc(sizeof p);
        commbuf[i] = strcpy(commbuf[i], buf);

        if (fork()==0)
        {
            exec(argv[1], commbuf);
            exit(0);
        }
        else
        {
            int st;
            wait(&st);
            getnewline(buf, sizeof buf);
        }
    
    }
    exit(0);
 
}

char*
getnewline(char *buf, int max)
{
    int i, cc;
    char c;

    for(i=0; i+1 < max; ){
        cc = read(0, &c, 1);
        if(cc < 1)
        break;
        if(c == '\n' || c == '\r')
        break;
        buf[i++] = c;
    }
    buf[i] = '\0';
    return buf;
}


#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(char *path, char *name);

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *path,  char *name)
{
    char buf[512] ,*p;
    char namebuf[DIRSIZ+1];
    char *cdir = ".";
    char *udir = "..";
    char cbuf[DIRSIZ+1];
    char ubuf[DIRSIZ+1];
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    memmove(namebuf, name, strlen(name));
    memset(namebuf+strlen(name), ' ', DIRSIZ-strlen(name));
    memmove(cbuf, cdir, strlen(cdir));
    memset(cbuf+strlen(cdir), ' ', DIRSIZ-strlen(cdir));
    memmove(ubuf, udir, strlen(udir));
    memset(ubuf+strlen(udir), ' ', DIRSIZ-strlen(udir));

    switch(st.type) {
    case T_FILE:
        if (strcmp(namebuf, fmtname(path))==0) {
            printf("%s\n", path);
        }
        break;
    
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }  
        strcpy(buf, path);
        p = buf+strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if ((st.type == T_DIR)&&(strcmp(cbuf, fmtname(buf))!=0)&&(strcmp(ubuf, fmtname(buf))!=0)) {
                find(buf, name);
            }
            // printf("%s %d\n", fmtname("."),strcmp(fmtname("."), fmtname(buf)));
            if (strcmp(namebuf, fmtname(buf))==0) {
                printf("%s\n", buf);
            }
        }
        break;   
    }
    close(fd);

}

int
main(int argc, char *argv[])
{
    if (argc < 3){
        fprintf(2, "Usage: find [dirname] [filename]");
        exit(1);
    }
    for (int i = 2 ; i < argc; i++){
        find(argv[1], argv[i]);
    }

    exit(0);
}
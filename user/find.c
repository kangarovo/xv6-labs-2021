#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char* path, char* target) {
    struct stat st;
    struct dirent de;
    char buf[64], * p;
    char* last;

    int fd = open(path, O_RDONLY);
    fstat(fd, &st);

    switch(st.type) {
    case T_FILE:
        last = path + strlen(path);
        while (last >= path && *last != '/') last--;
        last++;

        if(strcmp(last, target) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(path);
        *p ++ = '/';

        while(read(fd, &de, sizeof de) > 0) {
            if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }

            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = '\0';

            find(buf, target);
        }
        break;
    }

    close(fd);
}

int main(int argc, char* argv[]) {
    find(argv[1], argv[2]);

    exit(0);
}

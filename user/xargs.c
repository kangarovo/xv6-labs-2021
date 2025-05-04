#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    char* argv2[64];
    
    int i;
    for(i = 1; i < argc; i ++) {
        argv2[i - 1] = argv[i];
    }

    char c;
    char buf[64], * p = buf;
    while(read(0, &c, sizeof c) > 0) {
        if(c == '\n') {
            *p = '\0';
            argv2[i - 1] = buf;
            argv2[i] = 0;
            
            if(fork() == 0) {
                exec(argv[1], argv2);
            }

            p = buf;
            wait(0);
            continue;
        }

        *p ++ = c;
    }

    exit(0);
}

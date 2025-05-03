#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    int p[2], p2[2];
    pipe(p), pipe(p2);

    char buf[64];
    char msg = 'p';

    if(fork() == 0) {
        int pid = getpid();
        read(p[0], buf, sizeof buf);
        close(p[0]);
        printf("%d: received ping\n", pid);

        write(p2[1], buf, sizeof buf);
        close(p2[1]);

        close(p[1]), close(p2[0]);

        exit(0);
    }
    int pid = getpid();

    write(p[1], &msg, sizeof msg);
    close(p[1]);

    wait(0);

    read(p2[0], buf, sizeof buf);
    close(p2[0]);
    printf("%d: received pong\n", pid);

    close(p[0]), close(p2[1]);

    exit(0);
}

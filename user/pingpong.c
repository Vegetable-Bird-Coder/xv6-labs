#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int p2c[2], c2p[2];
    char buf[2];
    pipe(p2c);
    pipe(c2p);

    if (fork() == 0) {
        close(p2c[1]);
        close(c2p[0]);

        read(p2c[0], buf, 1);
        printf("%d: received ping\n", getpid());
        write(c2p[1], "o", 1);

        close(p2c[0]);
        close(c2p[1]);

        exit(0);
    } else {
        close(p2c[0]);
        close(c2p[1]);

        write(p2c[1], "i", 1);
        wait(0);
        read(c2p[0], buf, 1);

        printf("%d: received pong\n", getpid());

        close(p2c[1]);
        close(c2p[0]);
        exit(0);
    }
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int in, int prime) {
    printf("prime %d\n", prime);
    int p[2] = {0, 0};
    int num;
    while (read(in, &num, sizeof(num))) {
        if (num % prime != 0) {
            if (p[1] == 0) {
                pipe(p);
                if (fork() == 0) {
                    close(in);
                    close(p[1]);
                    sieve(p[0], num);
                    close(p[0]);
                    exit(0);
                } else {
                    close(p[0]);
                }
            }
            write(p[1], &num, sizeof(num));
        }
    }
    if (p[1] != 0) {
        close(p[1]);
    }
    close(in);
    while (wait(0) > 0);
}

int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if (fork() == 0) {
        close(p[1]);
        sieve(p[0], 2);
        close(p[0]);
        exit(0);
    } else {
        close(p[0]);
        for (int num = 2; num <= 35; num++) {
            write(p[1], &num, sizeof(num));
        }
        close(p[1]);
        while (wait(0) > 0);
    }
    exit(0);
}


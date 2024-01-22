#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    char *real_argv[MAXARG];
    for (int i = 0; i < MAXARG; i++) {
        real_argv[i] = 0;
    }

    int idx = 0;
    for (int i = 1; i < argc; i++) {
        real_argv[idx] = malloc(strlen(argv[i]) + 1);
        if (real_argv[idx] == 0) {
            exit(1);
        }
        strcpy(real_argv[idx++], argv[i]);
    }

    char buf[512];
    char *p = buf;
    while (read(0, p, 1) == 1) {
        if (*p == '\n') {
            *p = 0;
            real_argv[idx] = malloc(strlen(buf) + 1);
            if (real_argv[idx] == 0) {
                exit(1);
            }
            strcpy(real_argv[idx++], buf);
            p = buf;
        } else {
            p++;
        }
    }

    if (fork() == 0) {
        exec(argv[1], real_argv);
        exit(1);
    } else {
        wait(0);
        for (int i = 0; i < idx; i++) {
            free(real_argv[i]);
        }
        exit(0);
    }
}
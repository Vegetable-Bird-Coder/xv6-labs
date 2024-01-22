#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        char *error_info = "Usage: sleep number\n";
        write(1, error_info, strlen(error_info));
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}
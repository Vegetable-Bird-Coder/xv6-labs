#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
#include "user/user.h"

char *fmtname(char *path) {
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;

    return p;
}

void find(char *path, char *name) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find can't open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "find can't stat %s\n", path);
        close(fd);
        return;
    }

    switch (st.type) {
    case T_FILE:
        char *find_name = fmtname(path);
        if (strcmp(find_name, name) == 0) {
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0) {
                continue;
            }
            if (!(de.name[0] == '.' || (de.name[0] == '.' && de.name[1] == '.'))) {
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                if (stat(buf, &st) < 0) {
                    printf("ls: cannot stat %s\n", buf);
                    continue;
                }
                find(buf, name);
            }
        }
        break;
    default:
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: find path name\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}
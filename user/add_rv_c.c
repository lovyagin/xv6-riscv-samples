#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_LEN 15


int
main(int argc, char *argv[]) {

    char input[ BUF_LEN];

    input = gets(input, BUF_LEN);

    char *space = strchr(in, ' ');

    *space = '\0';

    char *f1 = input;
    char *f2 = space + 1;

    printf("%d\n", atoi(f1) + atoi(f2));


    exit(0);
}
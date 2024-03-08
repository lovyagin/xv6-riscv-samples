#include "kernel/types.h"
#include "user/user.h"

#define BUF_SIZE 15

void check(int n1, int n2, int i) {
    int sys_answer = add(n1, n2);
    printf((n1 + n2) == sys_answer ? "Test %d: success" : "Test %d: copy error", i);
    printf("\t numbers: %d, %d, system: %d, answer:%d \n", n1, n2, sys_answer, n1 + n2);
}

int
main(int argc, char *argv[]) {

    int n1, n2;

    for (int i = 1; i < 7; ++i) {
        n1 = (131 * i - 31) % 375;
        n2 = (239 * i + 11) % 431;
        check(n1, n2, i);
    }

    exit(0);
}

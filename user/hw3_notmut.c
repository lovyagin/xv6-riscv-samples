#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(2, "Малое количество аргементов\n");
        exit(-1);
    }
    int p1[2], p2[2];

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "Ошибка в создании pipe\n");
        exit(-2);
    }

    int pid = fork();

    if (pid < 0) {
        fprintf(2, "Ошибка при создании дочернего процесса\n");
        exit(-3);
    } else if (pid > 0) {

        close(p2[1]);
        close(p1[0]);

        if (write(p1[1], argv[1], strlen(argv[1])) < 0) {
            fprintf(2, "Ошибка в записи в канал из родительского процесса\n");
            exit(-4);
        }

        close(p1[1]);

        char c;
        int x;

        while ((x = read(p2[0], &c, 1)) > 0) {
            printf("%d: received '%c'\n", getpid(), c);
        }

        if (x < 0) {
            fprintf(2, "Ошибка чтения из канала родителским процессом\n");
            exit(-7);
        }

        close(p2[0]);

        exit(0);

    } else {

        close(p2[0]);
        close(p1[1]);

        char c;
        int x;

        while ((x = read(p1[0], &c, 1)) > 0) {
            printf("%d: received '%c'\n", getpid(), c);
            if (write(p2[1], &c, 1) < 0) {
                fprintf(2, "Ошибка записи в канал дочерним процессом полученного символа\n");
                exit(-5);
            }
        }

        if (x < 0) {
            fprintf(2, "Ошибка чтения из канала дочерним процессом\n");
            exit(-6);
        }

        close(p2[1]);
        close(p1[0]);

        exit(0);
    }
}
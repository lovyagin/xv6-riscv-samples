#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ERROR_MESSAGE(fun) fprintf(2, "Error occured during %s().\n", fun);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(2, "Малое количество аргементов\n");
        exit(-1);
    }

    int mutex = new_mutex();
    if (mutex < 0) {
        fprintf(2, "Ошибка в создании мьютекса\n");
        exit(-2);
    }

    int p1[2], p2[2];

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        fprintf(2, "Ошибка в создании pipe\n");
        exit(-3);
    }
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "Ошибка при создании дочернего процесса\n");
        exit(-4);
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
            if (acquire_mutex(mutex) < 0) {
                fprintf(2, "Ошибка в блокировке мьютекса в родительском процессе\n");
                exit(-5);
            }

            printf("%d: received '%c'\n", getpid(), c);

            if (release_mutex(mutex) < 0) {
                fprintf(2, "Ошибка в разблокировке мьютекса в родительском процессе\n");
                exit(-6);
            }
        }
        if (x < 0) {
            fprintf(2, "Ошибка чтения из канала родителским процессом\n");
            exit(-7);
        }
        close(p2[0]);

        if (free_mutex(mutex) < 0) {
            fprintf(2, "Ошибка в освобождении мьютекса родительским процессом после записи\n");
            exit(-8);
        }

        exit(0);

    } else {

        close(p2[0]);
        close(p1[1]);

        char c;
        int x;

        while ((x = read(p1[0], &c, 1)) > 0) {
            if (acquire_mutex(mutex) < 0) {
                fprintf(2, "Ошибка в блокировке мьютекса в дочернем процессе\n");
                exit(-9);
            }
            printf("%d: received '%c'\n", getpid(), c);

            if (release_mutex(mutex) < 0) {
                fprintf(2, "Ошибка в разблокировке мьютекса в дочернем процессе\n");
                exit(-10);
            }

            if (write(p2[1], &c, 1) < 0) {
                fprintf(2, "Ошибка записи в канал дочерним процессом полученного символа\n");
                exit(-11);
            }
        }
        if (x < 0) {
            fprintf(2, "Ошибка чтения из канала дочерним процессом\n");
            exit(-12);
        }

        close(p2[1]);
        close(p1[0]);

        if (free_mutex(mutex) < 0) {
            fprintf(2, "Ошибка в освобождении мьютекса дочернем процессом после записи\n");
            exit(-13);
        }

        exit(0);
    }
}
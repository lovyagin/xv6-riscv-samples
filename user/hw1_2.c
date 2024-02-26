#include "../kernel/types.h"
#include "user.h"


int main(int argc, char *argv[]) {

    int exit_code;

    if (argc < 2) {
        fprintf(2, "Ошибка количества аргументов\n");
        exit(-1);
    }

    int p[2];

    if (pipe(p) < 0) {
        fprintf(2, "Ошибка создания канала\n");
        exit(-1);
    }

    int pid = fork();

    if (pid == 0) {

        int n;
        char symb;

        close(p[1]);

        while (1) {
            n = read(p[0], &symb, 1);
            if (n <= 0) {
                break;
            }
            printf("%c", symb);
        }

        close(p[0]);

        exit(0);

    } else if (pid > 0) {

        close(p[0]);

        for (int i = 1; i < argc; i++) {
            write(p[1], argv[i], strlen(argv[i]));
            write(p[1], "\n", 1);
        }

        close(p[1]);

        wait(&exit_code);


    } else {
        fprintf(2, "Ошибка в создании дочернего процесса\n");
        exit(-1);
    }

    exit(0);
}

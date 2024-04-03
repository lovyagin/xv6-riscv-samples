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

        close(p[1]);

        close(0);
        dup(p[0]);

        close(p[0]);

        char *argv[] = {"/wc", 0};
        exec("/wc", argv);

        fprintf(2, "Ошибка в исполнении программы wc\n");
        exit(-1);

    } else if (pid > 0) {

        close(p[0]);

        for (int i = 1; i < argc; i++) {
            if (strlen(argv[i]) != write(p[1], argv[i], strlen(argv[i]))) {
                fprintf(2, "Ошибка записи в канал из родительского процесса\n");
                exit(-1);
            }
            if (strlen("\n") != write(p[1], "\n", 1)) {
                fprintf(2, "Ошибка записи в канал из родительского процесса\n");
                exit(-1);
            }
        }

        close(p[1]);

        wait(&exit_code);

    } else {
        fprintf(2, "Ошибка в создании дочернего процесса\n");
        exit(-1);
    }

    exit(0);
}

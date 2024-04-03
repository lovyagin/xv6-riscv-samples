#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/proc_info.h"
#include "user/user.h"
#include "../kernel/param.h"


int
main(int argc, char *argv[]) {

    int proc_cnt = 1;
    int count;

    const char *states[] = {
            "UNUSED", "USED", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"
    };

    struct procinfo *buffer;

    while (1) {
        buffer = malloc(sizeof(struct procinfo) * proc_cnt);

        if (buffer == 0) {
            fprintf(2, "Ошибка при выделении памяти для буфера\n");
            exit(-1);
        }

        count = listinfo(buffer, proc_cnt);

        if (count < 0) {
            free(buffer);
            fprintf(2, "Ошибка при системном вызове\n");
            exit(-1);
        }

        if (proc_cnt < count) {
            free(buffer);
            proc_cnt *= 2;
            continue;
        }

        printf("Информация о текущих процессах:\n");
        for (int i = 0; i < count; ++i) {
            printf("%d) Имя процесса - %s, состояние - %s , pid родителя - %d\n",
                   i + 1, buffer[i].name, states[buffer[i].state], buffer[i].parent_pid);
        }

        free(buffer);

        exit(1);
    }

    exit(-1);

}
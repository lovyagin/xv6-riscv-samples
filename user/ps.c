#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/proc_info.h"
#include "user/user.h"
#include "../kernel/param.h"


int
main(int argc, char *argv[]) {

    struct procinfo processes[NPROC];
    int count;

    const char *states[] = {
            "UNUSED", "USED", "SLEEPING", "RUNNABLE", "RUNNING", "ZOMBIE"
    };

    count = listinfo(processes, NPROC);

    if (count < 0 || count > NPROC) {
        fprintf(2, "Ошибка при системном вызове\n");
        exit(-2);
    }

    printf("Информация о текущих процессах:\n");
    for (int i = 0; i < count; ++i) {
        printf("Имя процесса - %s, состояние - %s, pid родителя - %d\n",
               processes[i].name, states[processes[i].state] , processes[i].parent_pid);
    }

    exit(0);
}
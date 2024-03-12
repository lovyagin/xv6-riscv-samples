#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/proc_info.h"
#include "user/user.h"
#include <stddef.h>


int
main(int argc, char *argv[]) {

    struct procinfo processes[20];
    int count;

    //тест корректного количества лимита и правильного буфера
    count = listinfo(processes, 20);
    if (count < 0 || count > 20) {
        fprintf(2, "Ошибка в тесте на корректных данных\n");
        exit(-2);
    }
    printf("Успех в тесте на корректных данных\n");


    //маленький лимит
    count = listinfo(processes, 1);
    if (count <= 1) {
        fprintf(2, "Ошибка в тесте с лимитом 1, было возвращено %d процессов\n", count);
        exit(-2);
    }
    printf("Успех в тесте на маленьком лимите, было возвращено %d > 1\n", count);


    // некорректный адрес
    count = listinfo((struct procinfo *)0xFFFFFFFFFFFFF000, 20);
    if (count >= 0) {
        fprintf(2, "Ошибка на некорректном адресе, было возвращено %d\n", count);
        exit(-2);
    }
    printf("Успех на некорректном адресе, было возвращено %d\n", count);


    // передача NULL
    count = listinfo((struct procinfo *)NULL, 20);
    if (count < 0) {
        fprintf(2, "Ошибка на NULL, было возвращено %d\n", count);
        exit(-2);
    }
    printf("Успех на NULL, было возвращено количество процессов %d\n", count);

    exit(0);
}
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_LEN 15

int check_string(char *buf) {
    int len = strlen(buf);
    if (len == 0) {
        return 0;
    }
    for (int i = 0; i < len; ++i) {
        if (buf[i] > '9' || buf[i] < '0') { // проверка что первое - число
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char *argv[]) {

    if (argc != 1) { // в случае если неверное количество аргументов
        printf("Error: wrong number of arguments\n");
        exit(-1);
    }

    char buf[BUF_LEN];

    int cc = 0, j = 0;
    char c;

    while (cc >= 0 && c != '\n' && c != '\r') {
        cc = read(0, &c, 1);
        if (cc < 1)
            break;
        buf[j++] = c;
        if (c == '\n' || c == '\r') {
            --j;
        }
        if (j > BUF_LEN) {
            printf("Error: buffer overflow");
            exit(-1);
        }
    }
    buf[j] = '\0';


    printf("Тестовый вывод строки:\n|");
    printf(buf);
    printf("|\n");

    int input_len = strlen(buf);
    if (input_len == 1) {
        printf("Error: empty argument\n");
        exit(-1);
    }

    char f1[BUF_LEN];
    char f2[BUF_LEN];

    int first = 0, second = 0;         // индексы в буферах чисел
    int space_was = 0, last_space = 0; // для контроля количества аргументов и парсинга


    for (int i = 0; i < input_len; ++i) {
        char x = buf[i];
        if (x == ' ') {
            if (first != 0) { // чтобы не учитывать начальные пробелы строки
                space_was = 1;
                last_space = 1;
            }
        } else {
            if (second != 0 && last_space == 1) { // в случе если больше 2 аргументов
                printf("Error: incorrect number of arguments\n");
                exit(-1);
            }
            last_space = 0;
            if (space_was == 0) {
                f1[first++] = x;
            } else {
                f2[second++] = x;
            }
        }
    }

    f1[first] = '\0';
    f2[second] = '\0';

    if (check_string(f1) == 0) {
        printf("Error: incorrect first number\n");
        exit(-1);
    }

    if (check_string(f2) == 0) {
        printf("Error: incorrect second number\n");
        exit(-1);
    }

    int number1 = atoi(f1);
    int number2 = atoi(f2);

    printf("%d\n", number1 + number2);


    exit(0);
}

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
        if ((buf[i] > '9' || buf[i] < '0') && buf[i] != ' ') { // проверка что первое - число
            return 0;
        }
    }
    return 1;
}

void normalize(char *buf) { // тк fgets сохрнаяет \n  \r в ulib.c
    int len = strlen(buf);
    for (int i = 0; i < len; ++i) {
        if (buf[i] == '\n' || buf[i] == '\r') {
            buf[i] = '\0';
            return;
        }
    }
}

int
main(int argc, char *argv[]) {

    if (argc != 1) { // в случае если неверное количество аргументов
        fprintf(2, "Error: wrong number of arguments\n");
        exit(-1);
    }

    char in[2 * BUF_LEN];
    char *buf = gets(in, 2 * BUF_LEN);
    int buf_len = strlen(buf);

    if (buf_len <= 1) {
        fprintf(2, "Error: empty argument\n");
        exit(-1);
    }

    int number1 = 0, number2 = 0;

    normalize(buf);

    if (check_string(buf) != 1) {
        fprintf(2, "Error: incorrect symbol in argument\n");
        exit(-1);
    }

    int first = 0, second = 0;         // индексы в буферах чисел
    int space_was = 0, last_space = 0; // для контроля количества аргументов и парсинга

    char cur[2] = {'\0', '\0'};

    for (int i = 0; buf[i]; ++i) {
        char x = buf[i];
        if (x == ' ') {
            if (first != 0) { // чтобы не учитывать начальные пробелы строки
                space_was = 1;
                last_space = 1;
            }
        } else {
            cur[0] = x;
            if (second != 0 && last_space == 1) { // в случе если больше 2 аргументов
                fprintf(2, "Error: incorrect number of arguments\n");
                exit(-1);
            }
            last_space = 0;
            if (space_was == 0) {
                first++;
                number1 *= 10;
                number1 += atoi(cur);
            } else {
                second++;
                number2 *= 10;
                number2 += atoi(cur);
            }
        }
        if (first == BUF_LEN || second == BUF_LEN) { // в случае переполнения буфера
            fprintf(2, "Error: too big argument\n");
            exit(-1);
        }
    }

    if (space_was == 0) {
        fprintf(2, "Error: only one argument\n");
        exit(-1);
    }

    printf("%d\n", number1 + number2);

    exit(0);
}

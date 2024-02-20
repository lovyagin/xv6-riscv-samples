// Здесь написана реализация утилиты, код которой на Си есть в файле add_rv_c
// в данной реализации нет никаких обработок ошибок и она работает только на корректном входе
// а именно "x y" из-за того, что мы не писали на ассемблере, а только единожды
// передовали в ассмблерные инструкции elf файл
#define BUF_LEN 15

.data
        print: .asciz "%d\n";
        input: .space BUF_LEN

.text
.global main

main:
    la a0, input
    li a1, BUF_LEN
    call gets

    la a0, input
    li a1, ' '
    call strchr    // разбиваем строку по пробелу
    la t6, input
    mv t5, a0
    sb zero, 0(t5) // присваиваем '\0' на конце первого числа
    addi t5, t5, 1 // двигаем указатель начала второго аргумента на 1

    mv a0, t6
    call atoi      // переводим из строки в число
    mv t3, a0
    mv a0, t5
    call atoi      // переводим из строки в число
    mv t0, a0

    add t1, t3, t0 //складываем наши числа
    la a0, print
    mv a1, t1
    call printf

    li a7, 2
    li a0, 0
    ecall
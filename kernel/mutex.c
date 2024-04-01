#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sleeplock.h"
#include "mutex.h"

// глобальная таблица мьютексов
struct mutex mutexes[NMUTEX];


// проводим инициализацию всех мьютексов
void
mutex_init(void) {
    struct mutex *mutex;
    for (mutex = mutexes; mutex < &mutexes[NMUTEX]; mutex++) {
        initlock(&mutex->spinlock, "spinlock");
        initsleeplock(&mutex->sleeplock, "sleeplock");
        mutex->used = 0;
    }
}

// аллокация свободного мьютекса и возвращение его дескриптор в случае успеха
// иначе -1, если все заняты
int
alloc_mutex(void) {
    for (int i = 0; i < NMUTEX; i++) {
        // чтобы одновременно пара процессов не могли забрать один
        // и тот же мьютекс и у него не был счетчик равен 1
        acquire(&mutexes[i].spinlock);
        if (mutexes[i].used == 0) {
            mutexes[i].used++;
            release(&mutexes[i].spinlock);
            return i;
        }
        release(&mutexes[i].spinlock);
    }
    return -1;
}

// блокируем мьютекс по его дескриптору в таблице текущего процесса
// возвращаем 0 - если блокировка успешна, -1 - если ошибка в дескрипторе
int
acquire_mutex(int descriptor) {

    if (descriptor < 0 || descriptor >= NOMUTEX)
        return -1;

    if (myproc()->mutexes[descriptor] < 0 || myproc()->mutexes[descriptor] >= NMUTEX)
        return -2;

    acquiresleep(&mutexes[myproc()->mutexes[descriptor]].sleeplock);

    return 0;
}

// освобождаем мьютекс по его дескриптору в таблице текущего процесса
// возвращаем 0 - если освобождение успешно, -1 - если ошибка в дескрипторе
int
release_mutex(int descriptor) {

    if (descriptor < 0 || descriptor >= NOMUTEX)
        return -1;

    if (myproc()->mutexes[descriptor] < 0 || myproc()->mutexes[descriptor] >= NMUTEX)
        return -2;

    releasesleep(&mutexes[myproc()->mutexes[descriptor]].sleeplock);

    return 0;
}


//   увеличиваем счетчик использования мьютекса в процессах
//   по его дескриптору в таблице текущего процесса
//  в случае ошибки дексриптора - -1, в случае успеха - 0
int
use_mutex(int descriptor) {

    if (descriptor < 0 || descriptor >= NOMUTEX)
        return -1;

    if (myproc()->mutexes[descriptor] < 0 || myproc()->mutexes[descriptor] >= NMUTEX)
        return -2;

    descriptor = myproc()->mutexes[descriptor];

    // для избежания гонки данных и корректной работы счетчика
    acquire(&mutexes[descriptor].spinlock);
    mutexes[descriptor].used += 1;
    release(&mutexes[descriptor].spinlock);

    return 0;
}

// осовобождаем мьютекс по его дескриптору таблицы текущего процесса
int
free_mutex(int descriptor) {

    if (descriptor < 0 || descriptor >= NOMUTEX)
        return -1;

    if (myproc()->mutexes[descriptor] < 0 || myproc()->mutexes[descriptor] >= NMUTEX)
        return -2;

    release_mutex(descriptor);

    int cur_desc = descriptor;
    descriptor = myproc()->mutexes[descriptor];

    acquire(&mutexes[descriptor].spinlock);

    if (mutexes[descriptor].used == 0) {
        release(&mutexes[descriptor].spinlock);
        return -3;
    }

    // удаляем мьютекс из набора у данного процесса, чтобы потом
    // не было двойного освобождения и ошибки
    myproc()->mutexes[cur_desc] = -1;
    mutexes[descriptor].used--;
    release(&mutexes[descriptor].spinlock);
    return 0;
}
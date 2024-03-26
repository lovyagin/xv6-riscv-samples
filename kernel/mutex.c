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

// блокируем мьютекс по его дескриптору
// возвращаем 0 - если блокировка успешна, -1 - если ошибка в дескрипторе
int
acquire_mutex(int descriptor) {

    if (descriptor < 0 || descriptor >= NMUTEX)
        return -1;

    acquire(&mutexes[descriptor].spinlock);
    acquiresleep(&mutexes[descriptor].sleeplock);
    release(&mutexes[descriptor].spinlock);
    return 0;
}

// освобождаем мьютекс по его дескриптору
// возвращаем 0 - если освобождение успешно, -1 - если ошибка в дескрипторе
int
release_mutex(int descriptor) {

    if (descriptor < 0 || descriptor >= NMUTEX)
        return -1;

    acquire(&mutexes[descriptor].spinlock);
    releasesleep(&mutexes[descriptor].sleeplock);
    release(&mutexes[descriptor].spinlock);

    return 0;
}


//   увеличиваем счетчик использования мьютекса в процессах
//  в случае ошибки дексриптора - -1, в случае успеха - 0
int
use_mutex(int descriptor) {

    if (descriptor < 0 || descriptor >= NMUTEX)
        return -1;

    // для избежания гонки данных и корректной работы счетчика
    acquire(&mutexes[descriptor].spinlock);
    mutexes[descriptor].used++;
    release(&mutexes[descriptor].spinlock);

    return 0;
}

// делаем проверку того, что процесс держит этот мьютекс
int
holding_mutex(int descriptor) {
    if (descriptor < 0 || descriptor >= NMUTEX)
        return -1;
    int ans;
    acquire(&mutexes[descriptor].spinlock);
    ans = holdingsleep(&mutexes[descriptor].sleeplock);
    release(&mutexes[descriptor].spinlock);
    return ans;
}

// осовобождаем мьютекс по его дескриптору
// успех - 0
// коды ошибок:
// -1 - неверный дескриптор, -2 - ошибка освобождения,
// -3 - его никто не использует
int
free_mutex(int descriptor) {

    int holds = holding_mutex(descriptor);
    int rel_code = release_mutex(descriptor);
    if (holds < 0)
        return -1;
    if (rel_code < 0)
        return -2;

    acquire(&mutexes[descriptor].spinlock);

    if (mutexes[descriptor].used == 0) {
        release(&mutexes[descriptor].spinlock);
        return -3;
    }

    mutexes[descriptor].used--;
    release(&mutexes[descriptor].spinlock);

    return 0;
}
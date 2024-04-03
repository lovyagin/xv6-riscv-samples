#ifndef XV6_RISCV_PRACTICE_HOMEWORK_MUTEX_H
#define XV6_RISCV_PRACTICE_HOMEWORK_MUTEX_H


struct mutex {
    uint64 used;
    struct sleeplock sleeplock;
    struct spinlock  spinlock;
};


#endif //XV6_RISCV_PRACTICE_HOMEWORK_MUTEX_H

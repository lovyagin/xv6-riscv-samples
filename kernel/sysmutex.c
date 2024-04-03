#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_new_mutex() {
    int description = alloc_mutex();
    if (description < 0) {
        return -1;
    }
     return add_new_mutex(description);
}

uint64
sys_acquire_mutex() {
    int description;
    argint(0, &description);
    return acquire_mutex(description);
}

uint64
sys_release_mutex() {
    int description;
    argint(0, &description);
    return release_mutex(description);
}

uint64
sys_free_mutex() {
    int description;
    argint(0, &description);
    return free_mutex(description);
}
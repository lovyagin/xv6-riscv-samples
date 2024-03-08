#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"


// Sysadd function code
uint64 sys_add(void) {

    int first, second;

    argint(0, &first);
    argint(1, &second);

    return first + second;

}
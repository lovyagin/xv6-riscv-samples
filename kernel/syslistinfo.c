#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"


//sys_listinfo
uint64 sys_listinfo(void){
    uint64 plist;
    int limit;

    argaddr(0, &plist);
    argint(2, &limit);

    return ps_listinfo((struct procinfo *)plist, limit);
}
#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"

#define BUF_LEN 50

// Syscall function code
uint64 sys_scpy(void)
{

    uint64 dst;
    argaddr(0, &dst);  // first argument: dst address

    char buf[BUF_LEN]; // kernel buffer for source string
    int len;
    len = argstr(1, buf, BUF_LEN); // second argument: source string
    if (len < 0) return -1;        // error code, src string invalid

    int size;
    argint(2, &size);              // third argument: dst buffer length
    
    if (len + 1 > size) return -2; // error code, dst buffer too small
    
    int ret = copyout(myproc()->pagetable, dst, buf, len + 1);
    if (ret < 0) return -3;        // error code, dst buffer invalid

    return 0;                      // Success
}
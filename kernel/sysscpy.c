#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "errno.h"

#define BUF_LEN 50

// Syscall function code
uint64 sys_scpy(void)
{
    uint64 dst;
    argaddr(0, &dst);                        // first argument: dst address
    if (dst == 0) return -ENULL;             // Check for NULL, error code, dst buffer invalid

    uint64 src;
    argaddr(1, &src);                        // second argument: src address, just to check for NULL
    if (src == 0) return -ENULL;             // error code: src buffer invalid

    char buf[BUF_LEN];                       // kernel buffer for source string
    int ret;                                 // copy return status
    char *d = buf;                           // write source string here
    do
    {
        // byte-by-byte copy
        ret = copyin(myproc()->pagetable, d, src++, 1);
    }
    while (ret >= 0 && *d++ != '\0'
           && d < buf + BUF_LEN);            // until '\0' received or buffer exceeded

    if (ret < 0) return -EFAULT;             // error code, src string invalid
    if (d >= buf + BUF_LEN) return -ENOMEM;  // not enough kernel buffer size

    int len = d - buf;                       // bytes copied

    int size;
    argint(2, &size);                        // third argument: dst buffer length

    if (len > size) return -ESIZE;           // error code, dst buffer too small

    ret = copyout(myproc()->pagetable, dst, buf, len);
    if (ret < 0) return -EFAULT;             // error code, dst buffer invalid

    return 0;                                // Success
}

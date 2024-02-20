#include "types.h"
#include "riscv.h"
#include "defs.h"

// Syscall function code
uint64 sys_hello(void)
{
    printf("Hello, world (system)\n"); // Print message
    return 0;                          // Return code (nothing to check here)
}

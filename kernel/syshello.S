# RISC-V hello
#
#   void sys_hello(void);
# 

fmt: .asciz "%s"
msg: .asciz "Hello, world (system)\n"

.globl sys_hello
sys_hello:

    addi sp, sp, -8  # push to stack:
    sd ra, 0(sp)     # save ra pointer
    
    la a0, msg       # format string
    la a1, msg       # first field    
    la t0, printf    # printf kernel function address
    jalr ra, t0, 0   # call printf
    
    ld ra, 0(sp)     # restore ra pointer:
    addi sp, sp, 8   # pop from stack
    
    ret              # return from sys_hello

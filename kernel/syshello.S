# RISC-V hello
#
#   void sys_hello(void);
# 

fmt: .asciz "%s"
msg: .asciz "Hello, world (system)\n"

.globl sys_hello
sys_hello:

    addi sp, sp, -16 # push to stack 2 elements:
    sd ra, 0(sp)     # save ra pointer,
    sd s1, 8(sp)     # save s1 register

    
    la a0, msg       # format string
    la a1, msg       # first field    
    la s1, printf    # printf kernel function address
    jalr ra, s1, 0   # call printf
    
    ld ra, 0(sp)     # restore ra pointer,
    ld s1, 8(sp)     # restore s1 register:
    addi sp, sp, 16  # pop from stack (restore stack pointer)
    
    ret              # return from sys_hello

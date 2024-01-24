.data                         # Data section
msg_wrt: .asciz "Hello, world (write)\n"   # Message string for write
msg_fmt: .asciz "Hello, world (printf)\n"  # Message string for printf

.text                         # Text (code) section
.global main

main:
    li a7, 16                 # Write syscall number
    li a0, 1                  # stdout index (1)
    la a1, msg_wrt            # Message string address 
    li a2, 21                 # Message string length
    ecall                     # Syscall

    la a0, msg_fmt            # First argument - format string
    call printf               # Call printf (external symbol)

    li a7, 2                  # Exit system call
    li a0, 0                  # Exit status
    ecall                     # Syscall

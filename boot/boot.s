.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set VIDEO,    1<<2
.set FLAGS,    ALIGN | MEMINFO | VIDEO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.long 0
.long 640
.long 480
.long 32

.section .text
.global _start
.type _start, @function
_start:
    cli
    push %ebx
    push %eax
    call kmain

.hang:
    hlt
    jmp .hang
.size _start, . - _start

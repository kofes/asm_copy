; void asm_copy(void* output, void* source, size_t size);

; x64 Linux:
; 1. From left to right, pass as many parameters as will fit in registers. The order in which registers are allocated, are:
; - For integers and pointers, rdi, rsi, rdx, rcx, r8, r9.
; - For floating-point (float, double), xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7.

; 2. Additional parameters are pushed on the stack, right to left, and are to be removed by the caller after the call.

; 3. After the parameters are pushed, the call instruction is made, so when the called function gets control, the return address is at [rsp], the first memory parameter is at [rsp+8], etc.

; 4. The stack pointer rsp must be aligned to a 16-byte boundary before making a call. Fine, but the process of making a call pushes the return address (8 bytes) on the stack, so when a function gets control, rsp is not aligned. You have to make that extra space yourself, by pushing something or subtracting 8 from rsp.

; 5. The only registers that the called function is required to preserve (the calle-save registers) are: rbp, rbx, r12, r13, r14, r15. All others are free to be changed by the called function.

; 6. The callee is also supposed to save the control bits of the XMCSR and the x87 control word, but x87 instructions are rare in 64-bit code so you probably don’t have to worry about this.

; 7. Integers are returned in rax or rdx:rax, and floating point values are returned in xmm0 or xmm1:xmm0.

; rdi -> output
; rsi -> source
; rdx -> size

global asm_copy_u8, asm_copy_u16, asm_copy_u32, asm_copy_u64, asm_copy_u128, asm_copy_u256, asm_copy_u512
section .text

asm_copy_u8:
    mov rcx, rdx; size -> rcx
    rep movsb; copy
    ret

asm_copy_u16:
    mov rcx, rdx; size -> rcx
    rep movsw
    ret

asm_copy_u32:
    mov rcx, rdx; size -> rcx
    rep movsd
    ret

asm_copy_u64:
    mov rcx, rdx; size -> rcx
    rep movsq
    ret

asm_copy_u128:
    mov rcx, rdx; size -> rcx
    xor rdx, rdx; counter -> rdx
u128_copy_loop:
    cmp rdx, rcx
    jge u128_copy_end
    mov rax, rdx
    shl rax, 4
    movdqa xmm1, [rsi+rax]
    movdqa [rdi+rax], xmm1
    add rdx, 1
    jmp u128_copy_loop
u128_copy_end:
    xor rax, rax
    ret

asm_copy_u256:
    mov rcx, rdx; size -> rcx
    xor rdx, rdx; counter -> rdx
u256_copy_loop:
    cmp rdx, rcx
    jge u256_copy_end
    mov rax, rdx
    shl rax, 5
    vmovdqu ymm1, [rsi+rax]
    vmovdqu [rdi+rax], ymm1
    add rdx, 1
    jmp u256_copy_loop
u256_copy_end:
    xor rax, rax
    ret

asm_copy_u512:
    mov rcx, rdx; size -> rcx
    xor rdx, rdx; counter -> rdx
u512_copy_loop:
    cmp rdx, rcx
    jge u512_copy_end
    mov rax, rdx
    shl rax, 6
    vmovupd zmm1, [rsi+rax]
    vmovupd [rdi+rax], zmm1
    add rdx, 1
    jmp u512_copy_loop
u512_copy_end:
    xor rax, rax
    ret
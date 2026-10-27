; Smart Traffic Light - 64-bit NASM Assembly
; Linux / WSL - NASM x86-64
;
; Priority Score = vehicleCount * weight + waitTime
; Green Time    = baseTime + priorityScore
; Emergency vehicle gets an additional bonus.

global _start

section .data
    vehicleCount dq 5
    weight       dq 2
    waitTime     dq 20
    baseTime     dq 30
    emergencyFlag dq 1
    emergencyBonus dq 20

    priorityScore dq 0
    greenTime     dq 0

    msg1 db "Emergency Vehicle Detected", 10
    len1 equ $ - msg1

    msg2 db "Priority Score = "
    len2 equ $ - msg2

    msg3 db "Green Time = "
    len3 equ $ - msg3

    newline db 10

section .bss
    buffer resb 32

section .text

_start:

    ; priorityScore = vehicleCount * weight + waitTime
    mov rax, [vehicleCount]
    imul rax, [weight]
    add rax, [waitTime]
    mov [priorityScore], rax

    ; greenTime = baseTime + priorityScore
    mov rbx, [baseTime]
    add rbx, [priorityScore]

    ; Check emergency flag
    cmp qword [emergencyFlag], 1
    jne normal_vehicle

    ; Emergency branch
    add rbx, [emergencyBonus]

    mov rax, 1
    mov rdi, 1
    mov rsi, msg1
    mov rdx, len1
    syscall

normal_vehicle:

    mov [greenTime], rbx

    ; Print Priority Score
    mov rax, 1
    mov rdi, 1
    mov rsi, msg2
    mov rdx, len2
    syscall

    mov rax, [priorityScore]
    call print_number

    ; Print Green Time
    mov rax, 1
    mov rdi, 1
    mov rsi, msg3
    mov rdx, len3
    syscall

    mov rax, [greenTime]
    call print_number

    mov rax, 60
    xor rdi, rdi
    syscall


; -----------------------------------------
; print_number
; Input: RAX = unsigned integer
; -----------------------------------------
print_number:
    lea rsi, [buffer + 31]
    mov byte [rsi], 10
    mov rcx, 10
    dec rsi

.convert:
    xor rdx, rdx
    div rcx
    add dl, '0'
    mov [rsi], dl
    dec rsi
    test rax, rax
    jnz .convert

    inc rsi
    lea rdx, [buffer + 32]
    sub rdx, rsi

    mov rax, 1
    mov rdi, 1
    syscall
    ret

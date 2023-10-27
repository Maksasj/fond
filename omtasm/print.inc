.model small
.stack 100h

include stack.inc

.data

.code

print_ptr_m MACRO message:REQ
    push_all_m

    mov cx, 09h
    mov ah, 09h
    mov dx, message
    int 21h

    pop_all_m
ENDM

print_m MACRO message:REQ
    print_ptr_m <offset message>
ENDM;
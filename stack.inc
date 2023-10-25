.model small
.stack 100h

.data

.code

push_all_m MACRO
    push ax
    push bx
    push cx
    push dx

    push bp
ENDM

pop_all_m MACRO
    pop bp
    
    pop dx
    pop cx
    pop bx
    pop ax
ENDM

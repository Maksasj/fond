.model small
.stack 100h

.data

.code

push_all_m MACRO
    push ax ; ah ~ al
    push bx ; bh ~ bl
    push cx ; ch ~ cl
    push dx ; dh ~ dl

    push bp
    push di
    push si
ENDM

pop_all_m MACRO
    pop si
    pop di
    pop bp
    
    pop dx
    pop cx
    pop bx
    pop ax
ENDM

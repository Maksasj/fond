.model small
.stack 100h

include print.inc
include file.inc

.data

.code

scan_k:
    pop bp

    mov ah, 0Ah
    pop dx
    int 21h

    push bp 

    catch_m

    ret

scan_k_m MACRO p_buffer:REQ
    push offset p_buffer
    call scan_k
ENDM

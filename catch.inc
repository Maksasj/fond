.model small
.stack 100h

include print.inc

.data
    error_message db "Error", '$'

.code

exit_error_m MACRO
    mov ax, 4c01h
    int 21h
ENDM

exit_ok_m MACRO
    mov ax, 4c00h
    int 21h
ENDM

catch_m MACRO
    jc error
ENDM

catch:
    jc error
    ret

error:
    print_m error_message
    exit_error_m

    ret
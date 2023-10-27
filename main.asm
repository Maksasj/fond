.model small
.stack 100h

.386

include io.inc
include flow.inc
include buffer.inc

.data
    argument_buffer db 256 dup(?)
    argument_length dw 0

    arguments_delim dw 128 dup(0000h)

    delim db "'", '$'
    argg db "arg", '$'
    endl db 13, 10, '$'

.code

setup_m MACRO 
    mov dx, @data
    mov ds, dx
ENDM

start:
    setup_m

    ; Save length of command line argument to 'argument_length'
    xor cx, cx
    mov cl, es:[80h]
    mov argument_length, cx

    ; Copy command line argument to 'argument_buffer'
    copy_buffer_ptr_m<81h, <offset argument_buffer>, argument_length>

    ; Shift buffer to the left, by 1 character
    shift_buffer_left_ptr_m<<offset argument_buffer>, 256, 1>
    dec argument_length

    mov arguments_delim, offset argument_buffer
    mov bx, offset arguments_delim
    mov di, 1

    parse_args:
        add bx, 4

        push bx
            sub bx, 4
            mov ax, [bx]
        pop bx

        find_byte_ptr_m<bx, ax, 256, ' '>

        mov ax, [bx]
        jmp_eql_m<ax, 0000h, break_parse_arg>

        inc di
    jmp parse_args
    break_parse_arg:

    mov cx, argument_length

    mov bx, offset arguments_delim
    
    for_m<si, 0, for_each_arg>
        mov ax, [bx]

        add bx, 2
        count_bytes_until_ptr_m<bx, ax, cx, ' '>
        mov ax, [bx]

        sub bx, 2

        mov bp, [bx]

        print_m<delim>
        write_file_ptr_m<bp, 0001h, ax>
        print_m<delim>

        print_m<endl>        

        sub cx, ax
        dec cx

        add bx, 4
    for_end_m<si, di, for_each_arg>
    
    ; Todo add split buffer by delim procedure

    exit_ok_m
end start

.model small
.stack 100h

include io.inc
include flow.inc
include buffer.inc

.data
    argument_buffer db 256 dup(?)
    argument_length dw 0

    arguments_delim dw 128 dup(0000h)

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

    mov arguments_delim, offset argument_buffer
    mov bx, offset arguments_delim

    parse_args:
        write_file_ptr_m<[bx], 1, 3>
        print_m<endl>

        add bx, 4

        push bx
            sub bx, 4
            mov ax, [bx]
        pop bx

        find_byte_ptr_m<bx, ax, 256, ' '>

        mov ax, [bx]
        jmp_eql_m<ax, 0000h, break_parse_arg>

    jmp parse_args
    break_parse_arg:

    for_each_arg:

            

    jmp for_each_arg
    break_for_each_arg:

    
    ; Todo add split buffer by delim procedure

    exit_ok_m
end start

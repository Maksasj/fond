.model small
.stack 100h

include io.inc
include flow.inc
include buffer.inc

.data
    argument_buffer db 128 dup(?)
    argument_length dw 0

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
    shift_buffer_left_ptr_m<<offset argument_buffer>, 128, 1>

    ; Print buffer
    write_file_m <argument_buffer, 1, argument_length>
    
    ; Todo add split buffer by delim procedure

    exit_ok_m
end start

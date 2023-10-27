.model small
.stack 100h

.386

include io.inc
include flow.inc
include buffer.inc
include data.inc

.data
    ; Argument things
    byte_buffer_m<argument_buffer, 256>
    argument_length dw 0

    arguments_delim dw 128 dup(0000h)
    arguments_count dw 0

    ; File thing
    dta_m<search_dta>
    ; search_template db 128 dup(0)

    ; search_template db 'test\*.*', 0  ; Search template for find first/next

    search_template db 128 dup('#')  ; Search template for find first/next
    any_file_search_template db "\*.*", 0

    ; Some const string, just for convenience
    delim db "'", '$'
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
    copy_es_buffer_ptr_m<81h, <offset argument_buffer>, argument_length>

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
    
    for_m<si, 0, for_each_arg_post>
        mov ax, [bx]

        add bx, 2
        count_bytes_until_ptr_m<bx, ax, cx, ' '>
        mov ax, [bx]

        sub bx, 2

        mov bp, [bx]

        ; bp - pointer to string
        ; ax - length of string
        print_m<delim>
        write_file_ptr_m<bp, 0001h, ax>
        print_m<delim>
        print_m<endl>

        sub cx, ax
        dec cx

        add bx, 4
    for_end_m<si, di, for_each_arg_post>

    mov arguments_count, di
    mov bx, offset arguments_delim

    ; Skip searchable word
    add bx, 4

    dec di

    for_m<si, 0, for_each_arg>
        mov bp, [bx]
        mov ax, [bx + 2]

        copy_buffer_ptr_m<bp, <offset search_template>, ax>

        push bx

        mov bx, offset search_template
        add bx, ax

        copy_buffer_ptr_m<<offset any_file_search_template>, bx, 5>

        pop bx

        mov cx, ax
        add cx, 5

        setup_dta_m<offset search_dta>
        find_first_file_m<search_template, done_search>

        search_next_file:
            write_file_ptr_m<<offset search_dta + 30>, 0001h, 000Ch> ; 000D - 13 bytes
            print_m<endl>

            find_next_file_m<done_search>
        
            jmp search_next_file

        done_search:
        
        add bx, 4
    for_end_m<si, di, for_each_arg>

    exit_ok_m
end start

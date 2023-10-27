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
    search_template db 128 dup('#')  ; Search template for find first/next
    any_file_search_template db "\*.*", 0

    ; Comparison buffer
    active_file_handle dw 0

    active_file_name_length dw 0
    byte_buffer_m<active_file_name, 256>

    active_folder_path_length dw 0
    byte_buffer_m<active_folder_path, 256>

    active_file_path_length dw 0
    byte_buffer_m<active_file_path, 256>

    comparison_buffer db 256 dup(0000h)

    ; Some const string, just for convenience
    delim db "'", '$'
    endl db 13, 10, '$'
    processing_file_msg db "Processing file: '", '$'

.code

setup_m MACRO 
    mov dx, @data
    mov ds, dx
ENDM

process_file:
    pop bp
    push bp

    fill_buffer_ptr_m<<offset active_file_name>, 256, '$'>
    count_bytes_until_ptr_m<<offset active_file_name_length>, <offset search_dta + 30>, 256, 0000h>
    copy_buffer_ptr_m<<offset search_dta + 30>, <offset active_file_name>, active_file_name_length>

    fill_buffer_ptr_m<<offset active_file_path>, 256, 0000h>
    copy_buffer_ptr_m<<offset active_folder_path>, <offset active_file_path>, active_folder_path_length>

    mov bx, offset active_file_path
    add bx, active_folder_path_length
    mov [bx], '\'
    inc bx

    copy_buffer_ptr_m<<offset active_file_name>, bx, active_file_name_length>

    print_m<processing_file_msg>
    print_m<active_file_name>
    print_m<delim>
    print_m<endl>

    open_file_read_m<active_file_path, active_file_handle>

    ; loop_over_string:
    ;     read_file_m<bytes_read, tmp_buffer, fh_in, 512>
    ;     write_file_m<tmp_buffer, fh_out, bytes_read>
; 
    ;     jmp_not_eql_m <bytes_read, 0, loop_over_string>
    
    file_close_m<active_file_handle>

    ret

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
        
        ; fill_buffer_ptr_m<<offset active_file_name>, 256, '$'>
        copy_buffer_ptr_m<bp, <offset search_template>, ax>

        ; There we process active folder path
        ; fill_buffer_ptr_m<<offset active_folder_path>, 256, '$'>
        ; count_bytes_until_ptr_m<<offset active_folder_path_length>, <offset active_folder_path>, 256, 0000h>
        ; copy_buffer_ptr_m<bp, <offset active_folder_path>, active_folder_path_length>

        fill_buffer_ptr_m<<offset active_folder_path>, 256, '$'>
        ; count_bytes_until_ptr_m<<offset active_folder_path_length>, bp, 256, 0000h>
        copy_buffer_ptr_m<bp, <offset active_folder_path>, ax>
        mov active_folder_path_length, ax

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
            push_all_m
            call process_file
            pop_all_m

            find_next_file_m<done_search>
        
            jmp search_next_file

        done_search:
        
        add bx, 4
    for_end_m<si, di, for_each_arg>

    exit_ok_m
end start

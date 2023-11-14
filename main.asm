.model small
.stack 100h

.386

include omtasm.inc

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
    active_file_bytes_read dw 0

    active_file_name_length dw 0
    byte_buffer_m<active_file_name, 256>

    active_folder_path_length dw 0
    byte_buffer_m<active_folder_path, 256>

    active_file_path_length dw 0
    byte_buffer_m<active_file_path, 256>

    found_match_boolean db 0
    comparison_buffer db 256 dup('#')

    ; Progress
    file_match_progress_msg db 16 dup(0000h)
    progress_read_bytes dw 0, 0

    ; Some const string, just for convenience
    endl db 13, 10, '$'
    stick db '/'
    processing_file_msg db "[Processing] '"
    scanned_msg db "', scanned "
    bytes_endl_msg db " bytes$", 13, 10, '$'

    match_prefix_msg db "[Match] "
    index_prefix_msg db "[Index] "

    match_msg db "found in '"
    at_byte_msg db "' at byte "
    clear_line_msg db 80 dup(0)

    erase_string db 13, '$'

    first_line_msg db "[Startup] FOND 0.1v just have started"
    checking_msg db "[Index] Checking '"
    index_file_msg db "' index file"
    index_file_checked db "[Index] Index file checked"
    backup_copy_file_msg db "[Index] Backuping index file"

    index_last_match_stored_msg db "[Index] Last match is already stored in index"

    ; Index file things
    index_file_path db "i.bin", '$'
    index_file_handle dw 0
    index_file_row db 256 dup('#')
    index_file_bytes_read dw 0
    
    backup_index_file_path db "ib.bin", '$'
    backup_index_file_handle dw 0

    backup_index_file_row db 256 dup('#')
    backup_index_file_bytes_read dw 0

    test_msg db "Poggers$"

.code

setup_m MACRO 
    mov dx, @data
    mov ds, dx
ENDM

copy_backup_file:
    pop bp

    write_file_m<<backup_copy_file_msg>, 1, 28>
    print_m<endl>

    print_m<test_msg>

    open_file_extended_m<index_file_path, index_file_handle>
    open_file_clear_extended_m<backup_index_file_path, backup_index_file_handle>

    backup_copy_loop:
        read_file_m<backup_index_file_bytes_read, backup_index_file_row, index_file_handle, 256>
        write_file_m<backup_index_file_row, backup_index_file_handle, backup_index_file_bytes_read>

        jmp_not_eql_m <backup_index_file_bytes_read, 0, backup_copy_loop>

    file_close_m<backup_index_file_handle>
    file_close_m<index_file_handle>

    mov index_file_handle, 0
    mov backup_index_file_handle, 0

    push bp
    
    ret

check_if_already_in_index:
    pop bp
    
    mov backup_index_file_bytes_read, 0000h

    open_file_read_m<backup_index_file_path, backup_index_file_handle>

    mov found_match_boolean, 0

    check_loop:
        read_file_m<backup_index_file_bytes_read, backup_index_file_row, backup_index_file_handle, 256>
        jmp_not_eql_m <backup_index_file_bytes_read, 256, break_check_loop>

        jmp_eql_buffer_m<<offset backup_index_file_row>, <offset index_file_row>, 256, <offset found_match_boolean>>

        cmp found_match_boolean, 1
        je break_check_loop

        jmp check_loop

    break_check_loop:

    file_close_m<backup_index_file_handle>
    mov backup_index_file_handle, 0

    push bp

    ret

save_match_to_index:
    pop bp

    fill_buffer_ptr_m<<offset index_file_row>, 256, '#'>

    mov bx, offset arguments_delim
    mov ax, [bx + 2]
    mov bx, [bx]

    ; copy searchable word to index file row
    copy_buffer_ptr_m<bx, <offset index_file_row>, ax>

    mov bx, offset index_file_row
    add bx, ax

    copy_buffer_ptr_m<<offset progress_read_bytes>, bx, 4>

    add bx, 4

    mov cx, active_file_path_length
    mov [bx], ch
    inc bx
    mov [bx], cl
    inc bx

    ; copy filepath to bx(offsert index_file_row + len(searable word))
    copy_buffer_ptr_m<<offset active_file_path>, bx, active_file_path_length>

    push_all_m
        call check_if_already_in_index
    pop_all_m

    cmp found_match_boolean, 1
    jne match_already_in_index_continue

    write_file_m<<index_last_match_stored_msg>, 1, 45>
    print_m<endl>

    jmp do_not_save_to_index

    match_already_in_index_continue:

    write_file_m<<index_file_row>, index_file_handle, 256>

    do_not_save_to_index:

    push bp

    ret

process_index_file:
    pop bp
    
    mov bx, offset arguments_delim
    mov ax, [bx + 2]
    mov bx, [bx]

    write_file_m<<checking_msg>, 1, 18>
    print_m<index_file_path>
    write_file_m<<index_file_msg>, 1, 12>
    print_m<endl>

    open_file_extended_m<index_file_path, index_file_handle>
    index_loop:
        read_file_m<index_file_bytes_read, index_file_row, index_file_handle, 256>
        jmp_not_eql_m <index_file_bytes_read, 256, break_index_loop>

        to_upper_case_buffer_ptr_m<<offset index_file_row>, 256>

        jmp_eql_buffer_m<bx, <offset index_file_row>, ax, <offset found_match_boolean>>

        cmp found_match_boolean, 1
        je found_match_but_yes

        jmp index_loop

    found_match_but_yes:
        push_all_m
            push_all_m
                mov bx, offset progress_read_bytes
                mov di, offset index_file_row
                add di, ax

                copy_buffer_ptr_m<di, bx, 4>

                add di, 4

                mov ch, [di]
                inc di
                mov cl, [di]
                inc di
                
                mov bx, offset active_file_path
                mov active_file_path_length, cx

                copy_buffer_ptr_m<di, bx, active_file_path_length>

            pop_all_m

            write_file_m<<index_prefix_msg>, 1, 8>
            call print_found_match
        pop_all_m

        jmp index_loop

    break_index_loop:

    write_file_m<index_file_checked, 1, 26>
    print_m<endl>

    push bp

    ret

print_progress:
    pop bp

    print_m<erase_string>

    write_file_m<<processing_file_msg>, 1, 14>
    write_file_m<<active_file_name>, 1, active_file_name_length>
    write_file_m<<scanned_msg>, 1, 11>

    mov ax, progress_read_bytes
    mov dx, progress_read_bytes + 2
    print_32u_m<ax, dx>

    write_file_m<<stick>, 1, 1>

    mov ax, word ptr search_dta + 28
    mov dx, word ptr search_dta + 26
    print_32u_m<ax, dx>
    
    print_m<bytes_endl_msg>

    push bp

    ret

print_found_match:
    pop bp

    write_file_m<<match_msg>, 1, 10>
    write_file_m<<active_file_path>, 1, active_file_path_length>
    write_file_m<<at_byte_msg>, 1, 10>

    mov ax, progress_read_bytes
    mov dx, progress_read_bytes + 2
    print_32u_m<ax, dx>

    print_m<endl>

    push bp

    ret

found_match:
    pop bp

    push_all_m
        print_m<erase_string>
        write_file_m<<clear_line_msg>, 1, 79>
        print_m<erase_string>
        
        write_file_m<<match_prefix_msg>, 1, 8>
        call print_found_match
    pop_all_m

    push_all_m
    call save_match_to_index
    pop_all_m

    push bp

    ret

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
    mov byte ptr [bx], '\'
    inc bx

    push ax

    mov ax, active_folder_path_length
    inc ax
    add ax, active_file_name_length

    mov active_file_path_length, ax

    pop ax

    copy_buffer_ptr_m<<offset active_file_name>, bx, active_file_name_length>
    open_file_read_m<active_file_path, active_file_handle>

    ; Todo
    fill_buffer_ptr_m<<offset file_match_progress_msg>, 16, '$'>

    mov active_file_bytes_read, 0
    mov found_match_boolean, 0

    mov si, 0

    match_loop:
        mov bx, offset arguments_delim
        mov ax, [bx + 2]
        mov bx, [bx]

        read_file_m<active_file_bytes_read, comparison_buffer, active_file_handle, ax>
        jmp_not_eql_m <active_file_bytes_read, ax, break_match_loop>

        to_upper_case_buffer_ptr_m<<offset comparison_buffer>, active_file_bytes_read>

        ; There we move pointer to the left by (word_length - 1)
        push ax
        neg ax
        inc ax
        lseek_m<active_file_handle, 0ffffh, ax>
        pop ax

        get_file_ptr_m<active_file_handle, <offset progress_read_bytes>>


        mov cx, progress_read_bytes + 2
        and cx, 007Fh ; print progress every 127 bytes
        cmp cx, 007Fh

        jne not_time_to_print

        push_all_m
        call print_progress
        pop_all_m

        not_time_to_print:

        jmp_eql_buffer_m<bx, <offset comparison_buffer>, ax, <offset found_match_boolean>>

        cmp found_match_boolean, 1
        jne match_loop


    push_all_m

    call found_match

    pop_all_m

    break_match_loop:
    
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

    to_upper_case_buffer_ptr_m<<offset argument_buffer>, 256>

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

        sub cx, ax
        dec cx

        add bx, 4
    for_end_m<si, di, for_each_arg_post>

    mov arguments_count, di
    mov bx, offset arguments_delim

    ; Skip searchable word
    add bx, 4

    dec di

    ;; There basically entry point of our program
    push_all_m

    write_file_m<<first_line_msg>, 1, 37>
    print_m<endl>

    call copy_backup_file

    call process_index_file

    pop_all_m

    ; exit_ok_m

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

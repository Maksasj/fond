.model small
.stack 100h

include stack.inc
include flow.inc
include print.inc

.data
    poggers db "huj", '$'

.code

copy_buffer_ptr_internal: 
    pop bp

    pop cx ; p_length
    pop di ; p_dst_buffer_ptr
    pop si ; p_source_buffer_ptr

    copy_loop:
        mov al, es:[si]
        mov [di], al
        inc si
        inc di
    loop copy_loop

    push bp

    ret

copy_buffer_ptr_internal_m MACRO p_source_buffer_ptr:REQ, p_dst_buffer_ptr:REQ, p_length:REQ
    push_all_m

    push p_source_buffer_ptr
    push p_dst_buffer_ptr
    push p_length

    call copy_buffer_ptr_internal

    pop_all_m
ENDM

copy_buffer_ptr_m MACRO p_bundle:REQ 
    copy_buffer_ptr_internal_m p_bundle
ENDM

shift_buffer_left_ptr_internal: 
    pop bp

    pop cx ; p_length
    pop bx ; p_buffer_ptr
    pop si ; p_bytes_shift

    ; We substract bytes shift from total length
    sub cx, si

    for_m<dx, 0, shift_loop>
        mov al, [bx + si]
        mov [bx], al

        inc bx
    for_end_m<dx, cx, shift_loop>

    push bp

    ret

shift_buffer_left_ptr_internal_m MACRO p_buffer_ptr:REQ, p_length:REQ, p_bytes_shift:REQ
    push_all_m

    push p_bytes_shift
    push p_buffer_ptr
    push p_length

    call shift_buffer_left_ptr_internal

    pop_all_m
ENDM

shift_buffer_left_ptr_m MACRO p_bundle:REQ 
    shift_buffer_left_ptr_internal_m p_bundle
ENDM

shift_buffer_left_ptr_internal_m MACRO p_buffer_ptr:REQ, p_length:REQ, p_bytes_shift:REQ
    push_all_m

    push p_bytes_shift
    push p_buffer_ptr
    push p_length

    call shift_buffer_left_ptr_internal

    pop_all_m
ENDM

shift_buffer_left_ptr_m MACRO p_bundle:REQ 
    shift_buffer_left_ptr_internal_m p_bundle
ENDM

find_byte_ptr_internal:
    pop bp

    pop ax ; p_target_ptr
    pop bx ; p_buffer_ptr
    pop dx ; p_length

    xor cx, cx
    pop cx ; p_delim

    push bp

    for_m<si, 0, split_loop>
        mov ch, [bx + si]

        cmp ch, cl
        jne continue_

        ; print_m<poggers>

        mov di, bx
        push bx
        mov bx, ax

        add di, si

        mov [bx], di

        pop bx

        ret


        continue_:
    for_end_m<si, dx, split_loop>

    ret


find_byte_ptr_internal_m MACRO p_target_ptr:REQ, p_buffer_ptr:REQ, p_length:REQ, p_byte:REQ
    push_all_m

    push p_byte
    push p_length
    push p_buffer_ptr
    push p_target_ptr

    call find_byte_ptr_internal

    pop_all_m
ENDM

find_byte_ptr_m MACRO p_bundle:REQ 
    find_byte_ptr_internal_m p_bundle
ENDM


; split_buffer_by_delim_internal:
;     pop bp
; 
;     pop ax ; p_target_buffer
;     pop bx ; p_buffer_ptr
;     pop dx ; p_length
; 
;     xor cx, cx
;     pop cx ; p_delim
; 
;     xor di, di
; 
;     push bp
; 
;     xor bp, bp
; 
;     for_m<si, 0, split_loop>
;         mov ch, [bx + si + 1]
; 
;         cmp ch, cl
;         jne continue_
;         inc di
; 
;         mov [ax + di], bx + si        
;    
;         print_m<poggers>
; 
;         continue_:
;     for_end_m<si, dx, split_loop>
; 
; 
;     ret
; 
; split_buffer_by_delim_internal_m MACRO p_target_buffer:REQ, p_buffer_ptr:REQ, p_length:REQ, p_delim:REQ
;     push_all_m
; 
;     push p_delim
;     push p_length
;     push p_buffer_ptr
;     push p_target_buffer
; 
;     call split_buffer_by_delim_internal
; 
;     pop_all_m
; ENDM
; 
; 
; split_buffer_by_delim_m MACRO p_bundle:REQ 
;     split_buffer_by_delim_internal_m p_bundle
; ENDM



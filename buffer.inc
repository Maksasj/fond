.model small
.stack 100h

include stack.inc

.data

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

.model small
.stack 100h

.data

    poggers db "huj", '$'
    matchhh db "matchhh", '$'
    dot db ".", '$'
    not_eql db "not eql", '$'

.code

copy_buffer_ptr_internal: 
    pop bp

    pop cx ; p_length
    pop di ; p_dst_buffer_ptr
    pop bx ; p_source_buffer_ptr

    push bp

    xor ax, ax

    copy_buffer_ptr_internal_loop:
        mov al, [bx]
        mov [di], al
        inc bx
        inc di
    loop copy_buffer_ptr_internal_loop

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


copy_es_buffer_ptr_internal: 
    pop bp

    pop cx ; p_length
    pop di ; p_dst_buffer_ptr
    pop si ; p_source_buffer_ptr

    push bp

    xor ax, ax

    copy_es_buffer_ptr_internal_loop:
        mov al, es:[si]
        mov [di], al
        inc si
        inc di
    loop copy_es_buffer_ptr_internal_loop

    ret

copy_es_buffer_ptr_internal_m MACRO p_source_buffer_ptr:REQ, p_dst_buffer_ptr:REQ, p_length:REQ
    push_all_m

    push p_source_buffer_ptr
    push p_dst_buffer_ptr
    push p_length

    call copy_es_buffer_ptr_internal

    pop_all_m
ENDM

copy_es_buffer_ptr_m MACRO p_bundle:REQ 
    copy_es_buffer_ptr_internal_m p_bundle
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

        mov di, bx
        push bx
        mov bx, ax

        add di, si
        inc di

        mov [bx], di

        pop bx

        ret

        continue_:
    for_end_m<si, dx, split_loop>

    ; There we set target buffer value to nullptr, akka we could not find a byte
    mov di, 0000h

    mov bx, ax
    mov [bx], di

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

count_bytes_until_ptr_internal:
    pop bp

    pop ax ; p_target_ptr
    pop bx ; p_buffer_ptr
    pop dx ; p_length

    xor cx, cx
    pop cx ; p_delim
    
    push bp

    xor di, di

    for_m<si, 0, count_bytes_until_ptr_internal_loop>
        mov ch, [bx + si]

        cmp ch, cl
        je count_bytes_until_ptr_internal_break

        inc di
    for_end_m<si, dx, count_bytes_until_ptr_internal_loop>

    count_bytes_until_ptr_internal_break:

    mov bx, ax
    mov [bx], di

    ret

count_bytes_until_ptr_internal_m MACRO p_target_ptr:REQ, p_buffer_ptr:REQ, p_length:REQ, p_byte:REQ
    push_all_m

    push p_byte
    push p_length
    push p_buffer_ptr
    push p_target_ptr

    call count_bytes_until_ptr_internal

    pop_all_m
ENDM

count_bytes_until_ptr_m MACRO p_bundle:REQ 
    count_bytes_until_ptr_internal_m p_bundle
ENDM

fill_buffer_ptr_internal:
    pop bp

    pop bx ; p_buffer
    pop dx ; p_length

    xor ax, ax
    pop ax ; p_byte

    push bp

    for_m<si, 0, fill_buffer_ptr_internal_loop>
        mov [bx], al
        inc bx
    for_end_m<si, dx, fill_buffer_ptr_internal_loop>

    ret

fill_buffer_ptr_internal_m MACRO p_buffer:REQ, p_length:REQ, p_byte:REQ
    push_all_m

    push p_byte
    push p_length
    push p_buffer
    call fill_buffer_ptr_internal

    pop_all_m
ENDM

fill_buffer_ptr_m MACRO p_bundle:REQ
    fill_buffer_ptr_internal_m p_bundle
ENDM

; jmp_eql_buffer_internal:
;     pop bp
; 
;     pop ax ; p_result
;     mov bx, ax
;     mov [bx], 0
; 
;     pop di ; p_length
;     pop bx ; p_second_buffer_ptr
;     pop dx ; p_first_buffer_ptr
; 
;     push bp
; 
;     for_m<si, 0, compare_buffer_ptr_internal_loop>
;         mov ch, [bx]
; 
;         push bx
;         mov bx, dx
; 
;         mov cl, [bx]
; 
;         pop bx
; 
;         cmp ch, cl
; 
;         inc bx
;         inc dx
; 
;         jne compare_buffer_ptr_internal_not_eql
;     for_end_m<si, di, compare_buffer_ptr_internal_loop>
; 
;     mov bx, ax
;     mov [bx], 1
;     print_m<poggers>
; 
;     compare_buffer_ptr_internal_not_eql:
; 
;     ret


jmp_eql_buffer_internal:
    pop bp

    pop ax ; p_result

    mov bx, ax
    mov byte ptr [bx], 0

    pop di ; p_length
    pop bx ; p_second_buffer_ptr
    pop dx ; p_first_buffer_ptr

    push bp

    for_m<si, 0, jmp_eql_buffer_internal_loop>
        mov ch, [bx]

        push bx
        mov bx, dx
        mov cl, [bx]
        pop bx

        cmp cl, ch
        jne compare_buffer_ptr_internal_not_eql

        inc bx
        inc dx
    for_end_m<si, di, jmp_eql_buffer_internal_loop>

    mov bx, ax
    mov byte ptr [bx], 1

    compare_buffer_ptr_internal_not_eql:

    ret

jmp_eql_buffer_internal_m MACRO p_first_buffer_ptr:REQ, p_second_buffer_ptr:REQ, p_length:REQ, p_result:REQ
    push_all_m

    push p_first_buffer_ptr
    push p_second_buffer_ptr
    push p_length
    push p_result

    call jmp_eql_buffer_internal

    pop_all_m
ENDM

jmp_eql_buffer_m MACRO p_bundle:REQ
    jmp_eql_buffer_internal_m p_bundle
ENDM

format_to_buffer_ptr_32u_internal:
    pop bp

    pop di
    pop dx
    pop ax

    mov     bx,10
    push    bx

    format_to_buffer_ptr_32u_internal_a: 
        mov     cx,ax
        mov     ax,dx
        xor     dx,dx
        div     bx
        xchg    ax,cx
        div     bx
        push    dx
        mov     dx,cx
        or      cx,ax
        jnz     format_to_buffer_ptr_32u_internal_a
        pop     dx

    format_to_buffer_ptr_32u_internal_b: 
        add     dl, '0'

        push bx

        mov bx, di
        mov [bx], dl
        inc di

        pop bx

        pop     dx
        cmp     dx,bx
        jb      format_to_buffer_ptr_32u_internal_b

    push bp

    ret

format_to_buffer_ptr_32u_internal_m MACRO p_buffer_ptr:REQ, p_high:REQ, p_low:REQ
    push_all_m

    push p_low
    push p_high
    push p_buffer_ptr

    call format_to_buffer_ptr_32u_internal        

    pop_all_m
ENDM

format_to_buffer_ptr_32u_m MACRO p_bundle:REQ
    format_to_buffer_ptr_32u_internal_m p_bundle
ENDM
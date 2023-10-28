.model small
.stack 100h

.data

.code

print_ptr_m MACRO message:REQ
    push_all_m

    mov cx, 09h
    mov ah, 09h
    mov dx, message
    int 21h

    pop_all_m
ENDM

print_m MACRO message:REQ
    print_ptr_m <offset message>
ENDM;

print_byte_m MACRO p_byte:REQ
    push_all_m

    mov ah, 02h
    mov dl, p_byte
    int 21h

    pop_all_m
ENDM

print_32u_internal:
    pop bp

    pop dx
    pop ax

    mov     bx,10          ; CONST
    push    bx             ; Sentinel

    print_32u_internal_a: 
        mov     cx,ax          ;Temporarily store LowDividend in CX
        mov     ax,dx          ;First divide the HighDividend
        xor     dx,dx          ;Setup for division DX:AX / BX
        div     bx             ; -> AX is HighQuotient, Remainder is re-used
        xchg    ax,cx          ;Temporarily move it to CX restoring LowDividend
        div     bx             ; -> AX is LowQuotient, Remainder DX=[0,9]
        push    dx             ;(1) Save remainder for now
        mov     dx,cx          ;Build true 32-bit quotient in DX:AX
        or      cx,ax          ;Is the true 32-bit quotient zero?
        jnz     print_32u_internal_a             ;No, use as next dividend
        pop     dx             ;(1a) First pop (Is digit for sure)

    print_32u_internal_b: 
        add     dl, '0'         ;Turn into character [0,9] -> ["0","9"]
        mov     ah, 02h         ;DOS.DisplayCharacter
        int     21h            ; -> AL
        pop     dx             ;(1b) All remaining pops
        cmp     dx,bx          ;Was it the sentinel?
        jb      print_32u_internal_b             ;Not yet

    push bp

    ret

print_32u_internal_m MACRO p_high:REQ, p_low:REQ
    push_all_m

    push p_low
    push p_high

    call print_32u_internal        

    pop_all_m
ENDM

print_32u_m MACRO p_bundle:REQ
    print_32u_internal_m p_bundle
ENDM
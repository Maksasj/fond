.model small
.stack 100h

.data

.code
; push offset fileHandle
; push offset fileName
; call open_file_read
open_file_read:
    pop bp ; Push adress where to return into bp register

    mov ax, 3d00h
    pop dx;
    int 21h
    
    ; Save handle to target adress
    pop bx
    mov [bx], ax

    push bp ; Put back return adress

    catch_m

    ret

open_file_read_internal_m MACRO p_file_name:REQ, p_file_handle:REQ
    push_all_m

    push offset p_file_handle
    push offset p_file_name
    call open_file_read

    pop_all_m
ENDM

open_file_read_m MACRO p_bundle:REQ
    open_file_read_internal_m p_bundle
ENDM

open_file_extended:
    pop bp ; Push adress where to return into bp register

    mov ax, 6c00h
    mov bl, 2
    mov bh, 60h
    mov cx, 00h
    mov dl, 11h
    mov dh, 0

    pop si

    int 21h
    
    ; Save handle to target adress
    pop bx
    mov [bx], ax

    push bp ; Put back return adress

    catch_m

    ret

open_file_extended_internal_m MACRO p_file_name:REQ, p_file_handle:REQ
    push_all_m

    push offset p_file_handle
    push offset p_file_name
    call open_file_extended

    pop_all_m
ENDM

open_file_extended_m MACRO p_bundle:REQ
    open_file_extended_internal_m p_bundle
ENDM




; push offset fileHandle
; push offset fileName
; call open_file_write
open_file_write:
    pop bp ; Push adress where to return into bp register

    mov ax, 3d01h
    pop dx;
    int 21h
    
    ; Save handle to target adress
    pop bx
    mov [bx], ax

    push bp ; Put back return adress

    catch_m

    ret

; push offset file_handle
; push offset file_name
; call create_file
create_file:
    pop bp ; Push adress where to return into bp register

    mov ax, 3c00h
    xor cx, cx
    pop dx;
    int 21h
    
    ; Save handle to target adress
    pop bx
    mov [bx], ax

    push bp ; Put back return adress

    catch_m

    ret

create_file_internal_m MACRO p_file_name:REQ, p_file_handle:REQ
    push_all_m

    push offset p_file_handle
    push offset p_file_name
    call create_file

    pop_all_m
ENDM

create_file_m MACRO p_bundle:REQ
    create_file_internal_m p_bundle
ENDM

; push offset bytes_read <- how many bytes read (Result)
; push offset fh_in <- Handle
; push offset tmp_buffer <- Buffer where to put out read bytes
; push 0dh <- Bytes to read
; call read_file
read_file:
    pop bp ; Push adress where to return into bp register

    mov ax, 3F00h
    pop cx ; Number of bytes to read
    pop dx ; Destination
    
    pop bx

    int 21h

    pop bx
    mov [bx], ax 

    push bp ; Put back return adress

    catch_m

    ret

read_file_internal_m MACRO p_bytes_read_out:REQ, p_buffer:REQ, p_file_handle:REQ, p_bytes_to_read:REQ
    push_all_m
    
    push offset p_bytes_read_out
    push p_file_handle
    push offset p_buffer
    push p_bytes_to_read
    call read_file

    pop_all_m
ENDM

read_file_m MACRO p_bundle:REQ
    read_file_internal_m p_bundle
ENDM

; push offset fh_out
; push offset tmp_buffer
; push bytes_read
; call write_file
write_file:
    pop bp ; Push adress where to return into bp register

    mov ax, 4000h
    pop cx ; Number of bytes to write
    pop dx ; Destination
    
    pop bx
    ;; mov bx, [bx] ; Handle of the file, there we dereference it, since previosly we have stored pointer to handle, not value

    int 21h

    push bp ; Put back return adress

    catch_m

    ret

write_file_internal_m MACRO p_buffer:REQ, p_file_handle:REQ, p_bytes_to_write:REQ
    push_all_m

    push p_file_handle
    push offset p_buffer
    push p_bytes_to_write
    call write_file

    pop_all_m
ENDM

write_file_m MACRO p_bundle:REQ
    write_file_internal_m p_bundle
ENDM

write_file_ptr_internal_m MACRO p_buffer_ptr:REQ, p_file_handle:REQ, p_bytes_to_write:REQ
    push_all_m

    push p_file_handle
    push p_buffer_ptr
    push p_bytes_to_write
    call write_file

    pop_all_m
ENDM

write_file_ptr_m MACRO p_bundle:REQ
    write_file_ptr_internal_m p_bundle
ENDM

; push offset file handle <- Handle
; call file_close
file_close_internal:
    pop bp ; Push adress where to return into bp register
   
    mov ax, 3e00h
    pop bx ; File handle
    int 21h

    push bp ; Put back return adress

    call catch
    ; catch_m

    ret

; FILE_CLOSE
file_close_m MACRO p_file_handle:REQ
    push_all_m

    push p_file_handle
    call file_close_internal

    pop_all_m
ENDM

setup_dta_internal:
    pop bp

    pop bx

    mov ah, 1Ah
    lea dx, [bx]
    int 21h

    push bp

    ret

setup_dta_m MACRO p_dta:REQ
    push_all_m

    push p_dta
    call setup_dta_internal

    pop_all_m
ENDM

find_first_file_internal_m MACRO p_dta:REQ, p_label:REQ
    mov ah, 4Eh
    lea dx, p_dta
    mov cx, 0
    int 21h
    jc p_label
ENDM

find_first_file_m MACRO p_bundle:REQ
    push_all_m

    find_first_file_internal_m p_bundle

    pop_all_m
ENDM

find_next_file_m MACRO p_label:REQ
    push_all_m

    mov ah, 4Fh
    int 21h
    jc p_label

    pop_all_m
ENDM

lseek_internal:
    pop bp

    pop dx
    pop cx

    mov al, 1
    mov ah, 42h
    pop bx

    int 21h

    ; TODO move this cringe from there
    ; format_to_buffer_ptr_32u_m<<offset file_match_progress_msg>, dx, ax>

    push bp

    catch_m

    ret

lseek_internal_m MACRO p_file_handle:REQ, p_high_offset_bytes:REQ, p_low_offset_bytes:REQ
    push_all_m

    push p_file_handle
    
    push p_high_offset_bytes
    push p_low_offset_bytes

    call lseek_internal

    pop_all_m
ENDM

lseek_m MACRO p_bundle:REQ
    lseek_internal_m p_bundle
ENDM

get_file_ptr_internal:
    pop bp

    pop si

    xor dx, dx
    xor cx, cx

    mov al, 1
    mov ah, 42h
    pop bx

    int 21h

    mov bx, si
    mov word ptr [bx], dx
    add bx, 2
    mov word ptr [bx], ax

    ; TODO move this cringe from there
    ; format_to_buffer_ptr_32u_m<<offset file_match_progress_msg>, dx, ax>

    push bp

    catch_m

    ret

get_file_ptr_internal_m MACRO p_file_handle:REQ, p_target_ptr:REQ
    push_all_m

    push p_file_handle
    push p_target_ptr

    call get_file_ptr_internal

    pop_all_m
ENDM

get_file_ptr_m MACRO p_bundle:REQ
    get_file_ptr_internal_m p_bundle
ENDM

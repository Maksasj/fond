.model small
.stack 100h

include catch.inc

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
    push offset p_file_handle
    push offset p_file_name
    call open_file_read
ENDM

open_file_read_m MACRO p_bundle:REQ
    open_file_read_internal_m p_bundle
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
    push offset p_file_handle
    push offset p_file_name
    call create_file
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
    push offset p_bytes_read_out
    push p_file_handle
    push offset p_buffer
    push p_bytes_to_read
    call read_file
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
    push p_file_handle
    push offset p_buffer
    push p_bytes_to_write
    call write_file
ENDM

write_file_m MACRO p_bundle:REQ
    write_file_internal_m p_bundle
ENDM

; push offset file handle <- Handle
; call file_close
file_close:
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
    push p_file_handle
    call file_close
ENDM
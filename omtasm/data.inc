.model small
.stack 100h

.data

.code

dta_m MACRO p_label:REQ
    p_label db 128 dup(?)
ENDM

byte_buffer_internal_m MACRO p_label:REQ, p_size:REQ
    p_label db p_size dup(?)
ENDM

byte_buffer_m MACRO p_bundle:REQ
    byte_buffer_internal_m p_bundle
ENDM

word_buffer_internal_m MACRO p_label:REQ, p_size:REQ
    p_label dw p_size dup(?)
ENDM

word_buffer_m MACRO p_bundle:REQ
    word_buffer_internal_m p_bundle
ENDM
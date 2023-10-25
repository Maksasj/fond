.model small
.stack 100h

.data

.code

; FOR
for_internal_m MACRO p_iterator:REQ, p_start:REQ, p_label:REQ
    mov p_iterator, p_start
        p_label:
ENDM

for_m MACRO p_bundle:REQ
    for_internal_m p_bundle
ENDM

; FOR_END
for_end_internal_m MACRO p_iterator:REQ, p_end:REQ, p_label:REQ
    inc p_iterator
    cmp p_iterator, p_end
    jne p_label
ENDM

for_end_m MACRO p_bundle:REQ
    for_end_internal_m p_bundle
ENDM

; JMP_NOT_EQL
jmp_not_eql_internal_m MACRO p_first:REQ, p_second:REQ, p_label:REQ
    cmp p_first, p_second
    jne p_label
ENDM

jmp_not_eql_m MACRO p_bundle:REQ
    jmp_not_eql_internal_m p_bundle
ENDM

; (p_min_value <= p_value <= p_max_value) -> p_label
jmp_not_in_range_internal_m MACRO p_value:REQ, p_min_value:REQ, p_max_value:REQ, p_label:REQ 
    cmp p_value, p_min_value
    jb p_label

    cmp p_value, p_max_value
    ja p_label
ENDM

jmp_not_in_range_m MACRO p_bundle:REQ
    jmp_not_in_range_internal_m p_bundle
ENDM


; for_m <cx, 0, copy>
;     print_m <error_message>
; for_end_m<cx, 15, copy>
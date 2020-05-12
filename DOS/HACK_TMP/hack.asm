
.model tiny

.code 
                        
locals @@

include ..\header.asm    

;-----------------------------------------------------
; Print string macro
;-----------------------------------------------------
.putst          macro str_ptr
                nop

                mov ah, 09h
                mov dx, offset str_ptr
                int 21h

                nop
                endm
;-----------------------------------------------------

org 100h
;-----------------------------------------------------
Start:          push cs
                push cs



                .putst Gret
		
                pop es
                pop ds

                call getstr

                mov di, offset String
                call pascmp

                call dummy2
                .exitDOS 1
;-----------------------------------------------------


Gret            db "Input password to get an access to the DREC dormitory: ", CRLF, '$'

Denied          db "Acces is denied.", '$'

Allowed         db "Acces is allowed.", '$'

;-----------------------------------------------------
;-----------------------------------------------------
dummy3          proc
                nop
                nop
                jmp @@ex
                
                db "AD6CGSG"
                nop
@@ex:           ret
dummy3          endp
;-----------------------------------------------------

;-----------------------------------------------------
; Function for store password
;-----------------------------------------------------
pass            proc
                jmp @@movv

Pas:            db "RTRT"                

@@movv:
                mov ax, offset Pas
                ret
pass            endp
;-----------------------------------------------------

;-----------------------------------------------------
COMMENT @
        Compare password procedure
        ARGUMENTS:
        di - addres of the string to compare
@
;-----------------------------------------------------
pascmp          proc
                jmp @@st
                nop

@@Nexxt1:       loop @@Next
                jmp @@OK

@@st:           call pass
                jmp @@pp
String:		db 10 dup('&')

@@pp:           mov si, ax
                and cx, 0h
                or cx, 100b


@@Next:         cmpsb

                je @@Nexxt 

                jmp @@notOK
                 
@@Nexxt:        jmp @@Nexxt1

@@OK:
                .putst Allowed                            
                jmp @@ex


@@notOK:        .putst Denied        

@@ex:           ret
pascmp          endp
;-----------------------------------------------------

;-----------------------------------------------------
dummy           proc
                mov di, offset String

                call dummy1
                ret
dummy           endp
;-----------------------------------------------------


;-----------------------------------------------------
COMMENT @ Get string from stdin procedure
        ARGUMENTS:
        - di - addres to put string
        DESTR:
                AX, 
@
;-----------------------------------------------------
getstr          proc
                
                jmp @@2
@@1:

                call dummy
                jmp @@Next

@@2:            jmp @@1
      

                

@@Next:         mov ah, 1h
                int 21h

                cmp al, CR
                je @@Fnc_end

                stosb
                jmp @@Next


@@Fnc_end:      mov al, '$'
                stosb
                call dummy2

                ret
getstr          endp
;-----------------------------------------------------



include ..\STRS\str_hd.asm


dummy1          proc
                cld

                ret
dummy1          endp


dummy2          proc
                dec di
                
                call dummy3

                ret
dummy2          endp
                   

end Start


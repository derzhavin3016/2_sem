;.186

.model tiny
.code

locals @@
include ..\header.asm

org 100h

;-------------------------------------------------------
Start:          mov ah, 09h
                mov dx, offset String
                int 21h

                mov ah, 09h
                mov dx, offset String1
                int 21h

                mov al, 'w'
                mov di, offset String
;                mov si, offset String1
                 
                call strchr

		cmp di, 0h
		;jz end_main

                mov ah, 09h
                mov dx, di
                int 21h
               
end_main:       .getch

                .exitDos         
;-------------------------------------------------------
include str_hd.asm

String:         db "Hello world", NULLCHR, CRLF, '$'
String1:        db "llo", NULLCHR, CRLF, '$'

end Start
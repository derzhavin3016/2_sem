
.model tiny
.code
org 100h

Start:		mov ax, 3802d
		push ax
		push ax
		pop bx
		pop cx
		
		mov ah, 09h         ; ah = 9
		mov dx, offset Msg  ; dx = &Msg
		int 21h             ; interrupt (puts (dx))
		
		mov ax, 4c00h       ;  exit( al = 0), ah = 4c, ax = 4c00h
		int 21h             ;  system call

CR		equ 13 ; defines
LF		equ 10
CRLF		equ CR, LF

Msg:		db 'Hello meow', CRLF, '$'
		
end 		Start
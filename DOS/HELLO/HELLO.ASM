

.model tiny
.code
org 100h

Start:		mov ax, 0003h
                int 10h
				
		mov dx, 0h

start_cycle:	mov al, '0'
		mov bx, offset Msg
		add bx, dx
	        cmp [bx], al
		je end_cycle

		mov ah, 0ah
		mov al, [bx]
 		mov bh, 0h
                mov cx, 1h
		int 10h
		
		inc dx


		mov ah, 02h
		mov bh, 0h
		int 10h 

		
		jmp start_cycle

end_cycle:	mov ax, 4C00h
                int 21h

 
NOL		equ '0'	

Msg:		db "Hello world!", NOL

end 		Start

.model tiny

.data

VERT		equ 0bah
GOR		equ 0cdh
DEFCOL		equ 1Bh
SPACE		equ 20h
SHD		equ 70h
VIDEO_SEG	equ 0b800h

TEXT_MODE	equ 3h

END_MODE	equ 4c00h

put_angles proc
    		push bx      
		mov al, 0c9h
		mov byte ptr es:[bx], al
		inc bx

	        mov ah, DEFCOL
		mov byte ptr es:[bx], ah
		inc bx
	

		add bx, cx
		add bx, cx		

		mov al, 0bbh
		mov byte ptr es:[bx], al
		inc bx

	        mov ah, DEFCOL
		mov byte ptr es:[bx], ah
		inc bx
		
   	        pop bx
		push bx

		mov al, dl
		mov ah, 80 * 2
		mul ah
		
		sub ax, 80 * 2

		add bx, ax          
		mov al, 0c8h

		mov byte ptr es:[bx], al
		inc bx

		mov ah, DEFCOL
		mov byte ptr es:[bx], ah
		inc bx
	
		add bx, cx
		add bx, cx

		mov al, 0bch

		mov byte ptr es:[bx], al
		inc bx

   	        mov ah, DEFCOL
		mov byte ptr es:[bx], ah
		inc bx
		
		pop bx

    		ret
put_angles  endp


make_frame proc
    mov dh, 0h
    push dx                    

    rows_cyc:	cmp dl, 0
		je return 		

		mov ch, cl
		
		pop ax
		push ax
		cmp dl, al
		je hor
	
		cmp dl, 1d
		je hor
		
                mov al, VERT			;  put a vertical bar
		mov byte ptr es:[bx], al     
		mov al, DEFCOL			; set color
		inc bx
		mov byte ptr es:[bx], al
		inc bx

		mov al, SPACE
		mov ah, DEFCOL
		mov dh, 0

		jmp row_fill

hor:            add bx, 2d    
    
                mov al, GOR
		mov ah, DEFCOL
		mov dh, 1

row_fill:	cmp ch, 0d
		je end_cyc

		mov byte ptr es:[bx], al		
		inc bx
		
		mov byte ptr es:[bx], ah
		inc bx
		

		dec ch
		jmp row_fill


end_cyc:	pop ax
		push ax
		
		cmp dl, al
                je no_smb

                cmp dl, 1d
	        je no_smb
		
                mov al, VERT
		mov ah, DEFCOL		

		mov byte ptr es:[bx], al
		inc bx

		mov byte ptr es:[bx], ah
		inc bx

	        jmp end_if
                  
no_smb:         add bx, 2d		  

end_if:         pop ax
		push ax

		cmp dl, al
		je no_shd
		
		inc bx
		mov al, SHD
		mov byte ptr es:[bx], al
		dec bx

no_shd:		add bx, 78 * 2
		mov ch, 0h
	
		sub bx, cx		
		sub bx, cx

		dec dl
		jmp rows_cyc

return:		pop ax
		call down_shade

		ret
make_frame endp

down_shade proc
 	        add bx, 2
  
		mov ch, cl
		add ch, 2h
	
make_shd:	cmp ch, 0d
		je retrn

		inc bx

        	mov al, SHD
       		mov byte ptr es:[bx], al 

		inc bx
		
		dec ch
		jmp make_shd

retrn:
		ret
down_shade endp

.code
org 100h

Start:		
		mov ax, 10d
	
zoom:		cmp ax, 0
		je end_zoom
				
		push ax
	
		mov bx, VIDEO_SEG
		mov es, bx

		mov ax, TEXT_MODE
		int 10h

		
		mov bx, 2 * 2	; coordinate of left upper angle
		mov dl, 22d	; window height

		mov cx, 0h
		mov cl, 74d	; window width
                call put_angles 

		call make_frame
		pop ax
		dec ax
		jmp zoom
end_zoom:
		mov ax, END_MODE
		int 21h
		
end 		Start

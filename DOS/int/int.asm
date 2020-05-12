.model tiny

.code
org 100h

include ..\header.asm

;----------------------------------------------
Start:          xor ax, ax
                mov es, ax

                mov bx, 8 * 4

                mov ax, es:[bx]
                mov word ptr Old8, ax
                mov ax, es:[bx + 2]
                mov word ptr Old8 + 2, ax

                mov ax, cs
                cli

                mov word ptr es:[bx], offset New8
                mov word ptr es:[bx + 2], ax

                sti

                mov ax, 3100h
                mov dx, offset TheEND
                shr dx, 4
                inc dx
                int 21h


;----------------------------------------------

;----------------------------------------------
COMMENT @
  New interrupt function        
@
;----------------------------------------------
New9            proc
                push ax di es

                mov di, VIDEO_SEG
                mov es, di
                mov di, (5 * 80 + 40) * 2

                mov al, dl
                mov ah, 4eh
                stosw


                pop es di ax
                db 0eah         ; jmp far
Old9:           dw 0
                dw 0
                endp
;----------------------------------------------

;----------------------------------------------
COMMENT @
  New interrupt function        
@
;----------------------------------------------
New8            proc
                push ax di es

                mov di, VIDEO_SEG
                mov es, di
                mov di, (5 * 80 + 40) * 2
                           
                mov al, cl
                mov ah, 4eh
		mov ax, cx
                stosw


                pop es di ax
                db 0eah         ; jmp far
Old8:           dw 0
                dw 0
                endp
;----------------------------------------------

TheEND:
end Start

;---------------------------------------------------
; Useful defines
VIDEO_SEG	equ 0B800h

TEXT_MODE	equ 0003h

END_MODE	equ 4C00h

CR		equ 13

LF		equ 10

CRLF		equ CR, LF
;---------------------------------------------------

;---------------------------------------------------
; Clear console
.fflush         macro
                nop

                mov ax, TEXT_MODE
                int 10h

                nop
                endm 
;---------------------------------------------------

;---------------------------------------------------
; Waiting for keyboard input macro
.getch          macro
                nop
                xor ah, ah
                int 16h
                nop
                endm
;---------------------------------------------------

;---------------------------------------------------
; Put symbol into video segment macro
;---------------------------------------------------
.puts           macro COLOR, SMB
                nop
                mov ax, ((COLOR and 0FFh) shl 8) or (SMB and 0FFh) 
                stosw
                nop
                endm
;---------------------------------------------------
; Put char to the stdout macro
;---------------------------------------------------
.putchar        macro CHAR
                nop

                mov ah, 2h
                mov dl, CHAR
                int 21h

                nop
                endm
;---------------------------------------------------

;---------------------------------------------------

;---------------------------------------------------
; Exit from the program function
; Input: ret - return code
;---------------------------------------------------
.exitDOS        macro ret
                nop
        ifnb <ret>
                mov ax, 04C00h or (ret and 0FFh)
        else
                mov ah, 4Ch
        endif
                int 21h
                nop
                endm
;---------------------------------------------------


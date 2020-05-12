;-------------------------------------------------------
COMMENT @
  Set amount of bytes of the memory block by value
  AL - byte to set to block
  DI - begin of the memory block
  CX - amount of bytes
  Destr: AH, DI, CX
@
;-------------------------------------------------------
memset          proc
                
	
             	cld
                rep stosb

                ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
COMMENT @
  Find byte in the memory block in first cx bytes
  AL - value of a byte to find
  DI - begin of the memory block
  CX - amount of bytes
  Destr: AL, DI, CX
  Return:
  CX - if 0, symbol wasn't find
  DI - bias of the symbol in block, if it was find
@
;-------------------------------------------------------
memchr          proc

                cld
		     
                repne scasb
                dec di
	
		
                ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
COMMENT @
  Copy cx bytes from one block to another
  SI - begin of source memory block
  DI - begin of destination memory block
  CX - amount of bytes
  Destr: AL, DI, SI
@
;-------------------------------------------------------
memcpy          proc

                cld
		     
                rep movsb
                
                ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
COMMENT @
  Compare first cx bytes of two blocks
  SI - begin of first memory block
  DI - begin of second memory block
  CX - amount of bytes
  Destr: CX, DI, SI
  Retr: cmp flags
@
;-------------------------------------------------------
memcmp          proc

                cld
		     
                repe cmpsb 
                
                ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
COMMENT @
  Find length of the string
  DI - begin of the string
  Destr: AL, CX, DI
  Ret:
  CX - length of the string
@
;-------------------------------------------------------
strlen          proc

                cld        
                
                xor cx, cx
                mov al, NULLCHR
                
@@Next:         scasb

                jz @@end_cyc

                inc cx
                jmp @@Next
                 
@@end_cyc:      ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
.cmpd           macro param, mark
                nop

                cmp byte ptr [di], param
                jz mark

                nop
                endm
;-------------------------------------------------------

;-------------------------------------------------------
COMMENT @
  Find byte in the string
  AL - value of a byte to find
  DI - begin of the string
  Destr: AH, DI
  Return:                      
  DI - bias of the symbol in block, if it was find
@
;-------------------------------------------------------
strchr          proc            
                cld             

@@Next:         .cmpd NULLCHR, @@str_end
                
                .cmpd al, @@stop

                inc di
                jmp @@Next

@@str_end:      xor di, di

@@stop:         ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
COMMENT @
  Find byte in the string from the end
  AL - value of a byte to find
  DI - begin of the string
  Destr: AH, DI, CX
  Return:                      
  DI - bias of the symbol in block, if it was find
@
;-------------------------------------------------------
strrchr         proc   

                push ax          
                call strlen
                std
                pop ax

                sub di, 2	

                repne scasb
                inc di

                ret
                endp
;-------------------------------------------------------


;-------------------------------------------------------
COMMENT @
  Copy string to block
  SI - begin of source string
  DI - begin of destination memory block 
  Destr: AL, DI, SI
@
;-------------------------------------------------------
strcpy          proc

                cld
		     
@@Next:         cmp byte ptr [si], NULLCHR
                
                movsb
                je @@end_cyc

                jmp @@Next
                
@@end_cyc:      ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
COMMENT @
  Compare two strings
  SI - begin of first string
  DI - begin of second string
  Destr: AL, DI, SI
  Retr: AL - diference of last different symbols
@
;-------------------------------------------------------
strcmp          proc

                cld            

@@Next:         mov al, [di]
                cmp byte ptr [si], al
                jne @@end_fnc

                cmp byte ptr [si], NULLCHR
                je @@end_fnc 

                inc si
                inc di  
                jmp @@Next

@@end_fnc:      mov al, [si]
                sub al, [di]
                ret
                endp
;-------------------------------------------------------

;-------------------------------------------------------
.scan&chng      macro MARK
                nop

                scasb
                jz MARK

                dec di

                xchg di, si

                nop
                endm
;-------------------------------------------------------                


;-------------------------------------------------------
COMMENT @
  Compare two strings without null-terminated symbol
  SI - begin of first string
  DI - begin of second string
  Destr: AL, DI, SI
  Retr: cmp flags
@
;-------------------------------------------------------
strnzcmp        proc

                cld
		           
                mov al, NULLCHR

@@Next:         pushf
                .scan&chng @@end_fnc
                .scan&chng @@end_fnc

                popf
                cmpsb
                jne @@end_all

                jmp @@Next

@@end_fnc:      popf      
                jmp @@rett


@@end_all:      xor di, di

@@rett:         ret
                endp
;-------------------------------------------------------


;-------------------------------------------------------
COMMENT @
  Find the first occurnce of string in another (does not include null character
  SI - begin of string to find in di string
  DI - begin of string to find occurence in 
  Destr: AL, DI, SI
  Retr: DI - begin of occured si string if it was find, 0 otherwise
@
;-------------------------------------------------------
strstr          proc

                cld

                mov al, [si]

@@Next:         call strchr

                cmp di, 0h
                je @@end_fnc

                push di
                call strnzcmp

                cmp di, 0h
                pop di
                jz @@Next
                 
                mov di, 0h

@@end_fnc:      ret
                endp
;-------------------------------------------------------

CR		equ 13 ; defines
LF		equ 10
CRLF		equ CR, LF
NULLCHR         equ 0
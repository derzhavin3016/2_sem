
.model tiny

.code
org 100h

locals @@
include ..\header.asm
include fr_hd.asm
        
;-----------------------------------------------------
Start:		.fflush

                mov di, (80d * 4 + 14) * 2		; start position
                mov bx, 25d		                ; frame width
                mov cx, 10d             		; frame height

;               call DrawFrame
                call DrawZoom
                
                .getch

                .exitDOS 1
;-----------------------------------------------------


;-----------------------------------------------------
COMMENT @
  Draw a horizontal line
  AH - color attribute
  AL - symb to draw
  DI - adress to start draw
  CX - number of symbols
  Destr: CX ES DI
@
;-----------------------------------------------------
DrawLine        proc
                push VIDEO_SEG
                pop es
		cmp cx, 0
		
                cld
                rep stosw               ; mov es:[di], ax
                                        ; add di, 2
                ret
                endp                
;-----------------------------------------------------

;-----------------------------------------------------
COMMENT @
 Draw line(s) with edges
 AH - non-edge symbol attr
 AL - non-edge symbol
 BX - amount of non-edges elements
 CX - amount of lines
 DI - adress to start draw
 Destr: BX CX DI
@
;-----------------------------------------------------
DrawEdg         proc
                push ax

@@Next:         .puts DEFCOL, VERT   ; put edges symbol
                pop ax
                push ax              ; get non-edge symbol from stack

                push cx bx
                pop cx
                call DrawLine       
                pop cx

                .puts DEFCOL, VERT  ; put edge  symbol and shade
                .puts SHD, SPACE

                add di, 77d * 2     ; move di to the next row
                shl bx, 1
                sub di, bx
                shr bx, 1

                loop @@Next                
                pop ax              ; remove ax value from stack
                ret
                endp
;-----------------------------------------------------

;-----------------------------------------------------
COMMENT @
  Draw frame
  BX - width of the frame
  CX - height of the frame
  DI - adress to start draw
  Destr: AX ES
@
;-----------------------------------------------------
DrawFrame       proc
                push VIDEO_SEG                    ; mov es, video_segment
                pop es                
                push bx cx di                   ; save di value (start position) for the put angles function

                sub bx, 2h                        ; bx -= 2
		sub cx, 2h                        ; cx -= 2
                push cx                           ; save cx
                mov cx, 1h                        ; cx = 1

                mov ax, (DEFCOL shl 8) or GOR     ; ax = DEFCOL

                call DrawEdg
                pop cx
                push cx

                mov al, SPACE                    ; al = '  '
                call DrawEdg

                mov cx, 1h                       ; cx = 1

                mov al, GOR                      ; al = '-'
                call DrawEdg

                add di, 2h                      ;  di = 2
                mov cx, bx                      ; cx  = bx
                add cx, 2h                      ; cx += 2 
                mov ax, (SHD shl 8) or SPACE
                call DrawLine

                pop cx
                pop di
                push di
                call DrawAngl
                pop di cx bx
                ret

                endp
;-----------------------------------------------------

;-----------------------------------------------------
COMMENT @
 Draw angles to frame
 BX - width of the frame  (-2)
 CX - height of the frame (-2)
 DI - position to start draw
 Destr: BX CX DI
@
;-----------------------------------------------------
DrawAngl        proc
                shl bx, 1               ; bx *= 2               
		inc cx                  ; cx++
                shl cx, 1               ; cx *= 2
                .puts DEFCOL, LUA       ; put left upper angle

                add di, bx              ; di += bx
		add bx, 4               ; bx += 4
                .puts DEFCOL, RUA       ; put right upper angle

                .puts BLACK, SPACE      ; delete excess "shadowed" pixel
                sub di, 2h              ; di -= 2

                mov ax, 80d and 0FFh    ; al  = 80d
                mul cx                  ; ax = al * cx
                add di, ax              ; di += ax
                sub di, 2               ; di -= 2
                .puts DEFCOL, RBA       ; put right bottom angle

		sub di, bx              ; di -= bx
                .puts DEFCOL, LBA       ; put left bottom angle
                
                ret
                endp
;-----------------------------------------------------

;-----------------------------------------------------
COMMENT @
 Draw zoom frame
 BX - width of the frame
 CX - height of the frame
 DI - position to start draw
 Destr: AX ES
@
;-----------------------------------------------------
DrawZoom        proc                

                push di bx cx


              ; call DrawFrame

               ; pop di cx bx

                and bx, not 1h
                and cx, not 1h
                sub cx, 2

                mov ax, 80d
                mul cx
                add di, ax
		add di, bx
                sub di, 2
                
                mov bx, 3
		mov cx, 3

                pop ax
                push ax
                test ax, 1h
                jnz @@bx_even

                inc cx
		sub di, 80d * 2

@@bx_even:      push bp
                mov bp, sp

                test byte ptr ss:[bp + 4], 1h
                jnz @@Next

                inc bx
		sub di, 2

@@Next:         .fflush
                call DrawFrame
COMMENT @
                mov ah, 86h             ; ah = 86h 
		push cx                 ; save cx
		xor cx, cx              ; cx = 0
                mov dx, DELAY           ; dx = delay
                int 15h                 ; delay interrupt
		pop cx
@
               
                mov ax, ss:[bp + 2]
                cmp cx, ax
                jne @@check_inc

                cmp bx, ss:[bp + 4] 
                jne @@check_inc

                jmp @@exit_cyc

@@check_inc:    cmp cx, ax
                je @@check_bx 
                
                add cx, 2
                sub di, 80d * 2
                
@@check_bx:     cmp bx, ss:[bp + 4]
                je @@repeat

                add bx, 2
                sub di, 2
                           
@@repeat:       jmp @@Next 
                

@@exit_cyc:     pop bp cx bx di  
                ret
                endp
;-----------------------------------------------------


end Start
;-----------------------------------------------------

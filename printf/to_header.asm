.686
.model flat, stdcall
option casemap: none              

includelib C:\masm32\lib\kernel32.lib

STD_OUTPUT_HANDLE       equ -11d

GetStdHandle    proto :dword
WriteConsoleA   proto :dword, :dword, :dword, :dword, :dword
ExitProcess     proto :dword

WriteSym        proto :byte                      
TobNum          proto :dword
ToDec           proto :dword
.data

Msg             db "Hello world!", 0ah, 0
smbol           db 0

hex             db "0123456789abcdef"
.code

;------------------------------------------------
COMMENT @
        Write symbol to console macro
        Arguments:
                - sym - symbol to write         
@
;------------------------------------------------
WriteSym        proc, sym: byte
                nop
                push eax
                push ebx
                push ecx
                push edx

                invoke GetStdHandle, STD_OUTPUT_HANDLE

                xor edx, edx
                mov dl, sym
                
                mov byte ptr smbol, dl  

                invoke WriteConsoleA, eax, offset smbol, 1, 0, 0

                pop edx
                pop ecx
                pop ebx
                pop eax

                nop
                ret 
WriteSym        endp               
;------------------------------------------------ 


;------------------------------------------------ 
COMMENT @
        Number convert macro
@
;------------------------------------------------ 
Conv            macro edx_add, dl_val, exit_label, next_label
                nop

                and dl, bh
                and edx, 0ffh
                add edx, edx_add
                mov dl, dl_val
                invoke WriteSym, dl

                mov edx, num
                cmp cl, 0
                je exit_label

                sub cl, bl
                shr edx, cl

                jmp next_label

                nop
                endm
;------------------------------------------------ 


;------------------------------------------------------------
COMMENT @
        Convert number from hex, oct or bin to string procedure
        Arguments: 
                - num - number to convert
                - cl - first shift right length (31d for binary, 
                                                  30d for octal,
                                                  28d for hexadecimal).
                - bl - bias size (1d for binary, 3d for octal, 4d for hexadecimal)
                - bh - mask (1b for binary, 111b for octal, 1111b for hexadecimal)
              Destr: ecx                                         
@
;------------------------------------------------------------
TobNum          proc num: dword

                push edx               

Next:           
                mov edx, num

                shr edx, cl
                cmp edx, 0
                jnz Convert
                
                cmp cl, 0
                je Zero

                sub cl, bl
                jmp Next

Convert:
                cmp bl, 1h
                je writeBin

                cmp bl, 3h
                je writeOct

                jmp writeHex


writeBin:       Conv '0', dl, Fnc_end, writeBin

writeOct:       Conv '0', dl, Fnc_end, writeOct

writeHex:       Conv offset hex, byte ptr [edx], Fnc_end, writeHex

Zero:           invoke WriteSym, '0' 

Fnc_end:        pop edx
                ret
TobNum          endp
;------------------------------------------------

;------------------------------------------------
COMMENT @
                Useful define for ToDec function
@
;------------------------------------------------
DivTen          macro exit_label, next_label
                nop

                push edx
                xor edx, edx                
                mov eax, ebx            ; eax = ebx
                mov ebx, 10d            ; ebx = 10
                div ebx                 ; eax = eax / ebx

                cmp eax, 0
                je exit_label               
                                        ; edx = eax % ebx
                xchg eax, ebx           ; Swap(eax, ebx)
                pop eax

                jmp next_label

                nop
                endm
;------------------------------------------------

;------------------------------------------------
COMMENT @
        Convert number from decimal to string
        Arguments:
                - num - number to convert 
@
;------------------------------------------------
ToDec           proc num: dword
                push eax
                push edx
                push ebx

                mov ebx, 1000000000d    ; ebx = 10e9
                push num
Next_prep:
                xor edx, edx            ; edx = 0
                mov eax, num            ; eax = num
                div ebx

                cmp eax, 0
                jne Convert

                DivTen Zero, Next_prep


Convert:        
                pop eax
                xor edx, edx
                
Next:           div ebx                 ; eax = eax / ebx
                                        ; edx = eax % ebx
                add al, '0'             ; al = '0'
                invoke WriteSym, al     ; putchar(al)

                DivTen Fnc_end, Next

Zero:           invoke WriteSym, '0'
Fnc_end:        pop ebx
                pop edx
                pop eax
                ret
ToDec           endp
;------------------------------------------------

;------------------------------------------------
COMMENT @
                Convert string to string in console
                ARGUMENTS:
                        esi - address of the start of the string
                Destr: esi, al 
@
;------------------------------------------------
ToStr           proc
                
          
                cld
Next:           
                cmp byte ptr [esi], 0
                je Fnc_end

                lodsb
                invoke WriteSym, al
                jmp Next

Fnc_end:        ret
ToStr           endp
;------------------------------------------------

COMMENT %   
;------------------------------------------------
Start:                                                                                                                                                                                                                     
                invoke GetStdHandle, STD_OUTPUT_HANDLE
              COMMENT @
                mov edx, 1h

                mov cl, 31d
                mov bl, 1d
                mov bh, 1b
                invoke TobNum, edx
                @      
                mov eax, 100d
                invoke ToDec, eax
                 

               ; mov esi, offset Msg
                ;call ToStr
                ;invoke WriteConsoleA, eax, offset Msg, 13, 0, 0

                invoke ExitProcess, 0
;------------------------------------------------

end Start%      
        
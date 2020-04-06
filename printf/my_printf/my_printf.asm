.686 
.MODEL FLAT, C, STDCALL
option casemap: none              


includelib C:\masm32\lib\kernel32.lib

STD_OUTPUT_HANDLE       equ -11d

GetStdHandle    proto :dword
WriteConsoleA   proto :dword, :dword, :dword, :dword, :dword
ExitProcess     proto :dword

Ad6Printf       proto C

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


;------------------------------------------------
;        Compare string element with byte macro
;------------------------------------------------
Compsb          macro cmp_byte, je_label
                nop

                cmp byte ptr [esi], cmp_byte
        ifnb <je_label>
                je je_label
        endif

                nop
                endm
;------------------------------------------------


;------------------------------------------------
COMMENT @
        Dispatcher printf function
        Arguments:      
                - esi - adress of fromat string
                - arguments in stack
@
;------------------------------------------------
DispPrintf      proc

                Compsb '%', Procent 

                add ebp, 4

                Compsb 's', String

                Compsb 'c', Char

                Compsb 'x', Hex

                Compsb 'o', Oct

                Compsb 'b', Bin

                Compsb 'd', Decm

Procent:        invoke WriteSym, '%'

                jmp Fnc_end

String:         push esi
                mov esi, [ebp]
                call ToStr 

                pop esi

                jmp Fnc_end

Char:           invoke WriteSym, [ebp]

                jmp Fnc_end

Hex:            mov cl, 28d
                mov bx, (1111b shl 8) or 4d

                invoke TobNum, [ebp]

                jmp Fnc_end

Oct:            mov cl, 30d
                mov bx, (111b shl 8) or 3d

                invoke TobNum, [ebp]

                jmp Fnc_end

Bin:            mov cl, 31d
                mov bx, (1d shl 8) or 1d

                invoke TobNum, [ebp]

                jmp Fnc_end

Decm:           invoke ToDec, [ebp]

Fnc_end:        inc esi
                ret
DispPrintf      endp
;------------------------------------------------


;------------------------------------------------
COMMENT @
        Printf function.
        Argumens: (push in reverse order)
                - (dword) format string
                - arguments
@
;------------------------------------------------
Ad6Printf       proc C
                push ebp
                mov ebp, esp
                
                cld
                add ebp, 8
                mov esi, [ebp] 

Next:           
                cmp byte ptr [esi], 0
                je End_fnc

                lodsb

                cmp al, '%'
                jne print_sym

                call DispPrintf
                jmp Next

print_sym:
                invoke WriteSym, al
                jmp Next

End_fnc:        pop ebp
                ret
Ad6Printf       endp        

;------------------------------------------------

end
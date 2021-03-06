include to_header.asm

.data
               
Format          db "Hello %c %s %x %d%%%c%b", 0ah, 0
Lov             db "love", 0   

.code

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
Ad6Printf       proc
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


;------------------------------------------------
Start:                                                                                                                                                                                                                     
                invoke GetStdHandle, STD_OUTPUT_HANDLE
              
                push 127o
                push '!'
                push 100d
                push 0edah
                push offset Lov
                push 'I' 
                push offset Format
                call Ad6Printf


                ;invoke WriteConsoleA, eax, offset Msg, 13, 0, 0

                invoke ExitProcess, 0
;------------------------------------------------
end Start                                      

.686
.MODEL FLAT, C, STDCALL
option casemap: none

LstFind proto C

StringCmp proto C

.code
;------------------------------------------------------------
; List find function
;------------------------------------------------------------
LstFind         proc C
                mov ebx, dword ptr [esp + 4] ; ebx = this_
                mov esi, dword ptr [ebx]     ; esi = this_->Head

                test esi, esi                   ; esi == 0 ?
                jne fnc_cycle

                xor eax, eax                 ; eax = 0
                ret

fnc_cycle:
                mov ebx, dword ptr [esp + 12]  ; edx = *(esp + 12)  // edx = cmp
                mov edi, dword ptr [esp +  8]   ; edi = Data

Next:           
                mov eax, dword ptr [esi] ; eax = this_->head->data
                push edi
                push eax
                call ebx
                add esp, 8
                test eax, eax
                jz fnc_end

                mov esi, dword ptr [esi + 4]

                test esi, esi
                jnz Next

                xor eax, eax

                ret

fnc_end:        mov eax, 1        
                ret
LstFind         endp
;------------------------------------------------------------

;------------------------------------------------------------
StringCmp       proc C
                mov esi, [esp + 4]
                mov esi, [esi]
                mov edi, [esp + 8]
                mov edi, [edi]

Next:
                mov al, [edi]
                cmp byte ptr [esi], al
                jne end_fnc
                
                and al, al
                jz end_fnc

                inc si
                inc di
                jmp Next

end_fnc:        
                movzx  eax, byte ptr [esi]
                sub    al, byte ptr [edi]
                cbw
                cwde

                ret
StringCmp       endp
;------------------------------------------------------------

end

COMMENT @

typedef struct TAGlist
{
  base_list *Head, *Tail;
  size_t size;
} list;

@

;int LstFind( list *this_, const lst_data *Data, int (*cmp)( const void *a, const void *b ) )
;{
;  assert(this_ != nullptr);
;
;  if (this_->Head == nullptr)
;    return 0;
;
; base_list *lst = this_->Head;
;
; for (size_t i = 0; i < this_->size; ++i)
; {
;   if (cmp(lst->Data, Data) == 0)
;     return 1;
;   lst = lst->Next;
; }
;
; return 0;
;}

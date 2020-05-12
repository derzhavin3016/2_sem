.model tiny
.code
org 100h

Start:          mov ah, 1h
                int 21h

                inc ah
                mov dl, al
                int 21h

                mov ax, 4c00h
                int 21h                

end Start


    
PUBLIC _x64spoof_stub
option casemap :none

_TEXT SEGMENT
_x64spoof_stub PROC
    pop r11
    add rsp, 8
    mov rax, [rsp + 24]
        
    mov r10, [rax]
    mov [rsp], r10
        
    mov r10, [rax + 8]
    mov [rax + 8], r11
     
    mov [rax + 16], rdi
    lea rdi, fixle
    mov [rax], rdi
    mov rdi, rax
        
    jmp r10
fixle:
    sub rsp, 16
    mov rcx, rdi
    mov rdi, [rcx + 16]
    jmp QWORD PTR [rcx + 8]
_x64spoof_stub ENDP
_TEXT ENDS
END
.global ipf_sqrt2
.type ipf_sqrt2 @function

ipf_sqrt2:
testl %edi, %edi
jz .L_zero

lzcntl %edi, %ecx
movl %ecx, %eax

subl $16, %eax
sarl $1, %eax

leal 16(%rax), %ecx
movl %edi, %eax
shrv %cl, %eax
addl $0xC000, %eax

movl $16, %ecx
subl %eax, %ecx
ret

.L_zero:
xorl %eax, %eax
ret

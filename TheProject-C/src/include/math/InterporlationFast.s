.global ipf_sqrt2
.type ipf_sqrt2 @function

ipf_sqrt2:
testl %edi, %edi
jz .L_zero

lzcntl %edi, %ecx
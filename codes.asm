push 10240
pop VID
push 14336
pop VIF
call :main 
hlt 

main: 



push VID
dup
dup
push 64
zdiv
push 64
mul
sub
sub
push 64
add
pop VID
call :fuck
push RCX

pop RCX

push VID
dup
dup
push 64
zdiv
push 2
sub
push 64
mul
sub
sub
push 64
add
pop VID
RET

RET


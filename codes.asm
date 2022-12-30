push 10240
pop VID
push 14336
pop VIF
call :main 
hlt 

main: 
push [VIF + 0] 
pop [VID + 0] 

in
pop [VID + 1]

push [VID + 1]
out

push [VID + 2]
push 1
push 1
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


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
pop [VIF + 0] 



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

pop [VID + 2]

push [VID + 2]
out

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

fuck: 
push [VIF + 0] 
pop [VID + 0] 

push 0
push [VID + 0]
push 1
GE
jee :else_0
push [VID + 0]
push [VID + 0]
push 1
SUB
MUL
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


jmp :endif_0
else_0:
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

endif_0:
RET


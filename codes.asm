push 10240
pop VID
push 14336
pop VIF
call :main 
hlt 

main: 
in
pop [VID + 0]

push 5
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

pop [VID + 1]

push [VID + 1]
out

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
push 1
SUB
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

push [VID + 1]
push [VID + 2]
MUL
pop [VID + 3]

push [VID + 3]
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


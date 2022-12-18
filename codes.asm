push 10240
pop VID
push 14336
pop VIF
call :main 
hlt 

main: 
push 0
push 7
SUB
ABS
push 10
ADD
pop [VID + 0]

push 0
push 7
SUB
ABS
push 10
ADD
out

RET


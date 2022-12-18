push 10240
pop VID
push 14336
pop VIF
call :main 
hlt 

main: 
push 3
push 2
DEGREE
pop [VID + 0]

push [VID + 0]
out

RET


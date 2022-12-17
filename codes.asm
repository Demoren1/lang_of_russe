push 14336
pop VIF
call :main 
hlt 

main: 
push 10240
pop VID

push 5
push 7
SUB
ABS
pop [VID + 0]

push [VID + 0]
out

RET


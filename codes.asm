push 10240
pop VAR_IN_DEF

call :main 
hlt 

push 10240
pop VAR_IN_DEF

main: 
push 1
pop [VAR_IN_DEF + 0]

push 1
pop [VAR_IN_DEF + 1]

push [VAR_IN_DEF + 1]
push [VAR_IN_DEF + 0]
ADD
pop [VAR_IN_DEF + 0]

push 1
pop [VAR_IN_DEF + 0]

RET


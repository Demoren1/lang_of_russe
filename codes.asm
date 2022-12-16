push 10240
pop VAR_IN_DEF

call :main 
hlt 

push 10240
pop VAR_IN_DEF

main: 
in
pop [VAR_IN_DEF + 0]

in
pop [VAR_IN_DEF + 0]

in
pop [VAR_IN_DEF + 0]

RET


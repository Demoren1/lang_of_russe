push 14336
pop VAR_IN_FUNCALL
call :main 
hlt 

main: 
push 10240
pop VAR_IN_DEF

push 1
pop [VAR_IN_DEF + 0]

push [VAR_IN_DEF + 0] 
pop [VAR_IN_FUNCALL + 0] 

call :fuck
push RCX

pop [VAR_IN_DEF + 0]

push [VAR_IN_DEF + 0]
out

RET

fuck: 
push 10304
pop VAR_IN_DEF

push [VAR_IN_FUNCALL + 0] 
pop [VAR_IN_DEF + 0] 

push 3
pop [VAR_IN_DEF + 0]

push [VAR_IN_DEF + 0]
pop RCX

RET


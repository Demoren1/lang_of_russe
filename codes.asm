push 10240
pop VID
push 14336
pop VIF
call :main 
hlt 

main: 
push 0      ## ax_+_b_=_0
push 10 
push 48
push 95
push 61
push 95
push 98
push 95
push 43
push 95
push 120
push 97
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
pop [8]
pop [9]
pop [10]
pop [11]
show

push 0      ## _input_a_and_b
push 10 
push 98
push 95
push 100
push 110
push 97
push 95
push 97
push 95
push 116
push 117
push 112
push 110
push 105
push 95
pop [0]
pop [1]
pop [2]
pop [3]
pop [4]
pop [5]
pop [6]
pop [7]
pop [8]
pop [9]
pop [10]
pop [11]
pop [12]
pop [13]
pop [14]
pop [15]
show

in
pop [VID + 2]

in
pop [VID + 3]

push 0
push [VID + 3]
SUB
push [VID + 2]
DIV
pop [VID + 4]

push [VID + 4]
out

RET


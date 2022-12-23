push 10240
pop VID
push 14336
pop VIF
call :main 
hlt 

main: 
push [VIF + 0] 
pop [VID + 0] 

push 0      ## ax^2_+_bx_+_c_=_0
push 10 
push 48
push 32
push 61
push 32
push 99
push 32
push 43
push 32
push 120
push 98
push 32
push 43
push 32
push 50
push 94
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
pop [12]
pop [13]
pop [14]
pop [15]
pop [16]
pop [17]
pop [18]
show

push 0      ## _input_a,_b,_c
push 10 
push 99
push 32
push 44
push 98
push 32
push 44
push 97
push 32
push 116
push 117
push 112
push 110
push 105
push 32
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
pop [VID + 3]

in
pop [VID + 4]

in
pop [VID + 5]

push [VID + 4]
push 2
DEGREE
push 4
push [VID + 3]
MUL
push [VID + 5]
MUL
SUB
pop [VID + 6]

push 0
push [VID + 4]
SUB
push [VID + 6]
push 1
push 2
DIV
DEGREE
SUB
push 2
push [VID + 3]
MUL
DIV
pop [VID + 7]

push 0
push [VID + 4]
SUB
push [VID + 6]
push 1
push 2
DIV
DEGREE
ADD
push 2
push [VID + 3]
MUL
DIV
pop [VID + 8]

push [VID + 7]
out

push [VID + 8]
out

RET


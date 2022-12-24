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

push 0
push [VID + 3]
ABS
jee :else_0
push [VID + 3] 
pop [VIF + 0] 
push [VID + 4] 
pop [VIF + 1] 
push [VID + 5] 
pop [VIF + 2] 



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
call :solve_square
push RCX


jmp :endif_0
else_0:
push 0
push [VID + 4]
ABS
jee :else_1
push [VID + 4] 
pop [VIF + 0] 
push [VID + 5] 
pop [VIF + 1] 



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
call :solve_lin
push RCX


jmp :endif_0
else_1:
push [VID + 5] 
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
call :solve_identity
push RCX

endif_0:
endif_1:
push 0
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

solve_square: 
push [VIF + 0] 
pop [VID + 0] 
push [VIF + 1] 
pop [VID + 1] 
push [VIF + 2] 
pop [VID + 2] 

push [VID + 1]
push 2
DEGREE
push 4
push [VID + 0]
MUL
push [VID + 2]
MUL
SUB
pop [VID + 3]

push 0
push [VID + 3]
push 0
GE
jee :else_2
push [VID + 0] 
pop [VIF + 0] 
push [VID + 1] 
pop [VIF + 1] 
push [VID + 2] 
pop [VIF + 2] 
push [VID + 3] 
pop [VIF + 3] 



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
call :solve_full_square
push RCX


jmp :endif_2
else_2:
push 0
push [VID + 3]
ABS
jee :else_3
push 0      ## _no_roots_
push 10 
push 32
push 115
push 116
push 111
push 111
push 114
push 32
push 111
push 110
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
show


jmp :endif_2
else_3:
push [VID + 0] 
pop [VIF + 0] 
push [VID + 1] 
pop [VIF + 1] 



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
call :find_one_root
push RCX

endif_2:
endif_3:
push 0
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

solve_full_square: 
push [VIF + 0] 
pop [VID + 0] 
push [VIF + 1] 
pop [VID + 1] 
push [VIF + 2] 
pop [VID + 2] 
push [VIF + 3] 
pop [VID + 3] 

push 0
push [VID + 1]
SUB
push [VID + 3]
push 1
DEGREE
push 2
DIV
SUB
push 2
push [VID + 0]
MUL
DIV
pop [VID + 4]

push 0
push [VID + 1]
SUB
push [VID + 3]
push 1
DEGREE
push 2
DIV
ADD
push 2
push [VID + 0]
MUL
DIV
pop [VID + 5]

push 0      ## _roots_are_
push 10 
push 32
push 101
push 114
push 97
push 32
push 115
push 116
push 111
push 111
push 114
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
show

push [VID + 4]
out

push [VID + 5]
out

push 0
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

find_one_root: 
push [VIF + 0] 
pop [VID + 0] 
push [VIF + 1] 
pop [VID + 1] 

push 0
push [VID + 1]
SUB
push 2
push [VID + 0]
MUL
DIV
pop [VID + 2]

push 0      ## _root_is_
push 10 
push 32
push 115
push 105
push 32
push 116
push 111
push 111
push 114
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
show

push [VID + 2]
out

push 0
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

solve_lin: 
push [VIF + 0] 
pop [VID + 0] 
push [VIF + 1] 
pop [VID + 1] 

push 0
push [VID + 1]
SUB
push [VID + 0]
DIV
pop [VID + 2]

push 0      ## _root_is_
push 10 
push 32
push 115
push 105
push 32
push 116
push 111
push 111
push 114
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
show

push [VID + 2]
out

push 0
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

solve_identity: 
push [VIF + 0] 
pop [VID + 0] 

push 0
push [VID + 0]
ABS
jee :else_4
push 0      ## _no_roots_
push 10 
push 32
push 115
push 116
push 111
push 111
push 114
push 32
push 111
push 110
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
show


jmp :endif_4
else_4:
push 0      ## _inf_roots_
push 10 
push 32
push 115
push 116
push 111
push 111
push 114
push 32
push 102
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
show

endif_4:
push 0
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


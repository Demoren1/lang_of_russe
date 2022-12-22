all : mkdir lang
SRC_CPU_DIR = ./src/cpu/
SRC_ASM_DIR = ./src/asm/
SRC_LANG_DIR = ./src/lang/
SRC_DIFF_DIR = ./src/differentiator/
SRC_TREE_DIR = ./src/tree/
SRC_DESCENT_DIR = ./src/recursive_descent/
DEBUG_DIR = ./src/debug_and_constants/
DUMP_DIR = ./graph_dumps/
OBJ_DIR = ./obj/
CC = g++

# RELEASE_FLAGS := -DNDEBUG
# release:
# 	g++ $(RELEASE_FLAGS)

CFLAGS= -save-temps -Wall -Wextra -fsanitize=address -g #-D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging \
-fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr \
-pie -Wlarger-than=8192 -Wstack-usage=8192

#include
INCLUDE_PATH = -I./include/asm/ -I./include/code_gen/ -I./include/cpu/ -I./include/diff_headers/ -I./include/lang_headers/ -I./include/other/ -I./include/tree/

#sources
DIFF_SRC  	:= $(wildcard $(SRC_DIFF_DIR)*.cpp)
TREE_SRC  	:= $(wildcard $(SRC_TREE_DIR)*.cpp)
DESCENT_SRC := $(wildcard $(SRC_DESCENT_DIR)*.cpp)
ASM_SRC   	:= $(wildcard $(SRC_ASM_DIR)*.cpp) 
CPU_SRC   	:= $(wildcard $(SRC_CPU_DIR)*.cpp) 
DEBUG_SRC 	:= $(wildcard $(DEBUG_DIR)*.cpp)
LANG_SRC  	:= $(wildcard $(SRC_LANG_DIR)*.cpp)


#obj
DIFF_OBJ  	:= $(patsubst $(SRC_DIFF_DIR)%.cpp, $(OBJ_DIR)%.o, $(DIFF_SRC)) 
TREE_OBJ  	:= $(patsubst $(SRC_TREE_DIR)%.cpp, $(OBJ_DIR)%.o, $(TREE_SRC)) 
DESCENT_OBJ := $(patsubst $(SRC_DESCENT_DIR)%.cpp, $(OBJ_DIR)%.o, $(DESCENT_SRC)) 
ASM_OBJ   	:= $(patsubst $(SRC_ASM_DIR)%.cpp, $(OBJ_DIR)%.o, $(ASM_SRC)) 
CPU_OBJ   	:= $(patsubst $(SRC_CPU_DIR)%.cpp, $(OBJ_DIR)%.o, $(CPU_SRC))
DEBUG_OBJ 	:= $(patsubst $(DEBUG_DIR)%.cpp, $(OBJ_DIR)%.o, $(DEBUG_SRC))
LANG_OBJ  	:= $(patsubst $(SRC_LANG_DIR)%.cpp, $(OBJ_DIR)%.o, $(LANG_SRC))


#exe
LANG_EXE := lang

$(LANG_EXE) : $(DIFF_OBJ) $(TREE_OBJ) $(DESCENT_OBJ) $(CPU_OBJ) $(DEBUG_OBJ) $(ASM_OBJ) $(LANG_OBJ)
	@$(CC) $(CFLAGS) $(INCLUDE_PATH) $(DIFF_OBJ) $(TREE_OBJ) $(DESCENT_OBJ) $(CPU_OBJ) $(DEBUG_OBJ) $(ASM_OBJ) $(LANG_OBJ) -o $(LANG_EXE)

$(OBJ_DIR)%.o : $(SRC_CPU_DIR)%.cpp
	@$(CC)  $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(SRC_ASM_DIR)%.cpp
	@$(CC)  $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(DEBUG_DIR)%.cpp
	@$(CC) $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(SRC_DIFF_DIR)%.cpp 
	@$(CC) $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(SRC_TREE_DIR)%.cpp 
	@$(CC) $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(SRC_DESCENT_DIR)%.cpp 
	@$(CC) $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(SRC_LANG_DIR)%.cpp 
	@$(CC) $(INCLUDE_PATH) -c $< -o $@


mkdir :
	@mkdir $(OBJ_DIR) -p
	@mkdir $(DUMP_DIR) -p

clean:
	rm $(OBJ_DIR)*.o $(OBJ_DIR)*.ii $(OBJ_DIR)*.s $(DUMP_DIR)*.jpeg *.save lang
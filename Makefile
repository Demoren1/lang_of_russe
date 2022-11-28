all : mkdir asm cpu 
CPU_DIR = ./src/cpu/
ASM_DIR = ./src/asm/
DEBUG_DIR = ./src/debug_and_constants/
OBJ_DIR = ./obj/
CC = g++

# RELEASE_FLAGS := -DNDEBUG
# release:
# 	g++ $(RELEASE_FLAGS)

CFLAGS= #-save-temps -Wall -Wextra -fsanitize=address -g #-D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging \
-fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr \
-pie -Wlarger-than=8192 -Wstack-usage=8192

#include
INCLUDE_PATH = -I./include/

#sources
ASM_SRC 	:= $(wildcard $(ASM_DIR)*.cpp) 
CPU_SRC 	:= $(wildcard $(CPU_DIR)*.cpp) 
DEBUG_SRC 	:= $(wildcard $(DEBUG_DIR)*.cpp) 

#obj
ASM_OBJ 	:= $(patsubst $(ASM_DIR)%.cpp, $(OBJ_DIR)%.o, $(ASM_SRC)) 
CPU_OBJ 	:= $(patsubst $(CPU_DIR)%.cpp, $(OBJ_DIR)%.o, $(CPU_SRC))
DEBUG_OBJ 	:= $(patsubst $(DEBUG_DIR)%.cpp, $(OBJ_DIR)%.o, $(DEBUG_SRC))

#exe
ASM_EXE := asm
CPU_EXE := cpu


$(CPU_EXE) : $(CPU_OBJ) $(DEBUG_OBJ)
	@$(CC) $(CFLAGS) $(INCLUDE_PATH) $(CPU_OBJ) $(DEBUG_OBJ) -o $(CPU_EXE)	

$(ASM_EXE) : $(ASM_OBJ) $(DEBUG_OBJ)
	@$(CC)  $(INCLUDE_PATH) $(ASM_OBJ) $(DEBUG_OBJ) -o $(ASM_EXE)

$(OBJ_DIR)%.o : $(CPU_DIR)%.cpp
	@$(CC)  $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(ASM_DIR)%.cpp
	@$(CC)  $(INCLUDE_PATH) -c $< -o $@

$(OBJ_DIR)%.o : $(DEBUG_DIR)%.cpp
	@$(CC) $(INCLUDE_PATH) -c $< -o $@


mkdir :
	@mkdir $(OBJ_DIR) -p

clean:
	rm $(OBJ_DIR)*.o $(OBJ_DIR)*.ii $(OBJ_DIR)*.s *.save asm cpu
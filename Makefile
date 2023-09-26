# files we're compiling
# OBJS = img_screen.cpp
# OBJS = $(SRFFILE)
OBJS = *.cpp 
OBJS = grid.cpp

# compiler
CC = g++

# options for the compiler
COMPILER_FLAGS = -w -Wall

# libraries we're linking 
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

# name of the executable
OBJ_NAME = grid


# add @ add the begining to not show the commang 
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

SOURCE ?= $(shell bash -c 'read -p "file: " file; echo $$file')
file:
	@echo file > $(SOURCE)
	OBJS = SOURCE 
	all

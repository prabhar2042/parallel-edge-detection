CXX = g++

SRC_DIR := src
OBJ_DIR := obj


EXE_NAME := seq

EXE := $(EXE_NAME)
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS = -g -ggdb -Wall -Wextra -Wpedantic -Wconversion
CFLAGS += -Wstrict-prototypes -Wwrite-strings -Wno-unused-parameter -Werror
LDFLAGS  := -Llib
LDLIBS   := 

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) 
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(OBJ_DIR)
	rm -rf $(EXE)

-include $(OBJ:.o=.d)
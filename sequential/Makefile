CC=gcc
CXX = g++
COPT = -O1
CFLAGS = -std=c99 $(COPT) -g -ggdb -Wall -Wextra -Wpedantic -Wconversion
CFLAGS += -Wstrict-prototypes -Wwrite-strings -Wno-unused-parameter -Werror


DEPS = $(wildcard *.h)
ALL_SRC_FILES := $(wildcard *.cpp)
OBJ:= $(patsubst %.cpp,%.o,$(ALL_SRC_FILES))

EXEC_NAME=main


%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CXX) -o $(EXEC_NAME) $^ $(CFLAGS) -g

clean:
	rm -rf *.o
	rm -rf $(EXEC_NAME)
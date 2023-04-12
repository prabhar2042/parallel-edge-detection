CC=gcc
CXX = g++
COPT = -O1
CFLAGS = -std=c99 $(COPT) -g -Wall -Wextra -Wpedantic -Wconversion
CFLAGS += -Wstrict-prototypes -Wwrite-strings -Wno-unused-parameter -Werror
DEPS = ppm.h image.h error.h
OBJ = main.o ppm.o image.o 

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o
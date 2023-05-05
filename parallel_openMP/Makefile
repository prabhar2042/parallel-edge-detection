

CFLAGS := -std=c++14 -fvisibility=hidden -lpthread


CFLAGS = -g -ggdb -Wall -Wextra -Wpedantic -Wconversion
CFLAGS += -Wwrite-strings -Wno-unused-parameter
CFLAGS += -O2 -fopenmp


SOURCES := src/*.cpp
HEADERS := src/*.h

TARGETBIN := parallel_mp

.SUFFIXES:
.PHONY: all clean

all: $(TARGETBIN)

$(TARGETBIN): $(SOURCES) $(HEADERS)
	$(CXX) -o $@ $(CFLAGS) $(SOURCES) 

format:
	clang-format -i src/*.cpp src/*.h

clean:
	rm -rf ./$(TARGETBIN)

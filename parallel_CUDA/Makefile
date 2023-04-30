EXECUTABLE := parallel_cuda
LDFLAGS=-L/usr/local/cuda-11.7/lib64/ -lcudart
CU_FILES   := canny.cu
CU_DEPS    :=
CC_FILES := $(notdir $(wildcard .cpp))

all: $(EXECUTABLE)

###########################################################

ARCH=$(shell uname | sed -e 's/-.//g')
OBJDIR=objs
CXX=g++ -m64
CXXFLAGS=-O3 -Wall -g
HOSTNAME=$(shell hostname)

LIBS       :=
FRAMEWORKS :=

NVCCFLAGS=-O3 -m64 --gpu-architecture=sm_61 -ccbin=/usr/bin/gcc
LIBS += GL glut cudart

LDLIBS  := $(addprefix -l, $(LIBS))
LDFRAMEWORKS := $(addprefix -framework , $(FRAMEWORKS))

NVCC=nvcc

OBJ := $(patsubst %.cpp,$(OBJDIR)/%.o,$(CC_FILES))
OBJ += $(patsubst %.cu,$(OBJDIR)/%.o,$(CU_FILES))

.PHONY: dirs clean

default: $(EXECUTABLE)

dirs:
    mkdir -p $(OBJDIR)/

clean:
    rm -rf $(OBJDIR) ~ $(EXECUTABLE) $(LOGS).ppm

export: $(EXFILES)
    cp -p $(EXFILES) $(STARTER)

$(EXECUTABLE): dirs $(OBJ)
    $(NVCC) $(NVCCFLAGS) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS) $(LDFRAMEWORKS)

$(OBJDIR)/%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cu
    $(NVCC) $(NVCCFLAGS) -c $< -o $@

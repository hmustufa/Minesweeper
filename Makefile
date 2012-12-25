# Heba Mustufa
# Minesweepr

CXX      = g++
CXXFLAGS = -Wall
INCLUDES = -Ihdr/ 

EXE  = bin/minesweeper
SRCS = src/main.cc \
       src/Square.cc \
       src/Board.cc \

OBJS = $(SRCS:.c=.o)

all: minesweeper

# Build main executable
minesweeper: $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(EXE) $(OBJS)

# Compile a .o file for each .cc    
.c.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $<  -o $@

# Remove all generated files	
clean:
	rm -rf $(EXE) src/*.o

# Clean, then make again    
re: clean all

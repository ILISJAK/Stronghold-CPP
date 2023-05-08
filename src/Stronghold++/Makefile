# Compiler
CXX=g++

# Compiler flags
CXXFLAGS=-Wall -std=c++11

# Executable output name
TARGET=stronghold

# Source files
SRCS=Archer.cpp Exception.cpp Knight.cpp Lord.cpp Maceman.cpp main.cpp Market.cpp Pikeman.cpp StoneQuarry.cpp Structure.cpp Town.cpp Troop.cpp WheatFarm.cpp WoodCamp.cpp

# Object files generated from source files
OBJS=$(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
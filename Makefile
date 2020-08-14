# Author: Michael J. Fischer
# Date: September 4, 2018
# Modified by Ferhat Erata <ferhat@cs.yale.edu> on October 10, 2019

#-----------------------------------------------------------
# Macro definitions
CXXFLAGS = -O1 -g -Wall -Wextra -Wnon-virtual-dtor -Wpedantic -Wnull-dereference -std=c++17
# CXXFLAGS = -O1 -g -Wall -std=c++17
OBJ = main.o controller.o inventory.o part.o utility.o
TARGET = repair
#-----------------------------------------------------------
# Rules
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CXX) -o $@ $(OBJ)

debug: CXXFLAGS += -DDEBUG
debug: $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
#-----------------------------------------------------------
# Dependencies
# c++ -MM *.cpp >> Makefile
controller.o: controller.cpp controller.hpp inventory.hpp part.hpp \
 exception.hpp
inventory.o: inventory.cpp inventory.hpp part.hpp exception.hpp
main.o: main.cpp controller.hpp inventory.hpp part.hpp exception.hpp \
 utility.hpp
part.o: part.cpp part.hpp exception.hpp
utility.o: utility.cpp utility.hpp

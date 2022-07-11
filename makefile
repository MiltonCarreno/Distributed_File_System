# Attempt to creating a makefile
# LDFLAGS = Linking flags
# LDLIBS = Linking libraries
# CXXFLAGS = Compiler flags
# CXX = Compiler
CXX = g++

dfs: Main.o Controller.o
	$(CXX) -o dfs Main.o Controller.o
client: client.cpp
	$(CXX) -o client client.cpp
Main.o: Main.cpp
	$(CXX) -std=c++17 -c Main.cpp
Controller.o: Controller.cpp
	$(CXX) -c Controller.cpp
clean:
	rm *.o dfs
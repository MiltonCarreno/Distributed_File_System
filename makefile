# Attempt to creating a makefile
# LDFLAGS = Linking flags
# LDLIBS = Linking libraries
# CXXFLAGS = Compiler flags
# CXX = Compiler
CXX = g++
all: dfs client storage		# Targets to run
dfs: Main.o Controller.o	# Controller Node
	$(CXX) -o dfs Main.o Controller.o
Main.o: Main.cpp
	$(CXX) -std=c++17 -c Main.cpp
Controller.o: Controller.cpp
	$(CXX) -std=c++17 -c Controller.cpp
client: Main2.o Client.o	# Client Node
	$(CXX) -o client Main2.o Client.o
Main2.o: Main2.cpp
	$(CXX) -g -std=c++17 -c Main2.cpp
Client.o: Client.cpp
	$(CXX) -g -std=c++17 -c Client.cpp
storage: Main3.o Storage.o	# Storage Node
	$(CXX) -o storage Main3.o Storage.o
Main3.o: Main3.cpp
	$(CXX) -g -std=c++17 -c Main3.cpp
Storage.o: Storage.cpp
	$(CXX) -g -std=c++17 -c Storage.cpp
clean:
	rm *.o dfs client storage
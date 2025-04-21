CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: main unit_tests

main: main.cpp algorithm.cpp algorithm.h
	$(CXX) $(CXXFLAGS) main.cpp algorithm.cpp -o main

unit_tests: unit_tests.cpp algorithm.cpp algorithm.h
	$(CXX) $(CXXFLAGS) unit_tests.cpp algorithm.cpp -o unit_tests

clean:
	rm -f main unit_tests

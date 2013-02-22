PROJ_ROOT = $(realpath ../)
TARGET = test_program
CC = g++
RM = rm -f

LDFLAGS =
CXXFLAGSS =

GTEST_INCDIR    = -I$(PROJ_ROOT)/tools/include
GTEST_LIB   = $(PROJ_ROOT)/tools/lib/gtest_main.a

BUILD_DIR = $(PROJ_ROOT)/build
SRC_DIR = $(PROJ_ROOT)/src

SRCS        = main.cpp
OBJS        = $(SRCS:.cpp=.o)

main: Simulator.o RelationalGraph.o EvaluationManager.o \
	Const.hpp UtilityFunctions.o
	$(CC) -o $@ $^

Simulator.o: Simulator.cpp
	$(CC) -o $@ -c $^

RelationalGraph.o: RelationalGraph.cpp
	$(CC) -o $@ -c $^

EvaluationManager.o: EvaluationManager.cpp
	$(CC) -o $@ -c $^

PhysicalNetwork.o: PhysicalNetwork.cpp
	$(CC) -o $@ -c $^

UtilityFunctions.o: UtilityFunctions.cpp
	$(CC) -o $@ -c $^

Simulator.cpp: Simulator.hpp
RelationalGraph.cpp: RelationalGraph.hpp
EvaluationManager.cpp: EvaluationManager.hpp
PhysicalNetwork.cpp: PhysicalNetwork.hpp
UtilityFunctions.cpp: UtilityFunctions.hpp

# テスト関連
test: mainTest.o RelationalGraphTest.o RelationalGraph.o \
	PhysicalNetworkTest.o PhysicalNetwork.o \
	Const.hpp UtilityFunctions.o
	$(CC) -o $@ $^ $(GTEST_LIB)

RelationalGraphTest.o: RelationalGraphTest.cpp
	$(CC) -o $@ -c $^ $(GTEST_INCDIR)

mainTest.o: mainTest.cpp
	$(CC) -o $@ -c $^ $(GTEST_INCDIR)

PhysicalNetworkTest.o: PhysicalNetworkTest.cpp
	$(CC) -o $@ -c $^ $(GTEST_INCDIR)

clean:
	rm *.o 
	rm main
	rm test

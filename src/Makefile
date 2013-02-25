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

MAIN_OBJ = RelationalGraph.o EventManager.o\
		   Const.hpp UtilityFunctions.o PhysicalNetwork.o\
		   EvaluationManager.o

main: $(MAIN_OBJ) Simulator.o
	$(CC) -o $@ $^

Simulator.o: Simulator.cpp
	$(CC) -o $@ -c $^

RelationalGraph.o: RelationalGraph.cpp
	$(CC) -o $@ -c $^

EventManager.o: EventManager.cpp
	$(CC) -o $@ -c $^

PhysicalNetwork.o: PhysicalNetwork.cpp
	$(CC) -o $@ -c $^

UtilityFunctions.o: UtilityFunctions.cpp
	$(CC) -o $@ -c $^

EvaluationManager.o: EvaluationManager.cpp
	$(CC) -o $@ -c $^

Simulator.cpp: Simulator.hpp
RelationalGraph.cpp: RelationalGraph.hpp
EventManager.cpp: EventManager.hpp
PhysicalNetwork.cpp: PhysicalNetwork.hpp
UtilityFunctions.cpp: UtilityFunctions.hpp
EvaluationManager.cpp: EvaluationManager.hpp

# テスト関連
test: $(MAIN_OBJ) mainTest.o RelationalGraphTest.o \
	PhysicalNetworkTest.o EventManagerTest.o
	$(CC) -o $@ $^ $(GTEST_LIB)

RelationalGraphTest.o: RelationalGraphTest.cpp
	$(CC) -o $@ -c $^ $(GTEST_INCDIR)

mainTest.o: mainTest.cpp
	$(CC) -o $@ -c $^ $(GTEST_INCDIR)

PhysicalNetworkTest.o: PhysicalNetworkTest.cpp
	$(CC) -o $@ -c $^ $(GTEST_INCDIR)

EventManagerTest.o: EventManagerTest.cpp
	$(CC) -o $@ -c $^ $(GTEST_INCDIR)

clean:
	rm *.o 
	rm main
	rm test

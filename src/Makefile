PROJ_ROOT = $(realpath ../)
TARGET = test_program
CC = g++
RM = rm -f

LDFLAGS =
CXXFLAGSS= -g -O3

GTEST_INCDIR    = -I$(PROJ_ROOT)/tools/include
GTEST_LIB   = $(PROJ_ROOT)/tools/lib/gtest_main.a

BUILD_DIR = $(PROJ_ROOT)/build
SRC_DIR = $(PROJ_ROOT)/src

SRCS        = main.cpp
OBJS        = $(SRCS:.cpp=.o)

MAIN_OBJ = RelationalGraph.o EventManager.o\
		   Const.hpp UtilityFunctions.o PhysicalNetwork.o\
		   EvaluationManager.o ContentPacket.o

main: $(MAIN_OBJ) Simulator.o
	$(CC) $(CXXFLAGSS) -o $@ $^

Simulator.o: Simulator.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^

RelationalGraph.o: RelationalGraph.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^

EventManager.o: EventManager.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^

PhysicalNetwork.o: PhysicalNetwork.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^

UtilityFunctions.o: UtilityFunctions.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^

EvaluationManager.o: EvaluationManager.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^

ContentPacket.o: ContentPacket.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^

Simulator.cpp: Simulator.hpp
RelationalGraph.cpp: RelationalGraph.hpp
EventManager.cpp: EventManager.hpp
PhysicalNetwork.cpp: PhysicalNetwork.hpp
UtilityFunctions.cpp: UtilityFunctions.hpp
EvaluationManager.cpp: EvaluationManager.hpp
ContentPacket.cpp: ContentPacket.hpp

# テスト関連
test: $(MAIN_OBJ) mainTest.o RelationalGraphTest.o \
	PhysicalNetworkTest.o EventManagerTest.o
	$(CC) $(CXXFLAGSS) -o $@ $^ $(GTEST_LIB)

RelationalGraphTest.o: RelationalGraphTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR)

mainTest.o: mainTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR)

PhysicalNetworkTest.o: PhysicalNetworkTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR)

EventManagerTest.o: EventManagerTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR)

clean:
	rm *.o 
	rm main
	rm test

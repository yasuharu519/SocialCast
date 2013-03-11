PROJ_ROOT = $(realpath ../)
TARGET = test_program
CC = g++
RM = rm -f

LDFLAGS =
CXXFLAGSS= -g -O3

BOOST_INCDIR = -I/opt/local/include/boost
GTEST_INCDIR  = -I$(PROJ_ROOT)/tools/include
GTEST_LIB   = $(PROJ_ROOT)/tools/lib/gtest_main.a

BUILD_DIR = $(PROJ_ROOT)/build
SRC_DIR = $(PROJ_ROOT)/src

SRCS        = main.cpp
OBJS        = $(SRCS:.cpp=.o)

MAIN_OBJ = RelationalGraph.o EventManager.o\
		   Const.hpp UtilityFunctions.o PhysicalNetwork.o\
		   EvaluationManager.o

main: $(MAIN_OBJ) Simulator.o
	$(CC) $(CXXFLAGSS) -o $@ $^ $(BOOST_INCDIR)

Simulator.o: Simulator.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(BOOST_INCDIR)

RelationalGraph.o: RelationalGraph.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(BOOST_INCDIR)

EventManager.o: EventManager.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(BOOST_INCDIR)

PhysicalNetwork.o: PhysicalNetwork.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(BOOST_INCDIR)

UtilityFunctions.o: UtilityFunctions.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(BOOST_INCDIR)

EvaluationManager.o: EvaluationManager.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(BOOST_INCDIR)

Simulator.cpp: Simulator.hpp
RelationalGraph.cpp: RelationalGraph.hpp
EventManager.cpp: EventManager.hpp
PhysicalNetwork.cpp: PhysicalNetwork.hpp
UtilityFunctions.cpp: UtilityFunctions.hpp
EvaluationManager.cpp: EvaluationManager.hpp

# テスト関連
test: $(MAIN_OBJ) mainTest.o RelationalGraphTest.o \
	PhysicalNetworkTest.o EventManagerTest.o
	$(CC) $(CXXFLAGSS) -o $@ $^ $(GTEST_LIB) $(BOOST_INCDIR)

RelationalGraphTest.o: RelationalGraphTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR) $(BOOST_INCDIR)

mainTest.o: mainTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR) $(BOOST_INCDIR)

PhysicalNetworkTest.o: PhysicalNetworkTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR) $(BOOST_INCDIR)

EventManagerTest.o: EventManagerTest.cpp
	$(CC) $(CXXFLAGSS) -o $@ -c $^ $(GTEST_INCDIR) $(BOOST_INCDIR)

clean:
	rm *.o 
	rm main
	rm test

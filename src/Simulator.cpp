#include "Simulator.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

using namespace std;

// Simulator Class
Simulator::Simulator()
{
    EvaluationManager* evaluationManager = new EvaluationManager();
    RelationalGraph* relationalGraph = new RelationalGraph();
    PhysicalNetwork* physicalNetwork = new PhysicalNetwork(relationalGraph, evaluationManager);
    physicalNodeIDList = physicalNetwork->getPhysicalNodeIDList();
}

Simulator::~Simulator()
{
    delete evaluationManager;
    delete relationalGraph;
    delete physicalNetwork;
}

// public functions
void Simulator::doSimulation()
{
    
}

// private functions
Vertex Simulator::chooseContentRequestedUser()
{
    using namespace boost;
    mt19937 gen(static_cast<unsigned long>(time(0)));
    uniform_smallint<> dst(0, physicalNodeIDList.size()-2); // 最後のIDはdistributorとなっているため
    variate_generator<mt19937&, uniform_smallint<> > rand(gen, dst);
    
    return rand();
}


int main(int argc, char* argv[])
{
    char program[256];
    char method[256];
    strcpy(program, argv[0]);
    cout << program << endl;
    if(argc < 2)
    {
        cout << "Usage: " << program << " [method] " << endl;
        exit(1);
    }
    else
    {
        
    }
}


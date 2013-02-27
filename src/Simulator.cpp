#include "Simulator.hpp"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

using namespace std;

// Simulator Class
Simulator::Simulator()
{
    evaluationManager = new EvaluationManager();
    relationalGraph = new RelationalGraph();
    physicalNetwork = new PhysicalNetwork(relationalGraph, evaluationManager);
    eventManager = new EventManager();
}

Simulator::~Simulator()
{
    delete evaluationManager;
    delete relationalGraph;
    delete physicalNetwork;
    delete eventManager;
}

// public functions
void Simulator::doSimulation()
{
    // 最初のイベントの追加
    Event* event = new ContentRequestedEvent(0.0);
    double time;
    while(!(eventManager->isEmpty()))
    {
        event = eventManager->popEvent();
        if(typeid(event) == typeid(ContentRequestedEvent))
        {
            doContentRequest();
        }
        else if(typeid(event) == typeid(ReceivePacketEvent))
        {
            doReceivePacket();
        }
        else if(typeid(event) == typeid(SendPacketEvent))
        {
            doSendPacket();
        }
        else{
            cout << "Error: Event class should not be in eventManager" << endl;
            std::abort();
        }
        delete event;
    }
}

void Simulator::doContentRequest()
{

}

void Simulator::doReceivePacket()
{

}

void Simulator::doSendPacket()
{

}

// private functions


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
        Simulator* simulator = new Simulator();
        simulator->doSimulation();
        delete simulator;
    }
}


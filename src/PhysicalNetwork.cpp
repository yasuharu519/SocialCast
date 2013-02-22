#include "PhysicalNetwork.hpp"

///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////
PhysicalNetwork::PhysicalNetwork()
{
    RelationalGraph* graph = new RelationalGraph();
    userList = graph->getUserList();
    contentList = graph->getContentList();
    distributorID = graph->size();
    VertexList::iterator it;
    int physicalID = 0;
    for(it = userList.begin(); it != userList.end(); ++it)
    {
        succ.push_back(VertexList());
        pred.push_back(VertexList());
        registerIDMapping(physicalID, (*it));
        physicalID++;
    }
    succ.push_back(VertexList());
    pred.push_back(VertexList());
    registerIDMapping(physicalID, distributorID);
    setRandomGeometricPosition();
    connectWithNeighbors();
    delete graph;
}
PhysicalNetwork::~PhysicalNetwork()
{
    
}

////////////////////////////////////////////////////////////////////////////////
// Public
///////////////////////////////////////////////////////////////////////////////
Edge PhysicalNetwork::add_edge(Vertex tail, Vertex head)
{
    succ[tail].push_back(head);
    pred[head].push_back(tail);
    return Edge(tail, head);
}

///////////////////////////////////////////////////////////////////////////////
// Private
///////////////////////////////////////////////////////////////////////////////
void PhysicalNetwork::setRandomGeometricPosition()
{
    using namespace boost;
    userPositionList.push_back(UserPosition(distributorID, Position(0.0, 0.0)));
    VertexList::iterator it;
    mt19937 gen( static_cast<unsigned long>(time(0)) );
    uniform_real<> dst( 0.1, PHYSICAL_NETWORK_MAP_RANGE);
    variate_generator<mt19937&, uniform_real<> > rand( gen, dst );
    for(it = userList.begin(); it != userList.end(); ++it)
    {
        Position position = Position(rand() - PHYSICAL_NETWORK_MAP_RANGE / 2.0,
                rand() - PHYSICAL_NETWORK_MAP_RANGE / 2.0);
        userPositionList.push_back(UserPosition(*it, position));
    }
}

void PhysicalNetwork::connectWithNeighbors()
{
    Position p1, p2;
    for(int i = 0; i < userPositionList.size(); ++i)
    {
        p1 = userPositionList[i].second;
        for(int j = i + 1; j < userPositionList.size(); ++j)
        {
            p2 = userPositionList[j].second;
            if(calcPhysicalDistance(p1, p2) <= PHYSICAL_NETWORK_CONNECT_RANGE)
            {
                add_edge(i, j);
            }
        }
    }
}


double PhysicalNetwork::calcPhysicalDistance(Position p1, Position p2)
{
    return sqrt(pow(p2.second - p1.second, 2.0) + pow(p2.first - p1.first, 2.0));
}

void PhysicalNetwork::registerIDMapping(Vertex physicalID, Vertex relationalID)
{
        relationalToPhysical[relationalID] = physicalID;
        physicalToRelational[physicalID] = relationalID;
}

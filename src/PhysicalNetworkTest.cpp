#include "gtest/gtest.h"
#include "PhysicalNetwork.hpp"
#include "RelationalGraph.hpp"

class PhysicalNetworkTest: public ::testing::Test{
    public:
        RelationalGraph* graph = new RelationalGraph();
        PhysicalNetwork* network = new PhysicalNetwork(graph);
};


//constructor
TEST_F(PhysicalNetworkTest, TestUserList){
    EXPECT_EQ(3897, network->getUserNodeNum());
}

TEST_F(PhysicalNetworkTest, TestContentList){
    EXPECT_EQ(5209, network->contentList.size());
}

TEST_F(PhysicalNetworkTest, TestConnectivity){
    network->setPositionUntilAllConnected();
    EXPECT_EQ(true, network->checkConnectivity());
}

TEST_F(PhysicalNetworkTest, TestShortestPath)
{
    int testID = network->relationalToPhysical[network->distributorID];
    VertexList neighborOfDistributor = network->neighbor[testID];
    VertexList *expectedList;
    foreach(Vertex v, neighborOfDistributor)
    {
        expectedList = new VertexList();
        expectedList->push_back(testID);
        expectedList->push_back(v);
        EXPECT_EQ(*expectedList, network->searchPhysicalShortestPath(testID, v));
        delete expectedList;
    }
}

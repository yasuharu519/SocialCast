#include "gtest/gtest.h"
#include "PhysicalNetwork.hpp"
#include "RelationalGraph.hpp"
#include "EvaluationManager.hpp"

class PhysicalNetworkTest: public ::testing::Test{
    public:
        PhysicalNetworkTest();
        ~PhysicalNetworkTest();
        RelationalGraph* graph;
        EvaluationManager* evaluationManager;
        PhysicalNetwork* network;
};

PhysicalNetworkTest::PhysicalNetworkTest()
{
    graph = new RelationalGraph();
    evaluationManager = new EvaluationManager();
    network = new PhysicalNetwork(graph, evaluationManager);
}

PhysicalNetworkTest::~PhysicalNetworkTest()
{
    delete graph;
    delete evaluationManager;
    delete network;
}


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

TEST_F(PhysicalNetworkTest, TestChooseRequestContentIsActuallyContentFromAllUserNode)/*{{{*/
{
    int selectedContent;
    //for(int i = 0; i < (network->getUserNodeNum() - 1); ++i)
    for(int i = 0; i < 100; ++i)
    {
        selectedContent = network->chooseRequestContent(i);
        EXPECT_EQ(1, graph->vertexTypeMap[selectedContent]);
    }
}/*}}}*/

TEST_F(PhysicalNetworkTest, isSendingToAndsetSendingTo)/*{{{*/
{
    EXPECT_NE(true, network->isSendingTo(1, 2));
    network->setSendingTo(1, 2, true);
    EXPECT_EQ(true, network->isSendingTo(1, 2));
}/*}}}*/


// パス検索系
TEST_F(PhysicalNetworkTest, TestShortestPath)/*{{{*/
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
}/*}}}*/

TEST_F(PhysicalNetworkTest, searchPhysicalShortestPath)/*{{{*/
{
    int selectedContent;
    int packetID;
    for(int i = 0; i < 10; ++i)
    {
        selectedContent = network->chooseRequestContent(i);
        // パケットIDがどんどん増えていくはず
        EXPECT_EQ(i, network->searchPhysicalShortestPathFromRequestedUser(i, selectedContent));
    }
}/*}}}*/

TEST_F(PhysicalNetworkTest, searchProposedPathFromRequestedUser)
{
    int selectedContent;
    int packetID;
    selectedContent = network->chooseRequestContent(0);
    // パケットIDがどんどん増えていくはず
    EXPECT_EQ(0, network->searchProposedPathFromRequestedUser(0, selectedContent));
}

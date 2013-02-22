#include "gtest/gtest.h"
#include "RelationalGraph.hpp"

class RelationalGraphTest: public ::testing::Test{
    public:
        RelationalGraph* graph = new RelationalGraph();
};


//getEdgeWeight
TEST_F(RelationalGraphTest, TestGetEdgeWeight){
    EXPECT_EQ(1, graph->getEdgeWeight(0, 1));
    EXPECT_EQ(1, graph->getEdgeWeight(0, 2));
    EXPECT_EQ(1, graph->getEdgeWeight(0, 3));
    EXPECT_EQ(0.5, graph->getEdgeWeight(3, 4));
    EXPECT_EQ(0.333333, graph->getEdgeWeight(3, 12));

}

//size
TEST_F(RelationalGraphTest, TestSize){
    EXPECT_EQ(9229, graph->size());
}

// DijkstraShortestPath
TEST_F(RelationalGraphTest, TestDijkstraShortestPathLengthSameVertex){
    EXPECT_EQ(0, graph->dijkstraShortestPathLength(0, 0));
}

TEST_F(RelationalGraphTest, TestDijkstraShortestPathLengthDirectLink){
    EXPECT_EQ(1, graph->dijkstraShortestPathLength(0, 1));
}

TEST_F(RelationalGraphTest, TestDijkstraShortestPathLengthRandomChoice){
    EXPECT_EQ(3.458333, graph->dijkstraShortestPathLength(0, 9228));
}

TEST_F(RelationalGraphTest, TestGetUserList){
    EXPECT_EQ(3896, graph->getUserList().size());
}

TEST_F(RelationalGraphTest, TestGetContentList){
    EXPECT_EQ(5209, graph->getContentList().size());
}

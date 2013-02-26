#include "gtest/gtest.h"
#include "RelationalGraph.hpp"
#include "algorithm"
#include "UtilityFunctions.hpp"

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

TEST_F(RelationalGraphTest, TestLinkBidirectional)/*{{{*/
{
    int size = graph->size();
    graph->add_node(size);
    graph->add_node(size+1);
    graph->add_edge(size, size+1);
    bool flag1, flag2, flag3, flag4;
    flag1 = false;
    flag2 = false;
    flag3 = false;
    flag4 = false;
    for(int i = 0; i < graph->pred[size+1].size(); ++i)
    {
        if(size == graph->pred[size+1][i])
        {
            flag1 = true;
        }
        if(size + 1 == graph->pred[size][i])
        {
            flag2 = true;
        }
    }
    for(int i = 0; i < graph->succ[size].size(); ++i)
    {
        if(size + 1 == graph->succ[size][i])
        {
            flag3 = true;
        }
        if(size == graph->succ[size+1][i])
        {
            flag4 = true;
        }
    }
    EXPECT_EQ(true, flag1);
    EXPECT_EQ(true, flag2);
    EXPECT_EQ(true, flag3);
    EXPECT_EQ(true, flag4);
}/*}}}*/

TEST_F(RelationalGraphTest, TestLinkNumberCheck)
{
    // リンクの数が、predとsuccでおなじかどうか確かめる
    // check bidirectionary
    int size = graph->size();
    int pred_num, succ_num;
    for(int i = 0; i < size; ++i)
    {
        pred_num = graph->pred[i].size();
        succ_num = graph->succ[i].size();
        EXPECT_EQ(pred_num, succ_num);
    }

}

TEST_F(RelationalGraphTest, TestRankIsSetForAllContents)
{
    // すべてのコンテンツに対してランクが設定されているか
    VertexList contentsList = graph->getContentList();
    int _id, rank;
    for(int i = 0; i < contentsList.size(); ++i)
    {
        _id = contentsList[i];
        rank = graph->getRank(_id);
        EXPECT_NE(-1, rank);
    }
}

bool rankSortLess(const pair<int, int>&a, const pair<int, int>&b)
{
    return a.second < b.second;
}

TEST_F(RelationalGraphTest, TestRankOrder)
{
    using namespace std;
    // コンテンツのランクのオーダーを調べる
    vector<pair<int, int> > IDAndRank;
    VertexList contentList = graph->getContentList();
    int rank;
    int link_count;
    int contentID;
    bool result = true;
    for(int i = 0; i < contentList.size(); ++i)
    {
        rank = graph->getRank(contentList[i]);
        IDAndRank.push_back(pair<int, int>(contentList[i], rank));
    }
    sort(IDAndRank.begin(), IDAndRank.end(), rankSortLess);
    vector<int> linkNumList;
    for(int i = 0; i < IDAndRank.size(); ++i)
    {
        link_count = 0;
        contentID = IDAndRank[i].first;
        for(int j = 0; j < graph->succ[contentID].size(); ++j)
        {
            if(graph->vertexTypeMap[graph->succ[contentID][j]] == 0 )// ユーザであることを確かめる
            {
                ++link_count;
            }
        }
        linkNumList.push_back(link_count);
    }
    for(int i = 0; i < linkNumList.size() - 1; ++i)
    {
        if(linkNumList[i] < linkNumList[i+1])
        {
            result = false;
        }
    }
    EXPECT_EQ(true, result);

}

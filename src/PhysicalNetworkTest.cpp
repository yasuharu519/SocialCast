#include "gtest/gtest.h"
#include "PhysicalNetwork.hpp"

class PhysicalNetworkTest: public ::testing::Test{
    public:
        PhysicalNetwork* network = new PhysicalNetwork();
};


//constructor
TEST_F(PhysicalNetworkTest, TestUserList){
    EXPECT_EQ(3896, network->userList.size());
}

TEST_F(PhysicalNetworkTest, TestContentList){
    EXPECT_EQ(5209, network->contentList.size());
}

TEST_F(PhysicalNetworkTest, TestConnectivity){
    network->setPositionUntilAllConnected();
    EXPECT_EQ(true, network->checkConnectivity());
}

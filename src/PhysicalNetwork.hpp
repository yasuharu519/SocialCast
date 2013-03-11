//  
//  Created by 澤田 泰治 on 2012/12/03.
//  Copyright (c) 2012年 澤田 泰治. All rights reserved.
//

#ifndef __PHYSICAL_NETWORK_HPP_
#define __PHYSICAL_NETWORK_HPP_

#include <utility>
#include <string>
#include "Const.hpp"
#include "RelationalGraph.hpp"
#include <vector>
#include <map>
#include <cstdlib>
#include <boost/random.hpp>
#include <complex>
#include <queue>
#include <algorithm>
#include "UtilityFunctions.hpp"
#include "EvaluationManager.hpp"

typedef int Vertex;
typedef pair<double, double> Position;
typedef pair<Vertex, Position> UserPosition;
typedef vector<UserPosition> UserPositionList;
typedef map<Vertex, Vertex> NodeIDMapping;
typedef map<NodePair, VertexList> ShortestPathMap;
typedef Vertex ContentID;

using namespace std;
using namespace boost;

class PhysicalNetwork
{
    public:
        PhysicalNetwork(RelationalGraph* graph, EvaluationManager* _evaluationManager);
        virtual ~PhysicalNetwork();
        //
        VertexList userList;
        VertexList contentList;
        UserPositionList userPositionList;
        AdjacencyList neighbor;
        NodeIDMapping relationalToPhysical, physicalToRelational;
        ShortestPathMap shortestPathMap;
        WeightProperty weight;
        int distributorID;
        int distributorPhysicalID;
        //
        Edge add_edge(Vertex, Vertex);
        bool checkConnectivity();
        void setPositionUntilAllConnected();
        int getUserNodeNum();
        VertexList getPhysicalNodeIDList();
        ContentID chooseRequestContent(Vertex _physicalID);
        Vertex chooseRequestUser();
        bool isSendingTo(int _from, int _to);
        void setSendingTo(int _from, int _to, bool _bool);
        Vertex getUserOnPathIndexWithPacketID(int _packetID, int _index);
        bool isLastUserToReceivePacket(int _packetID, int _index);
        // パス検索系
        VertexList searchPhysicalShortestPath(const Vertex &node_from, const Vertex &node_to);
        int searchPhysicalShortestPathFromRequestedUser(const Vertex &requestedUser, const Vertex &content);
        int searchProposedPathFromRequestedUser(const Vertex &requestedUser, const Vertex &content);
        // キャッシュ管理関連
        bool nodeHasContent(const Vertex &user, const Vertex &conent);
    private:
        int userNodeNum;
        RelationalGraph* relationalGraph;
        EvaluationManager* evaluationManager;
        VertexList physicalNodeIDList;
        vector< map< int, bool >* > isSendingToMap; // 現在送信中か調べるmap
        map<int, VertexList > packetIDAndPacketPathMap;
        map<int, ContentID> packetIDAndContentIDMap;
        int nextPacketID; // パケットID作成用の変数
        // キャッシュ管理関連
        vector< VertexList > userContentList;
        //
        void setRandomGeometricPosition();
        void connectWithNeighbors();
        double calcPhysicalDistance(const Position&, const Position&);
        void registerIDMapping(const Vertex&, const Vertex&);
        VertexList resolvePath(const int *prev, const Vertex &node_from, const Vertex &node_to);

        // ランダム発生用
        mt19937 gen;
        uniform_int<> requestUserDST;
        uniform_real<> geometricPositionDST;

};

#endif /* defined(__SocialCast__Network__) */


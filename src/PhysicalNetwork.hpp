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
typedef Vertex Content;

using namespace std;

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
        //
        Edge add_edge(Vertex, Vertex);
        bool checkConnectivity();
        void setPositionUntilAllConnected();
        VertexList searchPhysicalShortestPath(const Vertex &node_from, const Vertex &node_to);
        int getUserNodeNum();
        VertexList getPhysicalNodeIDList();
        Content chooseRequestContent(Vertex _physicalID);
        Vertex chooseRequestUser();
    private:
        int userNodeNum;
        RelationalGraph* relationalGraph;
        EvaluationManager* evaluationManager;
        VertexList physicalNodeIDList;
        //
        void setRandomGeometricPosition();
        void connectWithNeighbors();
        double calcPhysicalDistance(const Position&, const Position&);
        void registerIDMapping(const Vertex&, const Vertex&);
        VertexList resolvePath(const int *prev, const Vertex &node_from, const Vertex &node_to);
};

#endif /* defined(__SocialCast__Network__) */


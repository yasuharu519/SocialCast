#include "PhysicalNetwork.hpp"


///////////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
///////////////////////////////////////////////////////////////////////////////
PhysicalNetwork::PhysicalNetwork()/*{{{*/
{
    RelationalGraph* graph = new RelationalGraph();
    userList = graph->getUserList();
    contentList = graph->getContentList();
    distributorID = graph->size();
    VertexList::iterator it;
    int physicalID = 0;
    for(it = userList.begin(); it != userList.end(); ++it)
    {
        neighbor.push_back(VertexList());
        registerIDMapping(physicalID, (*it));
        physicalID++;
    }
    neighbor.push_back(VertexList());
    registerIDMapping(physicalID, distributorID);
    setRandomGeometricPosition();
    connectWithNeighbors();
    delete graph;
    userNodeNum = userPositionList.size();
} /*}}}*/

PhysicalNetwork::~PhysicalNetwork()/*{{{*/
{
    
}/*}}}*/

////////////////////////////////////////////////////////////////////////////////
// Public
///////////////////////////////////////////////////////////////////////////////
Edge PhysicalNetwork::add_edge(Vertex tail, Vertex head)/*{{{*/
{
    neighbor[tail].push_back(head);
    neighbor[head].push_back(tail);
    // 各エッジの重みの設定
    weight[Edge(tail, head)] = 1.0;
    weight[Edge(head, tail)] = 1.0;
    return Edge(tail, head);
}/*}}}*/

bool PhysicalNetwork::checkConnectivity(){/*{{{*/
    // distributorから他のすべてのノードとの接続性を確認する
    bool *f = new bool[userPositionList.size()]; // distributorIDは常に他のユーザ数+1
    for(int j= 0; j < userPositionList.size(); j++){
        f[j] = false;
    }
    queue<Vertex> q;
    q.push(relationalToPhysical[distributorID]);
    int count = 0;
    while (!q.empty()) {
        int u;
        int physicalID;
        u = q.front(); q.pop();
        if (f[u]) continue; 
        f[u] = true;
        foreach (Vertex v, neighbor[u])
        {
            if (f[v] == false)
            {
                q.push(v);
            }
        }
    }
    for(int i = 0; i < userList.size() + 1; ++i)
    {
        if(f[i] == false)
        {
            return false;
        }
    }
    return true;
}/*}}}*/

void PhysicalNetwork::setPositionUntilAllConnected()/*{{{*/
{
    while(!checkConnectivity()){
        // 位置情報のクリア
        userPositionList.clear();
        // 接続情報のクリア
        for(int i = 0; i < neighbor.size(); ++i)
        {
            neighbor[i].clear();
        }
        // 再度場所の決定
        setRandomGeometricPosition();
        // 再度接続情報の更新
        connectWithNeighbors();
    }
}/*}}}*/

VertexList PhysicalNetwork::searchPhysicalShortestPath(const Vertex &node_from, const Vertex &node_to, const Content &content)/*{{{*/
{
    // TODO: 関係性グラフのほうの重みの計算について、どのようにしてるのか確認する
    // メモされているものから取り出す
    ShortestPathMap::iterator it;
    it = shortestPathMap.find(NodePair(node_from, node_to));
    if(it != shortestPathMap.end()){
        return it->second;
    }
    // 使用する変数の初期化
    bool *f = new bool[userNodeNum];
    Weight *dist = new Weight[userNodeNum];
    int *prev = new int[userNodeNum];
    fill_n(dist, userNodeNum, INT_MAX), dist[node_from] = 0, fill_n(prev, userNodeNum, -1);
    for(int j= 0; j < userNodeNum; j++){
        f[j] = false;
    }
    typedef pair<Weight, int> Distance;
    priority_queue<Distance, vector<Distance>, less<Distance> > q;
    q.push(Distance(0, node_from));
    while (!q.empty()) {
        int u;
        Weight d_u;
        boost::tuples::tie(d_u, u) = q.top(), q.pop();
        if (f[u]) continue; 
        f[u] = true;
        shortestPathMap[NodePair(node_from, u)] = resolvePath(prev, node_from, u);
        if(node_to == u){
            return shortestPathMap[NodePair(node_from, node_to)];
        }
        foreach (Vertex v, neighbor[u])
        {
            if (dist[v] > d_u + weight(u, v))
            {
                prev[v] = u, q.push(Distance(dist[v] = d_u + weight(u, v), v));
            }
        }
    }
    return shortestPathMap[NodePair(node_from, node_to)];
}/*}}}*/

int PhysicalNetwork::getUserNodeNum()
{
    return userNodeNum;
}

///////////////////////////////////////////////////////////////////////////////
// Private
///////////////////////////////////////////////////////////////////////////////
void PhysicalNetwork::setRandomGeometricPosition()/*{{{*/
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
}/*}}}*/

void PhysicalNetwork::connectWithNeighbors()/*{{{*/
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
}/*}}}*/

double PhysicalNetwork::calcPhysicalDistance(const Position &p1, const Position &p2)/*{{{*/
{
    return sqrt(pow(p2.second - p1.second, 2.0) + pow(p2.first - p1.first, 2.0));
}/*}}}*/

void PhysicalNetwork::registerIDMapping(const Vertex &physicalID, const Vertex &relationalID)/*{{{*/
{
        relationalToPhysical[relationalID] = physicalID;
        physicalToRelational[physicalID] = relationalID;
}/*}}}*/

VertexList PhysicalNetwork::resolvePath(const int *prev, const Vertex &node_from, const Vertex &node_to)/*{{{*/
{
    VertexList path;
    Vertex u = node_from;
    path.push_back(u);
    while(u != node_to)
    {
        u = prev[u];
        path.push_back(u);
    }
    reverse(path.begin(), path.end());
    return path;
}/*}}}*/

#include "RelationalGraph.hpp"
#include "Const.hpp"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
RelationalGraph::RelationalGraph(){
    nodeNum = 0;
    loadNodeLabel();
    loadEdges();
    loadVertexType();
    setUserList();
    setContentList();
}

RelationalGraph::~RelationalGraph(){
}

//////////////////////////////////////////////////////////////////////
// Public
//////////////////////////////////////////////////////////////////////
Vertex RelationalGraph::add_node(int node_id){/*{{{*/
    nodeNum++;
    succ.push_back(VertexList());
    pred.push_back(VertexList());
    return node_id;
}/*}}}*/

Edge RelationalGraph::add_edge(const Vertex &tail, const Vertex &head){/*{{{*/
    succ[tail].push_back(head);
    pred[head].push_back(tail);
    // 双方向のため以下の追加
    succ[head].push_back(tail);
    pred[tail].push_back(head);
    return Edge(tail, head);
}/*}}}*/

string RelationalGraph::getLabel(int node_id){/*{{{*/
    return labelMap[node_id];
}/*}}}*/

Weight RelationalGraph::getEdgeWeight(const Vertex &tail, const Vertex &head){/*{{{*/
    return weight(tail, head);
}/*}}}*/

double RelationalGraph::dijkstraShortestPathLength(const Vertex &node_from, const Vertex &node_to){/*{{{*/
    ShortestMap::iterator it;
    it = shortestmap.find(NodePair(node_from, node_to));
    if(it != shortestmap.end()){
        return it->second;
    }
    bool *f = new bool[nodeNum];
    Weight *dist = new Weight[nodeNum];
    int *prev = new int[nodeNum];
    fill_n(dist, nodeNum, INT_MAX), dist[node_from] = 0, fill_n(prev, nodeNum, -1);
    for(int j= 0; j < nodeNum; j++){
        f[j] = false;
    }
    typedef pair<Weight, int> Distance;
    priority_queue<Distance, vector<Distance>, greater<Distance> > q;
    q.push(Distance(0, node_from));
    while (!q.empty()) {
        int u;
        Weight d_u;
        boost::tuples::tie(d_u, u) = q.top(), q.pop();
        if (f[u]) continue; 
        f[u] = true;
        shortestmap[NodePair(node_from, u)] = dist[u];
        if(node_to == u){
            //return shortestmap[NodePair(node_from, node_to)];
            return dist[u];
        }
        foreach (Vertex v, succ[u])
        {
            if (dist[v] > d_u + weight(u, v))
            {
                prev[v] = u, q.push(Distance(dist[v] = d_u + weight(u, v), v));
            }
        }
    }
    return shortestmap[NodePair(node_from, node_to)];
}/*}}}*/

int RelationalGraph::size(){/*{{{*/
    return nodeNum;
}/*}}}*/

VertexList RelationalGraph::getUserList(){/*{{{*/
    return userList;
}/*}}}*/

VertexList RelationalGraph::getContentList(){/*{{{*/
    return contentList;
}/*}}}*/

bool rankSort(const pair<long, long> &a, const pair<long, long> &b)
{
    return a.second > b.second;
}

void RelationalGraph::setRank()
{
    vector<pair<long, long> > rank;
    for(int i = 0; i < nodeNum; ++i)
    {
        rank.push_back(pair<long, long>(i, succ[i].size()));
    }
    sort(rank.begin(), rank.end(), rankSort);
}

//////////////////////////////////////////////////////////////////////
// Private
//////////////////////////////////////////////////////////////////////
void RelationalGraph::loadNodeLabel(){/*{{{*/
    ifstream label_fs(RELATIONAL_GRAPH_NODENAME_FILEPATH);
    string label_buff;
    char label[1024];
    int node_id;
    VertexLabelMap::iterator it;
    while(getline(label_fs, label_buff)){
        sscanf(label_buff.data(), "%d %s", &node_id, label);
        it = labelMap.find(node_id);
        if(it == labelMap.end()){
            string label_str = string(label);
            labelMap.insert(VertexLabelKV(node_id, label));
            add_node(node_id);
        }
    }
}/*}}}*/

void RelationalGraph::loadEdges(){/*{{{*/
    ifstream weight_fs(RELATIONAL_GRAPH_WEIGHT_FILEPATH);
    ifstream link_fs(RELATIONAL_GRAPH_LINK_FILEPATH);
    string weight_buff, link_buff;
    double link_weight;
    int node_from;
    int node_to;
    while(getline(weight_fs, weight_buff) && getline(link_fs, link_buff)){
        sscanf(weight_buff.data(), "%lf", &link_weight);
        sscanf(link_buff.data(), "%d %d", &node_from, &node_to);
        weight[add_edge(node_from, node_to)] = link_weight;
        weight[add_edge(node_to, node_from)] = link_weight;
    }
}/*}}}*/

void RelationalGraph::loadVertexType(){/*{{{*/
    ifstream type_fs(RELATIONAL_GRAPH_NODETYPE_FILEPATH);
    string type_buff;
    int type;
    int node_id;
    while(getline(type_fs, type_buff)){
        sscanf(type_buff.data(), "%d %d", &node_id, &type);
        vertexTypeMap[node_id] = type;
    }
}/*}}}*/

void RelationalGraph::setUserList(){/*{{{*/
    VertexTypeMap::iterator it;
    for(it = vertexTypeMap.begin(); it != vertexTypeMap.end(); ++it){
        if(it->second == 0){
            userList.push_back(it->first);
        }
    }
}/*}}}*/

void RelationalGraph::setContentList(){/*{{{*/
    VertexTypeMap::iterator it;
    for(it = vertexTypeMap.begin(); it != vertexTypeMap.end(); ++it){
        if(it->second == 1){
            contentList.push_back(it->first);
        }
    }
}/*}}}*/

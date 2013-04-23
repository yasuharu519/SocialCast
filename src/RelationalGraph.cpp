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
    setRank();
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
    // キャッシュから取り出し
    if(node_from < node_to)
    {
        it = shortestmap.find(NodePair(node_from, node_to));
    }
    else
    {
        it = shortestmap.find(NodePair(node_to, node_from));
    }
    // キャッシュから取り出し終わり
    if(it != shortestmap.end()){
        //cout << "dist" << node_from << ", " << node_to << ", : " << it->second << endl;
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
        // キャッシュへの登録
        // 小さいものを前とする
        if(node_from < u)
        {
            shortestmap[NodePair(node_from, u)] = dist[u];
        }
        else
        {
            shortestmap[NodePair(u, node_from)] = dist[u];
        }
        // キャッシュへの登録終わり
        if(node_to == u){
            //return shortestmap[NodePair(node_from, node_to)];
            //cout << "dist" << node_from << ", " << node_to << ", : " << dist[u] << endl;
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

bool linkNumSortGreater(const pair<long, long> &a, const pair<long, long> &b)/*{{{*/
{
    return a.second > b.second;
}/*}}}*/

void RelationalGraph::setRank()/*{{{*/
{
    vector<pair<long, long> > rank;
    pair<long, long> item;
    int link_count;
    VertexList::iterator it;
    for(it = contentList.begin(); it != contentList.end(); ++it)
    {
        link_count = 0;
        for(int j = 0; j < succ[(*it)].size(); ++j)
        {
            if(vertexTypeMap[succ[(*it)][j]] == 0) // ユーザであることを確かめる
            {
                ++link_count;
            }
        }
        rank.push_back(pair<long, long>((*it), link_count));
    }
    sort(rank.begin(), rank.end(), linkNumSortGreater);
    for(int i = 0; i < rank.size(); ++i)
    {
        item = rank[i];
        rankMap[item.first] = i+1;
    }
}/*}}}*/

int RelationalGraph::getRank(int _id)/*{{{*/
{
    RankMap::iterator it;
    it = rankMap.find(_id);
    if(it != rankMap.end())
    {
        return it->second;
    }
    else
    {
        return -1;
    }
}/*}}}*/

VertexList RelationalGraph::getLinkedContentsIDListOfUser(Vertex _userID)/*{{{*/
{
    VertexList contentsIDList;
    for(int i = 0; i < succ[_userID].size(); ++i)
    {
        if(vertexTypeMap[succ[_userID][i]] == 1)
        {
            contentsIDList.push_back(succ[_userID][i]);
        }
    }
    return contentsIDList;
}/*}}}*/

bool sortIDAndPossibilityPair(const pair<int, double> &a, const pair<int, double> &b)/*{{{*/
{
    return a.second < b.second;
}/*}}}*/

RequestPossibilityList RelationalGraph::getRequestPossibilityList(Vertex _relationalID)/*{{{*/
{
    // 使用する変数
    RequestPossibilityList IDAndPossibilityPairList;
    pair<int, double> item;
    double length;
    double sum_length;
    double accum;
    double before;
    // すでに計算しているかcheck
    ContentsRequestPossibilityMap::iterator it = contentsRequestPossibilityMap.find(_relationalID);
    if(it == contentsRequestPossibilityMap.end()) // メモ化が見つからなかった場合
    {
        sum_length = 0;
        VertexList contentsIDList = getLinkedContentsIDListOfUser(_relationalID);
        // 最短路長の計算
        for(int i = 0; i < contentsIDList.size(); ++i)
        {
            length = dijkstraShortestPathLength(_relationalID, contentsIDList[i]);
            // 関係の強いものほど距離が短いので変換
            length = 1.0 / length;
            sum_length += length;
            IDAndPossibilityPairList.push_back(pair<Vertex, double>(contentsIDList[i], length));
        }
        //}
        // 割合の長さでソート
        sort(IDAndPossibilityPairList.begin(), IDAndPossibilityPairList.end(), sortIDAndPossibilityPair);
        // 割合から確立に変換
        accum = 0;
        for(int i = 0; i < IDAndPossibilityPairList.size(); ++i)
        {
            IDAndPossibilityPairList[i].second /= sum_length;
        }
        // メモに登録
        contentsRequestPossibilityMap[_relationalID] = IDAndPossibilityPairList;
    }
    else
    {
        IDAndPossibilityPairList = it->second;
    }
    return IDAndPossibilityPairList;
}/*}}}*/

VertexList RelationalGraph::getMostRelatedContentsFromUser(Vertex _userID, int cacheNum){/*{{{*/
    // キャッシュを満たすため、関係の強いコンテンツをcacheNumだけとる
    // 結果
    VertexList result;
    bool *f = new bool[nodeNum];
    Weight *dist = new Weight[nodeNum];
    int *prev = new int[nodeNum];
    fill_n(dist, nodeNum, INT_MAX), dist[_userID] = 0, fill_n(prev, nodeNum, -1);
    for(int j= 0; j < nodeNum; j++){
        f[j] = false;
    }
    typedef pair<Weight, int> Distance;
    priority_queue<Distance, vector<Distance>, greater<Distance> > q;
    q.push(Distance(0, _userID));
    while (!q.empty()) {
        int u;
        Weight d_u;
        boost::tuples::tie(d_u, u) = q.top(), q.pop();
        if (f[u]) continue; 
        f[u] = true;
        // 小さいものを前とする
        if(_userID < u)
        {
            shortestmap[NodePair(_userID, u)] = dist[u];
        }
        else
        {
            shortestmap[NodePair(u, _userID)] = dist[u];
        }
         //キャッシュへの登録終わり
        //if(node_to == u){
            //return shortestmap[NodePair(node_from, node_to)];
            //return dist[u];
        //}
        if(vertexTypeMap[u] == 1){
            result.push_back(u);
            if(result.size() >= cacheNum)
                return result;
        }
        foreach (Vertex v, succ[u])
        {
            if (dist[v] > d_u + weight(u, v))
            {
                prev[v] = u, q.push(Distance(dist[v] = d_u + weight(u, v), v));
            }
        }
    }
    return result;
}/*}}}*/

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
        // 値の反転 (値が小さいほど関係が強いことに)
        link_weight = 1.0 / link_weight;
        //cout << node_from << ", " << node_to << ", " << link_weight << endl;
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

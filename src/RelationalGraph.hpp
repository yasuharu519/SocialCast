#ifndef __RELATIONAL_GRAPH_HPP__
#define __RELATIONAL_GRAPH_HPP__

#include "boost/tuple/tuple.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <boost/foreach.hpp>
#include <climits>
#include <queue>
#define foreach BOOST_FOREACH

using namespace std;

typedef int Vertex;
typedef map<Vertex, string> VertexLabelMap;
typedef map<Vertex, string>::value_type VertexLabelKV;
typedef map<Vertex, int> VertexTypeMap;
typedef pair<Vertex, Vertex> Edge;
typedef vector<Edge> EdgeList;
typedef vector<Vertex> VertexList;
typedef vector<VertexList> AdjacencyList;
typedef double Weight;
typedef pair<Vertex, Vertex> NodePair;
typedef map<NodePair, Weight> ShortestMap;
typedef map<Vertex, int> RankMap;

template <class T> class EdgeProperty {/*{{{*/
    private:/*{{{*/
        int size;/*}}}*/
    public:/*{{{*/
        T **w_array;
        map<Edge, T> w_map;
        EdgeProperty() {/*{{{*/
            size = -1;
        }/*}}}*/
        EdgeProperty(const int &n) {/*{{{*/
            w_array = new T*[n];
            for (int i = 0; i < n; ++i)
                w_array[i] = new T[n];
                size = n;
            }/*}}}*/
        T& get(const Vertex &tail, const Vertex &head) {/*{{{*/
            if (size == -1) return w_map[Edge(tail, head)];
            else if (tail < size && head < size) return w_array[tail][head];
        }/*}}}*/
        T& get(const Edge &e) {/*{{{*/
            return get(e.first, e.second);
        }/*}}}*/
        T& operator[] (const Edge &e) {/*{{{*/
            return get(e);
        }/*}}}*/
        T operator() (const Edge &e) const {/*{{{*/
            return (*this)(e.first, e.second);
        }/*}}}*/
        T operator() (const Vertex &tail, const Vertex &head) const {/*{{{*/
            if (size == -1) return w_map.find(Edge(tail, head))->second;
            else if (tail < size && head < size) return w_array[tail][head];
        }/*}}}*//*}}}*/
};/*}}}*/

typedef EdgeProperty<Weight> WeightProperty;

class RelationalGraph{
    public:/*{{{*/
        RelationalGraph();
        virtual ~RelationalGraph();
        //
        AdjacencyList succ, pred;
        VertexLabelMap labelMap;
        VertexTypeMap vertexTypeMap;
        WeightProperty weight;
        ShortestMap shortestmap;
        RankMap rankMap;
        //
        Vertex add_node(int node_id);
        Edge add_edge(const Vertex &tail, const Vertex &head);
        string getLabel(int node_id);
        Weight getEdgeWeight(const Vertex &tail, const Vertex &head);
        double dijkstraShortestPathLength(const Vertex &node_from, const Vertex &node_to);
        int size();
        VertexList getUserList();
        VertexList getContentList();/*}}}*/
        void setRank();
        int getRank(int _id);
        VertexList getLinkedContentsIDListOfUser(Vertex _userID);
    private:/*{{{*/
        int nodeNum;
        VertexList userList;
        VertexList contentList;
        //
        void loadNodeLabel();
        void loadEdges();
        void loadVertexType();
        void setUserList();
        void setContentList();/*}}}*/
};

#endif

#define RELATIONAL_GRAPH_WEIGHT_FILEPATH "../NodeData/outputWeight.txt"
#define RELATIONAL_GRAPH_LINK_FILEPATH "../NodeData/outputLinkList.txt"
#define RELATIONAL_GRAPH_NODENAME_FILEPATH "../NodeData/outputNodeLabel.txt"
#define RELATIONAL_GRAPH_NODETYPE_FILEPATH "../NodeData/outputNodeType.txt"

#define PHYSICAL_NETWORK_CONNECT_RANGE 30
#define PHYSICAL_NETWORK_MAP_RANGE 200

#define LAMBDA (0.8)
// 100Mbps
#define BANDWIDTH (100000000.0)
// 10MB = 80Mbits
#define CONTENT_SIZE (10000000.0 * 8) 
// MTU 1500bytes
#define PACKET_SIZE (1500.0 * 8)
// 100 micro second
#define PACKET_KANKAKU ((PACKET_SIZE / BANDWIDTH) + 0.0001)
// packet num
#define PACKET_NUM ((int)CONTENT_SIZE / PACKET_SIZE)


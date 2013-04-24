FILE_NODE_LABEL = "../NodeData/outputNodeLabel.txt"
FILE_NODE_TYPE = "../NodeData/outputNodeType.txt"
FILE_LINK_WEIGHT = "../NodeData/outputWeight.txt"
FILE_LINK_LIST = "../NodeData/outputLinkList.txt"

users = dict()
contents = dict()

def loadNodeType():
    with open(FILE_NODE_TYPE) as r:
        for line in r.readlines():
            line = line.rstrip()
            node_id, node_type = [int(x) for x in line.split()]
            if node_type == 0:
                users[node_id] = []
            elif node_type == 1:
                contents[node_id] = []

def loadLinkData():
    with open(FILE_LINK_LIST) as r:
        for line in r.readlines():
            line = line.rstrip()
            tail, head = [int(x) for x in line.split()]
            if tail in users and head in contents:
                users[tail].append(head)
            elif tail in contents and head in users:
                users[head].append(tail)

loadNodeType()
loadLinkData()
for i, u in enumerate(sorted(users.items(),
        key=lambda x: len(x[1]),
        reverse=True)):
    u_id, u_contents = u
    #print u_id, len(u_contents)
    print i+1, len(u_contents)

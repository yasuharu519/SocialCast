FILEPATH = "../NodeData/outputNodeType.txt"

count = 0
with open(FILEPATH) as w:
    for line in w.readlines():
        line = line.rstrip()
        n_id, type_id = [int(x) for x in line.split()]
        if type_id not in [0, 1]:
            count += 1
print count

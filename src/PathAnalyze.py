import sys

table = dict()

def resolve(path):
    for i in xrange(len(path) - 1):
        left, right = path[i], path[i+1]
        if left > right:
            left, right = right, left
        if not table.has_key((left, right)):
            table[(left, right)] = 1
        else:
            table[(left, right)] += 1

value = sys.stdin.readline().rstrip()
value = int(value)
print value
for line in sys.stdin.readlines():
    line = line.rstrip()
    path = [int(x) for x in line.split(",")]
    if len(path) > 1:
        resolve(path)
print value - len(table.keys())
for key, value in sorted(table.items(),
        key=lambda x:x[1],
        reverse=True):
    print value

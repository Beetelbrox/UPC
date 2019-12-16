import random as rnd

max_w = 10
max_h = 10

block_num = 160
blocks = []

num_pieces = 1

while len(blocks) < block_num :
    w = rnd.randint(1,max_w)
    h = rnd.randint(1,max_h)
    blocks.append([w,h])        

print(len(blocks))
for b in blocks:
    print(b[0], b[1], end=" ")
    if (b[0] != b[1]): print(b[1], b[0], end="")
    print()

connections = []
for i in range(len(blocks)):
    n_cons = rnd.randint(0,5)
    for _ in range(n_cons):
        con = rnd.randint(1,len(blocks))
        if (con != i+1): connections.append([i+1, con, 1])
        

print(len(connections))
for con in connections:
    print(con[0], con[1], con[2])
import random as rnd

width = 20
height = 10

block_num = 40
blocks = [[width, height]]

num_pieces = 1

while len(blocks) < block_num :
    block = rnd.randint(0,len(blocks)-1)
    if (rnd.randint(0,1)): # 0 is H cut, 1 is V cut
        if (blocks[block][0] <= 1): continue
        cut = blocks[block][0]//2
        blocks.append([blocks[block][0] - cut, blocks[block][1]])
        blocks[block] = [cut, blocks[block][1]]
    else:
        if (blocks[block][1] <= 1): continue
        cut = blocks[block][1]//2
        blocks.append([blocks[block][0], blocks[block][1]-cut])
        blocks[block] = [blocks[block][0], cut]
        

print(len(blocks))
for b in blocks:
    print(b[0], b[1], end=" ")
    if (b[0] != b[1]): print(b[1], b[0], end="")
    print()
f = open ("test_output.out")
positions = []
sizes = []
for _ in range(6):
    tokens = [int(x) for x in f.readline().rstrip().split()]
    positions.append(tokens[:2])
    sizes.append(tokens[2:])

print(positions)
print(sizes)

import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
import matplotlib.patches as patches

fig1 = plt.figure()
ax1 = fig1.add_subplot(111, aspect='equal')
for i in range(len(positions)):
    ax1.add_patch(patches.Rectangle((positions[i][0], positions[i][1]), positions[i][0], positions[i][1]))
plt.show()
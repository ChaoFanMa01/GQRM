#! /usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from matplotlib.lines import Line2D
import MySQLdb


ids = []
types = []
radii = []
xs = []
ys = []
neis = []

cxn = MySQLdb.connect(user = 'root', db = 'cpp', passwd = '0000000027')
cur = cxn.cursor()

cur.execute('SELECT id FROM graph')
for data in cur.fetchall():
    ids.append(data[0])

cur.execute('SELECT type FROM graph')
for data in cur.fetchall():
    types.append(data[0])

cur.execute('SELECT radius FROM graph')
for data in cur.fetchall():
    radii.append(data[0])

cur.execute('SELECT x FROM graph')
for data in cur.fetchall():
    xs.append(data[0])

cur.execute('SELECT y FROM graph')
for data in cur.fetchall():
    ys.append(data[0])

cur.execute('SELECT neighbors FROM graph')
for data in cur.fetchall():
    neis.append(data[0])

# convert strings to int
for i in range(0, len(neis)):
    nodes = neis[i].split(',')
    tmp = []
    if len(neis[i]) != 0:
        for n in nodes:
            if n.isdigit():
                tmp.append(int(n))
        neis[i] = tmp

#for i in range(0, len(ids)):
#    print str(ids[i]) + ' ' + str(types[i]) + ' ' + str(radii[i]) +\
#          ' ' + str(xs[i]) + str(ys[i]) + ' ' + neis[i]

xcoor = np.array(range(0, 101, 10))
ycoor = np.array(range(0, 101, 10))

fig, ax = plt.subplots(1, 1)
ax.set_xticks(xcoor)
ax.set_yticks(ycoor)
plt.tick_params(labelsize=15)


# draw nodes
for i in range(0, len(ids)):
    node = None
    # draw sensors
    if types[i] == 0:
        node = mpatches.Polygon([[xs[i]-2, ys[i]-2], [xs[i]+2, ys[i]-2], [xs[i], ys[i]+2]], color='g', alpha=1)
    # draw relays
    elif types[i] == 1:
        node = mpatches.Rectangle((xs[i]-1, ys[i]-1), 2, 2, color='r', alpha=1)
    # draw sinks
    elif types[i] == 2:
        node = mpatches.Circle((xs[i], ys[i]), 2, color='blue', alpha=1)
    if node != None:
        node.set_zorder(1)
        ax.add_patch(node)
        if 0 < ids[i] <= 10:
            ax.text(xs[i], ys[i]+1, str(ids[i]), family='monospace', fontsize=15)

# draw edges
for i in range(0, len(ids)):
    for j in range(0, len(neis[i])):
        line = Line2D([xs[i], xs[neis[i][j]]], [ys[i], ys[neis[i][j]]], linewidth=2, color="black", alpha=1)
        line.set_zorder(0)
        ax.add_line(line)

plt.grid(linestyle = '--', color = 'gray', linewidth = 1)
plt.show()

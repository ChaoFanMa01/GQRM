#!/usr/bin/env python3

import os

fs = []
cnt = 0

for root, dirs, files in os.walk("."):
    for f in files:
        fs.append(f)
for f in fs:
    fp = open(f, 'r')
    print(f)
    for line in fp.readlines():
        cnt += 1
    fp.close()
print("total " + str(cnt) + " lines")

import re
import os

f = open('all-stations.txt','r')
fd = open('station.tbl','a')
while True:
    line = f.readline()
    if not line:
        break

    line = re.findall('(.*),(.*),(.*)',line)
    if not line:
        break
    fd.write(line[0][0] + '|' + line[0][1] + '|' + line[0][2] + '\n')

import re
import os

def read_folder(folder):
    lst = os.listdir(folder)
    targets = [item for item in lst if item.endswith('.csv')]
    for item in targets:
        f = open(folder + '/' + item,'r')
        fd = open('ttttrain.tbl','a')
        f.readline()

        last_start = 0
        destination = 'NULL'
        arrive = '0'

        line = f.readline()

        line = line.replace(' ','')
        line = re.findall('(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)',line)
        station = line[0][1]

        start = line[0][3]

        line_start = re.findall('(.*):(.*)',line[0][3])
        start = str(int(line_start[0][0])*60 + int(line_start[0][1]))
        
        last_start = int(start)
        
        while True:
            line = f.readline()
            if not line:
                break
            
            line = line.replace(' ','')
            line = re.findall('(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)',line)
            #sn = int(line[0][0])

            if not line:
                break

            destination = line[0][1]
            
            arrive = line[0][2]

            line_arrive = re.findall('(.*):(.*)',line[0][2])
            arrive = str(int(line_arrive[0][0])*60 + int(line_arrive[0][1]))

            arrive_int = int(arrive)
            while arrive_int < last_start:
                arrive_int += 24*60

            arrive = str(arrive_int)

        fd.write(item[:-4] + '|' + station + '|' + destination + '|' + 
        start + '|' + arrive + '\n')

read_folder('0')
read_folder('c')
read_folder('d')
read_folder('g')
read_folder('k')
read_folder('t')
read_folder('y')
read_folder('z')

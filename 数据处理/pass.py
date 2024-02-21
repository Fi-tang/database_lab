import re
import os


def read_folder(folder):
    lst = os.listdir(folder)
    targets = [item for item in lst if item.endswith('.csv')]
    for item in targets:
        f = open(folder+'/'+item,'r')
        fd = open('train.tbl','a')
        f.readline()
        last_start = 0
        count = 0
        while True:
            line = f.readline()
            if not line:
                break
            
            line = line.replace(' ','')
            line = re.findall('(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*),(.*)',line)
            #sn = int(line[0][0])
            if not line:
                break
            station = line[0][1]

            start = line[0][2]

            if start =='-':
                start = 'NULL'
            else:
                line_start = re.findall('(.*):(.*)',line[0][2])
                start = str(int(line_start[0][0])*60 + int(line_start[0][1]))

                start_int = int(start)
                while start_int < last_start:
                    start_int += 24*60
                
                start = str(start_int)

            
            arrive = line[0][3]

            if arrive =='-':
                arrive = 'NULL'
            else:
                line_arrive = re.findall('(.*):(.*)',line[0][3])
                arrive = str(int(line_arrive[0][0])*60 + int(line_arrive[0][1]))

                arrive_int = int(arrive)
                while arrive_int < last_start:
                    arrive_int += 24*60

                arrive = str(arrive_int)


            seat1 = line[0][7]

            if seat1 == '-':
                seat1_hard = 'NULL'
                seat1_soft = 'NULL'
            else:
                line_seat1 = re.findall('(.*)/(.*)',line[0][7])
                if line_seat1[0][0]=='-':
                    seat1_hard = 'NULL'
                else:
                    seat1_hard = line_seat1[0][0]
                
                if line_seat1[0][1]=='-':
                    seat1_soft = 'NULL'
                else:
                    seat1_soft = line_seat1[0][1]
    

        
            seat2 = line[0][8]

            
            if seat2 == '-':
                seat2_up = 'NULL'
                seat2_mid = 'NULL'
                seat2_down = 'NULL'
            else:
                line_seat2 = re.findall('(.*)/(.*)/(.*)',line[0][8])
                if line_seat2[0][0]=='-':
                    seat2_up = 'NULL'
                else:
                    seat2_up = line_seat2[0][0]

                if line_seat2[0][1]=='-':
                    seat2_mid = 'NULL'
                else:
                    seat2_mid = line_seat2[0][1]

                if line_seat2[0][2]=='-':
                    seat2_down = 'NULL'
                else:
                    seat2_down = line_seat2[0][2]
            
            seat3 = line[0][9]

            if seat3 == '-':
                seat3_up = 'NULL'
                seat3_down = 'NULL'
            else:
                line_seat3 = re.findall('(.*)/(.*)',line[0][9])
                if line_seat3[0][0]=='-':
                    seat3_up = 'NULL'
                else:
                    seat3_up = line_seat3[0][0]
                
                if line_seat3[0][1]=='-':
                    seat3_down = 'NULL'
                else:
                    seat3_down = line_seat3[0][1]
            if count == 0:
                fd.write( item[:-4]+'|' +  station + '|' + start +'|' + arrive + '|'+ '0' + '|' + '0' + '|' + '0' + '|' + '0' + '|' + '0' + '|' + '0' + '|'
                + '0' + '\n')
            
            else:
                fd.write( item[:-4]+'|' +  station + '|' + start +'|' + arrive + '|'+ seat1_hard + '|' + seat1_soft + '|' + seat2_up + '|' + seat2_mid + '|' + seat2_down + '|' + seat3_up + '|'
                + seat3_down + '\n')
            count += 1
            
            if start == 'NULL':
                last_start = 0
            else:
                last_start = int(start)


read_folder('0')
read_folder('c')
read_folder('d')
read_folder('g')
read_folder('k')
read_folder('t')
read_folder('y')
read_folder('z')

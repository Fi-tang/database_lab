import re
import os


def read_folder(folder):
    lst = os.listdir(folder)
    targets = [item for item in lst if item.endswith('.csv')]
    for item in targets:
        f = open(folder+'/'+item,'r')
        fd = open('seat.tbl','a')
        f.readline()
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
                
            if seat1_hard == 'NULL':
                seat1_hard = '0'
            else:
                seat1_hard = '5'

            if seat1_soft == 'NULL':
                seat1_soft ='0'
            else:
                seat1_soft = '5'

        
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
            
            if seat2_up == 'NULL':
                seat2_up = '0'
            else:
                seat2_up = '5'

            if seat2_mid == 'NULL':
                seat2_mid = '0'
            else:
                seat2_mid = '5'

            if seat2_down == 'NULL':
                seat2_down = '0'
            else:
                seat2_down = '5'
                

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

            if seat3_up == 'NULL':
                seat3_up = '0'
            else:
                seat3_up = '5'
            
            if seat3_down == 'NULL':
                seat3_down = '0'
            else:
                seat3_down = '5'

            for i in range(0,31):
                fd.write( item[:-4]+'|' + str(i) +  '|'+ station + '|' + '5' + '|' + '5' + '|' + '5' + '|' + '5' + '|' + '5' + '|' + '5' + '|'
                + '5' + '\n')


read_folder('0')
read_folder('c')
read_folder('d')
read_folder('g')
read_folder('k')
read_folder('t')
read_folder('y')
read_folder('z')

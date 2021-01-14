import serial
import re
from pymongo import MongoClient
import datetime
import pprint
import time

ser=serial.Serial('/dev/ttyACM0',9600)
client = MongoClient()
db = client.dddata
collection = db.ddistance

count=0
while 1:
    if(ser.in_waiting >0):
        line=ser.readline()
        #print(line)  
        regular_v6 = re.findall(r"\d\d.\d\d",str(line) )
        #print(regular_v6)
        if len(regular_v6)==1:
            d=float(regular_v6[0])
            print (d)
                        
            count=count+1
            print(count)
            post123=[{"author": "Final",
                        "text": "Distance Test!",
                        "distance":d}]
            time.sleep(1.5)            
                        
                        
            print('post done')           
            post_id=collection.insert_many(post123)

            
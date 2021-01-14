import pymongo
from pymongo import MongoClient
import datetime
import pprint
import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
import serial
import json
import re
import time
clients =pymongo.MongoClient("mongodb://localhost:27017/")
db = clients.dddata
collection = db.ddistance

mydoc=collection.find().sort("_id",-1)#1.2.3.4.5 to 5.4.3.2.1

count=0
while 1:
        print(mydoc[0])
        line = collection.find_one({"author": "Final"})
        regular = re.findall(r"'distance': \d+.\d+",str(mydoc[0]))
        
        regularV2=re.findall(r"\d+.\d+",str(regular))
        print(regular)
        d=regularV2[0]
        publish.single('final/Jian',d,hostname='xxx.xxxx.xx.xxx',auth={'username':'DLRobot','password':'xxxxxx'})
        count=count+1
        time.sleep(1.5)

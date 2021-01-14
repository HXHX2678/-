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
        #db.collection_names(include_system_collections=False)
        print(mydoc[0])
        #pprint.pprint(collection.find_one({"author": "Jian"}))
        line = collection.find_one({"author": "Final"})
        #regular = re.findall("'distance':[0-9]+",str(mydoc[0]))
        #regular=(line["distance"])
        regular = re.findall(r"'distance': \d+.\d+",str(mydoc[0]))
        #print('hi')
        regularV2=re.findall(r"\d+.\d+",str(regular))
        print(regular)
        #regularV2=re.findall("[0-9]+",str(regular))
        #print(regularV2[0])
        d=regularV2[0]
        publish.single('final/Jian',d,hostname='163.13.136.72',auth={'username':'DLRobot','password':'ff21ff21'})
        count=count+1
        time.sleep(1.5)
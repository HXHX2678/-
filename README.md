# 超音波自走車透過藍芽回傳距離資訊至mongo server


# 設備

* Raspberry pi4
* ARDUINO UNO+擴充板
* 超音波自走車
  * 兩輪馬達套件
  * L298N模組
  * 超音波模組
  * 伺服馬達套件for超音波轉向
  * 杜邦線
  * 18660 電池+三串電池盒
* 藍芽模組

# 實體圖
![image](https://github.com/HXHX2678/ultrasonic_car-bluetooth-mongo-server/blob/main/img/261559.jpg)
![image](https://github.com/HXHX2678/ultrasonic_car-bluetooth-mongo-server/blob/main/img/261560.jpg)

# 環境建置
* 本地端(raspberry pi4)
  * Ubuntu 18.04
    * arduino ide
    * python3
* server
  * jupyter

# 進度
- [ ] 自走車藍芽與raspberry連接
- [x] 自走車測距與自走
- [x] 回傳資料至本地端
- [x] 上傳至server端

# 程式介紹

** !!注意!! 
`final2.py` 與 `final3.ipynb` 

IP與密碼已上碼
使用者須要自行更改

** 因為藍芽與PI的傳輸尚未實現,目前是用傳輸線連接PI與UNO,跳過藍芽傳輸的步驟。
* ultrasonic_car.ino

每台自走車因馬達組裝方式都有略微差異,再組裝時需要對車行進方向的副程式進行調整

ex.下面是前進的副程式

    void advance()     
    {
      analogWrite(speedpin, 100);
      analogWrite(speedpin1, 95);
      digitalWrite(pinI4, LOW); 左輪
      digitalWrite(pinI3, HIGH);左輪
      digitalWrite(pinI1, LOW); 右輪
      digitalWrite(pinI2, HIGH);右輪
    }

其中`100、95`是馬達轉速的控制,因每個馬達都要透過L298N的ENA接口控制PWM轉速,需要控制其走直線

pin的`HIGH、LOW`需要自行測試方向並設定or接線

* final1.py

接收uno板回傳的距離資訊

    ser=serial.Serial('/dev/ttyACM0',9600)
    
埠口連接 `('/dev/ttyACM0',9600)` 需修改

資料形態處理

    while 1:
        if(ser.in_waiting >0):
            line=ser.readline()
            regular_v6 = re.findall(r"\d\d.\d\d",str(line) )

            if len(regular_v6)==1:
                d=float(regular_v6[0])
                print (d)

                count=count+1
                print(count)
                post123=[{"author": "Final",
                            "text": "Distance Test!",
                            "distance":d}]
* final2.py
存至本地端

    clients =pymongo.MongoClient("mongodb://localhost:27017/")

本地端地址 `("mongodb://localhost:27017/")`

    while 1:
        print(mydoc[0])
        line = collection.find_one({"author": "Final"})
        regular = re.findall(r"'distance': \d+.\d+",str(mydoc[0]))
        regularV2=re.findall(r"\d+.\d+",str(regular))
        print(regular)
        d=regularV2[0]
        publish.single('final/Jian',d,hostname='xxx.xxx.xxx.xxxx',auth={'username':'DLRobot','password':'xxxxxxxxx'})
        count=count+1
        time.sleep(1.5)

資料形態處理與正規`\d+.\d+`

ip位址 `hostname='163.xxx.xxx.xxxx'`

密碼 `'password':'xxxxxxxxx'`

* final3.ipynb

檔案要丟上jupyter

    client = MongoClient("xxx.xxx.xxx.xxxx",27017,username="st_admin",password="xxxxxxx")
    db = client.db_ddd
    collection = db.aaa
    def on_connect(client, userdata, flags, rc):
        print('connected'+str(rc))
        client.subscribe('final/Jian')
        
同理,ip與密碼要設定好        

資料要再做一次形態處理`regular_v6 = re.findall(r"\d\d+.\d+",str(a))`

* jupyter

運行結果

`connected0
final/Jian
44.28`
    
* 最後去sever>client>post查看資料有無新增

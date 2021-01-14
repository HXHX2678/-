#include <Servo.h>

//定義輪子前後輸出///////////
int pinI1 = 9;   // 定義9腳位 左後
int pinI2 = 8;   // 定義8腳位 左前

int pinI3 = 7;  // 定義7腳位 右後
int pinI4 = 6;  // 定義6腳位 右前
//////////////////////////////

//控制速度.接L298N///////////////////
int speedpin = 3; //速度控制腳位
int speedpin1 = 11; //速度控制腳位
/////////////////////////////////////

//超音波腳位////////////////////////
int inputPinF = A0;  // 定義超音波信號接收腳位元元
int outputPinF = A1; // 定義超音波信號發射腳位
////////////////////////////////////

//接藍芽手機操控用/////////////////
int Fspeedd = 0;      // 前速
int Rspeedd = 0;      // 右速
int Lspeedd = 0;      // 左速
int directionn = 0;   // 前=8 後=2 左=4 右=6
//////////////////////////////////

//定義伺服馬達/////////////////////
Servo myservo;        // 設 myservo
int delay_time = 250; // 伺服馬達轉向後的穩定時間
///////////////////////////////////

//接藍芽手機操控用/////////////////
int Fgo = 8;         // 前進
int Rgo = 6;         // 右轉
int Lgo = 4;         // 左轉
int Bgo = 2;         // 倒車
///////////////////////////////////

//定義所有腳位/////////////////////
void setup()
{
  //胞率//////
  Serial.begin(9600);   
  
  //馬達腳位////
  pinMode(pinI1, OUTPUT); //
  pinMode(pinI2, OUTPUT); // 
  pinMode(pinI3, OUTPUT); // 
  pinMode(pinI4, OUTPUT); // 
  
  //L298N 轉速控制腳位///////
  pinMode(speedpin, OUTPUT); // EA A4
  pinMode(speedpin1, OUTPUT); // EB A5

  pinMode(inputPinF, INPUT);    // 定義超音波輸入腳位元
  pinMode(outputPinF, OUTPUT);  // 定義超音波輸出腳位元
  
  //左右輪轉速///////////
  analogWrite(speedpin, 100); // 左輪
  analogWrite(speedpin1, 90); // 右輪
  
  //定義伺服馬達輸出第5腳位(PWM訊號)
  myservo.attach(5);   
}
//前進方向程式,需要自行測試各方向是否正確,因馬達方向可能各有不同
void advance()     // 前進
{
  analogWrite(speedpin, 100);
  analogWrite(speedpin1, 95);
  digitalWrite(pinI4, LOW); 
  digitalWrite(pinI3, HIGH);
  digitalWrite(pinI1, LOW); 
  digitalWrite(pinI2, HIGH);
}

void back()          //後退
{
  analogWrite(speedpin, 100); 
  analogWrite(speedpin1, 90);
  digitalWrite(pinI4, HIGH); 
  digitalWrite(pinI3, LOW);
  digitalWrite(pinI1, HIGH); 
  digitalWrite(pinI2, LOW);
  delay(250);
}
void right()  
{
  analogWrite(speedpin, 100); 
  analogWrite(speedpin1, 90);
  
  //左輪前進,右輪後退
  digitalWrite(pinI4, HIGH);
  digitalWrite(pinI3, LOW);
  digitalWrite(pinI1, LOW); 
  digitalWrite(pinI2, HIGH);
  delay(500);
}
void left()  
{
  analogWrite(speedpin, 100);
  analogWrite(speedpin1, 90);
  
  //右輪前進,左輪後退
  digitalWrite(pinI4, LOW); 
  digitalWrite(pinI3, HIGH);
  digitalWrite(pinI1, HIGH); 
  digitalWrite(pinI2, LOW);
  delay(500);
}
void stopp()  
{
  analogWrite(speedpin, 100);
  analogWrite(speedpin1, 90);
  
  //注意!!全部HIGH是使馬達有一個扭力鎖死,若是LOW扭力=0會使車子慣性往前滑行
  digitalWrite(pinI4, HIGH); 
  digitalWrite(pinI3, HIGH);
  digitalWrite(pinI1, HIGH); 
  digitalWrite(pinI2, HIGH);
  delay(250);
}

//超音波三個角度測距副程式//////////
void ask_pin_F()   // 量出前方距離
{
  myservo.write(90);              //原始角度
  delay(100);
  digitalWrite(outputPinF, LOW);   // 讓超聲波發射低電壓2μs
  delayMicroseconds(1);
  digitalWrite(outputPinF, HIGH);  // 讓超聲波發射高電壓10μs，這裡至少是10μs
  delayMicroseconds(10);
  digitalWrite(outputPinF, LOW);    // 維持超聲波發射低電壓
  float Fdistance = pulseIn(inputPinF, HIGH);  // 讀差相差時間
  Fdistance = Fdistance / 5.8 / 10;  // 將時間轉為距離距離（單位：公分）
  Serial.print("F distance:");      //輸出距離（單位：公分）
  Serial.println(Fdistance);         //顯示距離
  Fspeedd = Fdistance;              // 將距離 讀入Fspeedd(前速)
}

void ask_pin_R()   // 量出右方距離
{
  myservo.write(175);
  delay(250);
  digitalWrite(outputPinF, LOW);   
  delayMicroseconds(1);
  digitalWrite(outputPinF, HIGH);  
  delayMicroseconds(10);
  digitalWrite(outputPinF, LOW);    
  float Rdistance = pulseIn(inputPinF, HIGH);
  Rdistance = Rdistance / 5.8 / 10;  
  Serial.print("F distance:");      
//  Serial.println(Fdistance);       
  Rspeedd = Rdistance;              
 // Serial.println(Rspeedd);
}
void ask_pin_L()   // 量出左方距離
{
  myservo.write(5);
  delay(250);
  digitalWrite(outputPinF, LOW);   
  delayMicroseconds(1);
  digitalWrite(outputPinF, HIGH);  
  delayMicroseconds(10);
  digitalWrite(outputPinF, LOW);    
  float Ldistance = pulseIn(inputPinF, HIGH);  
  Ldistance = Ldistance / 5.8 / 10;  
  Serial.print("F distance:");     
//  Serial.println(Fdistance);         
  Lspeedd = Ldistance;              
// Serial.println(Rspeedd);
}

//超音波測距角度主程式
void detection()        //測量角度(90)
{
  ask_pin_F();            // 讀取前方距離
  if (Fspeedd < 20)        // 假如前方距離小於10公分
  { 
    stopp();
   
    ask_pin_R();
    
    ask_pin_L();
    
    //directionn = 2;
  
    if (Lspeedd> Rspeedd)
    {
      directionn=Lgo;
     }
    if (Lspeedd< Rspeedd)
    {
      directionn=Rgo;
     }
    if (Lspeedd < 20 && Rspeedd < 20)   //假如 左邊距離和右邊距離皆小於10公分
     {
      directionn = Bgo;      //向後走        
     }
  }    
  else {
    directionn = Fgo;
  }
}
///////////////////////////////////////////////

//車子主程式///////////////////////////////////
void loop()
 {
    myservo.write(90);  //讓伺服馬達回歸 預備位置 準備下一次的測量
    detection();        //測量角度 並且判斷要往哪一方向移動
      
   if(directionn == 2)  //假如directionn(方向) = 2(倒車)             
   {
     back();                    //  倒退(車)
     left();                   //些微向左方移動(防止卡在死巷裡)
     Serial.print(" Reverse ");   //顯示方向(倒退)
   }
   if(directionn == 6)           //假如directionn(方向) = 6(右轉)    
   {
     //back(); 
     right();                   // 右轉
     Serial.print(" Right ");    //顯示方向(左轉)
   }
   if(directionn == 4)          //假如directionn(方向) = 4(左轉)    
   {  
     //back();      
     left();                  // 左轉
     Serial.print(" Left ");     //顯示方向(右轉)   
   }  
   if(directionn == 8)          //假如directionn(方向) = 8(前進)      
   { 
    advance();                 // 正常前進  
    Serial.print(" Advance ");   //顯示方向(前進)
    //Serial.print("   ");    
   }
 }

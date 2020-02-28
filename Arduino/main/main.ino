#include <TinyGPS++.h>
#include <SoftwareSerial.h>
 
 
TinyGPSPlus gps; 
SoftwareSerial gpsSerial(2, 3);
SoftwareSerial sim(6, 7); //SIM800L Tx & Rx is connected to Arduino #3 & #2
char incomingByte; 
String inputString;

int light1 = 12;
int buzzer = 5;

String sendSms ="Unstable the gps,.";
boolean isSim = true;
boolean isGPS = true;
int smsCount = 0; 
int reccount = 0;
boolean turnBth = false;
void setup()
{ 
  Serial.begin(9600); 
  sim.begin(9600);  
  gpsSerial.begin(9600); 
  sim.listen();
   
//  gpsSerial.listen();
  pinMode(light1, OUTPUT); 
  pinMode(buzzer, OUTPUT);
  digitalWrite(light1,HIGH);
  digitalWrite(buzzer,LOW); 
   
  while(!sim.available()){
    sim.println("AT");
    delay(1000); 
    Serial.println("Connecting...");
    }
  Serial.println("Connected!");  
  sim.println("AT+CMGF=1");  //Set SMS to Text Mode 
  delay(1000);  
  sim.println("AT+CNMI=1,2,0,0,0");  //Procedure to handle newly arrived messages(command name in text: new message indications to TE) 
   delay(1000);
  sim.println("AT+CMGDA=\"DEL ALL\"");
  delay(1000); 
  
}

void loop()
{  
  if(smsCount > 1){
    Serial.println("sim");   
    received();
  }else if(turnBth){ 
    if(Serial.available()){
      Serial.println("mweoh");
      Serial.println(Serial.readString());
      digitalWrite(buzzer,LOW);
    }else{
      Serial.println("arffff");
//      digitalWrite(buzmzer,HIGH);
    }
    turnBth = false;
  } else{ 
    gpsSerial.listen();
    if (gpsSerial.available() > 0){  
      if (gps.encode(gpsSerial.read())){ 
        Serial.println("displayInfo");  
        displayInfo(); 
      }
    } 
  }   
}

void received()
{  
 sim.listen();
 if(sim.available()){
    delay(100); 
    isSim = false; 
    Serial.println("sad");  
    while(sim.available()){
      incomingByte = sim.read();
      inputString += incomingByte; 
      Serial.print(".");   
    } 
    delay(10);       
    inputString.toLowerCase(); // Uppercase the Received Message 
//   +cmt: "+639555730503","","20/02/27,16:08:41+32" 
    String number = getValue(inputString,'"',1);
    Serial.println(number);  
    if(number != ""){ 
      reccount = 0;
      SendMessage(sendSms,number);  
    }
    delay(50);  
    inputString = ""; 
    isSim = true; 
  } 
  if(reccount > 6){
    smsCount = 0;
    reccount = 0;
    turnBth = true;
  }reccount =reccount +1;
  delay(100);
}

void displayInfo()
{ 
  isGPS = false;
  if (gps.location.isValid())
  {
    String Lat = String(gps.location.lat(),6);
    String Lng = String(gps.location.lng(),6); 
    Serial.println("("+Lat+","+Lng+")");  
    sendSms = Lat+","+Lng;
  }else{
    sendSms = "Unstable the gps,.";
    Serial.println("[nogps]");  
//    digitalWrite(light1,LOW);
  }
  delay(1000);
  isGPS = true;
  smsCount = smsCount+1; 
}
void SendMessage(String msgreq,String number)
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000); 
  sim.println(msgreq);
  delay(100);
  sim.println((char)26);
  delay(1000);
  sim.println("AT+CMGDA=\"DEL ALL\"");
  delay(1000);
  smsCount =0;
  reccount =0;
  turnBth = true;
}
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

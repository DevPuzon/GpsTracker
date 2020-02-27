#include <TinyGPS++.h>
#include <SoftwareSerial.h>
 
 
TinyGPSPlus gps; 
SoftwareSerial gpsSerial(2, 3);
SoftwareSerial sim(6, 7); //SIM800L Tx & Rx is connected to Arduino #3 & #2
char incomingByte; 
String inputString;

int light1 = 12;

String sendSms ="";
boolean isSim = true;
boolean isGPS = true;
void setup()
{ 
  Serial.begin(9600); 
  sim.begin(9600);  
  gpsSerial.begin(9600); 
  sim.listen();
   
   
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
  
//  gpsSerial.listen();
  pinMode(light1, OUTPUT); 
}

int smsCount = 0; 
void loop()
{    
  if(smsCount > 2){
     Serial.println("qweqweq");  
    received();
  }else{ 
    gpsSerial.listen();
  }  
  digitalWrite(light1,HIGH);
  if (gpsSerial.available() > 0){ 
    if (gps.encode(gpsSerial.read())){ 
      Serial.println("displayInfo");  
      displayInfo(); 
    }
  }  
}

int reccount = 0;
void received()
{  
 Serial.println("sim");  
 sim.listen();
 if(sim.available()){
    delay(100); 
    isSim = false;
    Serial.print("sad.");  
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
    SendMessage(sendSms,number);
    delay(50);  
    inputString = ""; 
    isSim = true;
    smsCount = 0;
  } 
  if(reccount > 6){
    smsCount = 0;
    reccount = 0;
  }reccount =reccount +1;
  delay(1000);
}

void displayInfo()
{ 
  isGPS = false;
  if (gps.location.isValid())
  {
    String Lat = String(gps.location.lat(),6);
    String Lng = String(gps.location.lng(),6); 
    Serial.println("("+Lat+","+Lng+")");  
    sendSms = "("+Lat+","+Lng+")";
  }else{
    sendSms = "Unstable the gps";
    Serial.println("[nogps]");  
    digitalWrite(light1,LOW);
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

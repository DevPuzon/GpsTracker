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
unsigned long previous=0;
unsigned long previous1=0;
// How frequently we want to send the location (milliseconds)
static const unsigned long frequency = 15000;
// How frequently we want to send the location (milliseconds)
static const unsigned long frequency1 = 15000;
unsigned long beltTime;
String responseString;
// Maximum time to wait SIM module for response
static long maxResponseTime = 30000;
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
  while(sim.available()){
    
    if (sim.available() > 0)
      Serial.write(sim.read());
  }
  gpsSerial.listen();
}

void loop()
{  
//  if(smsCount > 1){
//    Serial.println("sim");   
//    received();
//  }else if(turnBth){ 
//    if(Serial.available()){
//      Serial.println("mweoh");
//      Serial.println(Serial.readString());
//      digitalWrite(buzzer,LOW);
//    }else{
//      Serial.println("arffff");
////      digitalWrite(buzmzer,HIGH);
//    }
//    turnBth = false;
//  } else{ 
//  }   

  
    if (gpsSerial.available() > 0){  
      if (gps.encode(gpsSerial.read())){ 
        Serial.println("displayInfo");  
        displayInfo(); 
      }
    } 
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

  
  if(millis() - previous > frequency)
  { 
//    +cmgs: 163

    previous = millis();
    SendMessage("1111","+639555730503");
  }  
}
void SendMessage(String msgreq,String number)
{
  sim.listen();
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000); 
  sim.println(msgreq);
  delay(100);
  sim.println((char)26);
  sim.println(); 
  smsCount =0;
  reccount =0;
  turnBth = true; 
  while (sim.available())
    Serial.write(sim.read());
  
//    delay(20000);
// 
//    Serial.println("gpsSerial.listen();");  
//    gpsSerial.listen(); 
}

void waitUntilResponse(String response)
{
  beltTime = millis();
  responseString="";
  String totalResponse = "";
  while(responseString.indexOf(response) < 0 && millis() - beltTime < maxResponseTime)
  {
    readResponse();
    totalResponse = totalResponse + responseString;
    Serial.println(responseString);
  }

  if(totalResponse.length() <= 0)
  {
    Serial.println("No response from the module. Check wiring, SIM-card and power!"); 
    delay(30000);
//    exit(0); // No way to recoverk
  }
  else if (responseString.indexOf(response) < 0)
  {
    Serial.println("Unexpected response from the module");
    Serial.println(totalResponse); 
    delay(30000);
//    exit(0); // No way to recover
  }
}
void readResponse()
{
  responseString = "";
  while(responseString.length() <= 0 || !responseString.endsWith("\n"))
  {
    tryToRead();

    if(millis() - beltTime > maxResponseTime)
    {
      return;
    }
  }
}
void tryToRead()
{
  while(sim.available())
  {
    char c = sim.read();  //gets one byte from serial buffer
    responseString += c; //makes the string readString
  }
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

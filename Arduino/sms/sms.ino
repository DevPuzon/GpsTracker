 #include <ArduinoJson.h>
#include <SoftwareSerial.h>

String number = "+639555730503";  
//Create software serial object to communicate with SIM800L
SoftwareSerial sim(6, 7); //SIM800L Tx & Rx is connected to Arduino #3 & #2
char incomingByte; 
String inputString;

void setup()
{ 
  pinMode(2, OUTPUT); 
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600); 
  //Begin serial communication with Arduino and SIM800L
  sim.begin(9600);  
   
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
  sim.println("AT+CMGL=\"REC UNREAD\""); // Read Unread Messages 
   delay(1000);
  sim.println("AT+CMGDA=\"DEL ALL\"");
  digitalWrite(2, HIGH); // sets the digital pin 13 on 
}

void loop() {
  if (Serial.available() > 0)
    switch (Serial.read())
    {
      case 's':
        SendMessage("asdas");
        break; 
      case 'c':
        callNumber();
        break;
    }
  if (sim.available() > 0)
    Serial.write(sim.read());
  
  received();   
}

void received()
{ 
 if(sim.available()){
      delay(100);

      // Serial Buffer
      while(sim.available()){
        incomingByte = sim.read();
        inputString += incomingByte; 
        } 
        delay(10);       
        inputString.toLowerCase(); // Uppercase the Received Message 
        Serial.println("hello "+inputString);  
        delay(50); 
        //Delete Messages & Save Memory
//        if (inputString.indexOf("OK") == -1){
//        sim.println("AT+CMGDA=\"DEL ALL\"");

//        delay(1000);}

        inputString = "";
  }
}
void SendMessage(String msgreq)
{
  sim.println("AT+CMGF=1");
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r");
  delay(1000); 
  sim.println(msgreq);
  delay(100);
  sim.println((char)26);
  delay(1000);
}

void callNumber() {
  sim.print (F("ATD"));
  sim.print (number);
  sim.print (F(";\r\n"));
}

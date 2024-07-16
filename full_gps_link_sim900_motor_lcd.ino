// /////////gps
#include <TinyGPS++.h>
//#include <SoftwareSerial.h>
#include <AltSoftSerial.h>
//#include <EEPROM.h>
//#include <math.h>
static const int RXPin = 8, TXPin = 9;// Here we make pin 4 as RX of arduino & pin 3 as TX of arduino 
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
//SoftwareSerial ss(RXPin, TXPin);
AltSoftSerial ss(RXPin, TXPin);
//
String longitude;
String latitude; 

 /////////////
 #include <SoftwareSerial.h>  
//#include <AltSoftSerial.h>
// AltSoftSerial SIM900(2,3);
 SoftwareSerial SIM900(2,3); // SoftSerial( RX , TX );   //Create software serial object to communicate with SIM800L
#define Redled 4 // Load1 Pin Out
#define greenled 5 // Load2 Pin Out
#define buzzer 7 // Load3 Pin Out
//#include<LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27,16,2);

// #define ENA 
 #define IN3 12
 #define IN4 11
int Switch=4;
bool buulean=true;
String message;
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
void setup(){ // put your setup code here, to run once
   
  SIM900.begin(9600);
  Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)  
  ss.begin(GPSBaud); 
 
  Serial.println ("SIM900 Ready");  
  delay(100);  
 SIM900.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
pinMode(Redled, OUTPUT); 
pinMode(Switch, INPUT);
digitalWrite(Redled, 0); 
pinMode(greenled, OUTPUT); 
digitalWrite(greenled, 0); 
pinMode(buzzer, OUTPUT); 
 pinMode(IN3, OUTPUT); 
 pinMode(IN4, OUTPUT);

 // lcd.begin();
 lcd.clear();
 lcd.init();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print(" Vehicle Theft ");

 lcd.setCursor(3,1);
 lcd.print("welcome");
}

void loop(){
//    //////////////////// start gps
   while (ss.available() > 0)
    if (gps.encode(ss.read()))
//      displayInfo();


  if (gps.location.isValid())
  {
     latitude = String(gps.location.lat(), 6);
     longitude = String(gps.location.lng(), 6);
//       Serial.println("latitude"+latitude);
//       Serial.println("logitude: "+longitude);
  }
//    else
//  {
//    Serial.print(F("INVALID"));
//  }
////getGPS();
//  ///////////end gps
//
//    
  int stateSw=digitalRead(Switch);
  digitalWrite(greenled, 1);
    if (SIM900.available()>0){
  message = SIM900.readString();
  }
  

//------------ Is there a new SMS?  -------------


//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
  if(stateSw==1 && buulean==true){
Serial.println("your motor is on");
noTone(buzzer);
digitalWrite(greenled,HIGH);
digitalWrite(Redled,LOW);
digitalWrite(IN3, HIGH); 
digitalWrite(IN4, LOW);

  
 lcd.setCursor(0,0);
 lcd.print(" Vehicle Theft ");
 lcd.setCursor(1,1);
 lcd.print("your motor on ");
    delay(10);
  }//statesw==1
//  delayMicroseconds(1);
//----------------------DATA WHEN THE USER WANTS TO TURN OFF THE SYSTEM -------------------------
if(stateSw==1){
  digitalWrite(greenled,HIGH);
//digitalWrite(Redled,HIGH);
      if(message.indexOf("State") > -1){
        SendMessage();
         lcd.setCursor(0,0);
         lcd.print(" Vehicle Theft ");
         lcd.setCursor(1,1);
         lcd.print("your motor on");
         delay(10);
}
if(message.indexOf("Off") > -1){
      SendMessage1();
        digitalWrite(IN3, HIGH); 
        digitalWrite(IN4, HIGH);
        digitalWrite(Redled,HIGH);
        digitalWrite(greenled,LOW);
        digitalWrite(Redled,HIGH);
       
        //use when needed more data to send
        digitalWrite(Redled,HIGH);
        digitalWrite(greenled,LOW);
         lcd.setCursor(0,0);
         lcd.print(" Vehicle Theft ");
         lcd.setCursor(1,1);
         lcd.print("your motor off");
          delay(10);
for(int i=0;i<100; i++){
      tone(buzzer,2222);
     delay(50);
    tone(buzzer,3333);
     delay(50);
    tone(buzzer,5555);
     delay(50);}
     noTone(buzzer);
     digitalWrite(greenled,LOW);
     
      }//END OFF


      else if(message.indexOf("On") > -1){
         SendMessage();
        digitalWrite(IN3, HIGH); 
        digitalWrite(IN4, LOW);
        digitalWrite(Redled,LOW);
        noTone(buzzer);
        digitalWrite(greenled,HIGH);
        //sendSMS(GSM_Nr,"your motor is off");
        digitalWrite(Redled,LOW);
     
        //use when needed more data to send
        digitalWrite(Redled,LOW);
        digitalWrite(greenled,HIGH);
         lcd.setCursor(0,0);
          lcd.print(" Vehicle Theft ");
           lcd.setCursor(1,1);
           lcd.print("your motor on ");
           delay(10);
      }// END OF WHEN WRITE ON

      
buulean=false;
 }
  

// ---------------- DATA FOR WHEN THE SYSTEM IS OFF ---------------

// ---------------- DATA FOR WHEN THE SYSTEM IS ON ---------------
else if(stateSw==0){
Serial.println("your motor is off");
 lcd.setCursor(0,0);
 lcd.print(" Vehicle Theft ");
 lcd.setCursor(1,1);
 lcd.print("your motor off");
digitalWrite(Redled,LOW);
digitalWrite(greenled,HIGH);
noTone(buzzer);
digitalWrite(IN3, HIGH); 
digitalWrite(IN4, HIGH);

if(message.indexOf("State") > -1){
        Serial.println("your motor is off");  
       SendMessage1();
        lcd.setCursor(0,0);
        lcd.print(" Vehicle Theft ");
        lcd.setCursor(1,1);
        lcd.print("your motor off");
        delay(10);
}// END OF STATE OF SWITCH OFF

  buulean=true;
  } //statesw==0 NNN END STATE
//delayMicroseconds(1);
}// END OF VOID LOOP


  void SendMessage()  
 {  
  Serial.println ("Sending Message");  
  SIM900.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode  
  delay(1000);  
  Serial.println ("Set SMS Number");  
  SIM900.println("AT+CMGS=\"0612535355\"\r"); //Type Your Mobile number to send message  
  delay(1000);  
  Serial.println ("Set SMS Content"); 
  String link ="\nGPS Location :\nwww.google.com/maps/place/"+latitude + "," + longitude ;
  delay(100); 
  SIM900.println("Vehicle Report: \n YOur motor on \nVehicle ID: SO19865\nName: Ruwayda Ali "+link);// Messsage content  
  delay(100);  
//  SIM900.println(latitude);// Messsage content  
//  delay(100);
//  SIM900.println(longitude);// Messsage content  
//  delay(100);
  Serial.println ("Finish");  
  SIM900.println((char)26);// ASCII code of CTRL+Z  
  delay(1000);  
  Serial.println ("Message has been sent ->Messageka wuu dirmay ");  
 }  


 void SendMessage1()  
 {  
  Serial.println ("Sending Message");  
  SIM900.println("AT+CMGF=1");  //Sets the GSM Module in Text Mode  
  delay(1000);  
  Serial.println ("Set SMS Number");  
  SIM900.println("AT+CMGS=\"0612535355\"\r"); //Type Your Mobile number to send message  
  delay(1000);  
  Serial.println ("Set SMS Content");  
  String link ="\nGPS Location :\nwww.google.com/maps/place/" +latitude + "," + longitude ;
  delay(100);
  SIM900.println("Vehicle Report:\n YOur motor off \nVehicle ID: SO19865\nName:  Ruwayda Ali"+link);// Messsage content  
  delay(100);  
//  SIM900.println(latitude);// Messsage content  
//  delay(100);
//  SIM900.println(longitude);// Messsage content  
//  delay(100);
  Serial.println ("Finish");  
  SIM900.println((char)26);// ASCII code of CTRL+Z  
  delay(1000);  
  Serial.println ("Message has been sent ->Messageka wuu dirmay");  
 }  

 

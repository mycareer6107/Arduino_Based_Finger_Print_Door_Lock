// Finger Print Based Door Lock System
#include <Adafruit_Fingerprint.h>
//#include <SPI.h> //  Arduino pin 11 Receiver Mhz433 data pin

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
SoftwareSerial mySerial(2, 3);
int Relay = 13;  //output pin for door
const int buzzerPin = 9; //output pin for buzzer
int door_sensor = 8;  //input pin for door sensor
int manual_button = 4; // input pin for manual open button
int manual_button_state = 0;  //manual button state 
boolean door_state = 1 ;   //door open close state
int silent_state = 0 ;   //silent state
int running_status = 12; //output for circuit running state
int silent = 7;  //  input pin for silent
#else
#define mySerial Serial1
#endif

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup() {
Serial.begin(9600);
while (!Serial);  
delay(100);
pinMode(buzzerPin, OUTPUT);
pinMode(running_status, OUTPUT);
pinMode(manual_button, INPUT);
pinMode(door_sensor, INPUT);
pinMode(silent, INPUT);
digitalWrite(door_sensor, LOW);
pinMode(Relay, OUTPUT);
digitalWrite(Relay,LOW);
finger.begin(57600);
delay(5);
if (finger.verifyPassword()) {
  Serial.println("Found fingerprint sensor!");
} 
else {
  Serial.println("Did not find fingerprint sensor :(");
  while (1) { delay(1); }
}
}

void loop() {
uint8_t p = finger.getImage();
if (p == FINGERPRINT_NOFINGER)  goto NoFinger; 
else if (p == FINGERPRINT_OK) {
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  goto NoMatch;
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  goto NoMatch;
    digitalWrite(Relay, HIGH);
    tone(buzzerPin, 2000, 1000);
    delay(500);
    digitalWrite(Relay, LOW);
    //delay(5000);
    return;
 
}
  NoMatch: 
{
/*Serial.println("Access  Denied"); 
delay(100);*/ 
return;
} 
NoFinger: 
{
/*Serial.println("No finger detected");
delay(100);*/
}
silent_state = digitalRead(silent);  //silent input get in silent state variable
door_state = digitalRead(door_sensor);  //door sensor input get in door state variable
manual_button_state = digitalRead(manual_button);  //manual button input get in manual button state variable
  if(manual_button_state!=1)   //manual button condition
  {
    digitalWrite(Relay, HIGH);
    tone(buzzerPin, 2000, 1000);
    delay(500);
    digitalWrite(Relay, LOW);
    //delay(5000); 
  }
  if(silent_state!=1)  // silent button condition
  {
    delay(60000);    //silent mood for 1 minut
  }
 if(door_state!=0)    // door state condition
{
     tone(buzzerPin, 2000, 1000);
}

  digitalWrite(running_status, LOW);    //running status show led
  delay(400);
  digitalWrite(running_status, HIGH);
}
    

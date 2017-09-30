

//#include <SoftwareSerial.h>
#include <Servo.h>

// Pin definitions
#define ENA 12 //- Motor A Enable
#define IN1 24 //- Motor A Direction 1
#define IN2 25 //- Motor A Direction 2
#define IN3 26 //- Motor B Direction 1
#define IN4 27 //- Motor B Direction 2
#define ENB 11 //- Motor B Enable
#define PUMP_PIN 6 //- Turns on water pump
#define LED_PIN 13 //- Built in LED Pin
#define SHOULDER_PIN 10 // Servo for shoulder (left/right)
#define ELBOW_PIN 9 //
#define WRIST_PIN 8

// Where to accept commands from
#define PORT Serial1 

#define incAmt 10
int speed = 200;
int feet;

int incomingByte=0;

Servo elbow;
Servo wrist;
Servo shoulder;

int pos_wrist = 0;
int pos_shoulder = 0;
int pos_elbow=0;

const int pingPin = 7;
const int echo = 6;
long duration, inches, cm;

//SoftwareSerial Serial2(18, 19);


void setup(){
  
  Serial.begin(9600);
  PORT.begin(9600);
  
  shoulder.attach(SHOULDER_PIN);
  wrist.attach(WRIST_PIN);
  elbow.attach(ELBOW_PIN);
  
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(PUMP_PIN,OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  PORT.println("Send commands to Omni-bot.");
  PORT.println("Commands: f, b, l, r, s, p, m, g, u, d, e, w, 1");
  PORT.println("Send h for help and information on commands.");
  
  digitalWrite(LED_PIN, 0);
  delay(2000);
  digitalWrite(LED_PIN, 1);
  delay(1000);
  digitalWrite(LED_PIN, 0);
  delay(500);
  digitalWrite(LED_PIN, 1);
  delay(5000);
  digitalWrite(LED_PIN, 0);
  
}

void loop(){
  
//  getDistance();
//  horizontal();
//  vertical();

  if (PORT.available() > 0){
     incomingByte = PORT.read();
     
     Serial.print ("Received Char:  ");
     Serial.println ((char)incomingByte);
     
     if (incomingByte == 'f') {
       forward(speed);
     }
     else if (incomingByte == 'b') {
       back(speed);
     }
     else if (incomingByte == 'r') {
       right(speed);
     }
     else if (incomingByte == 'l') {
       left(speed);
     }
     else if (incomingByte == 's') {
       stop();
     }
     else if (incomingByte == 'h'){
       PORT.println("More command info coming soon!");
     }
     else if (incomingByte == 'p' && speed+incAmt<=255) {
       speed+=incAmt;
       PORT.println(speed);
     }
     else if (incomingByte == 'm' && speed-incAmt>=0) {
       speed-=incAmt;
       PORT.println(speed);
     }
     else if (incomingByte == 'g'){
       getDistance();
     }
     else if (incomingByte == 'd' && pos_wrist+10 <=180){
       pos_wrist+=10;
       PORT.println(pos_wrist);
       wrist.write(pos_wrist);
      
     }
     else if (incomingByte == 'u' && pos_wrist-10 >=0){
       pos_wrist-=10;
       PORT.println(pos_wrist);
       wrist.write(pos_wrist);
     }
     else if (incomingByte == 'e' && pos_shoulder+10<=180){
       pos_shoulder+=10;
       PORT.println(pos_shoulder);
       shoulder.write(pos_shoulder);
     }
     else if (incomingByte=='C' && pos_elbow+10<=180){
       pos_elbow+=10;
       PORT.println(pos_elbow);
       elbow.write(pos_elbow);
     }
     else if (incomingByte=='c' && pos_elbow-10>=0){
       pos_elbow-=10;
       PORT.println(pos_elbow);
       elbow.write(pos_elbow);
     }
     else if (incomingByte == 'w' && pos_shoulder-10>=0){
       pos_shoulder-=10;
       PORT.println(pos_shoulder);
       shoulder.write(pos_shoulder);
     }
     else if (incomingByte=='z'){
       pumpon();
     }
     else if (incomingByte=='x'){
       pumpoff();
      
     }
     else if (incomingByte=='y'){
       squirt();
     }
     else if (incomingByte=='q'){
       digitalWrite(LED_PIN,1);
     }
     else if (incomingByte=='k'){
       digitalWrite(LED_PIN,0);
     }
     else{
       PORT.print((char) incomingByte);
       PORT.println(" is not a command.");
       Serial.print((char) incomingByte);
       Serial.println(" is not a command.");
     }
     PORT.print("Received: ");
     PORT.println ((char)incomingByte);
     //PORT.print("Raw: ");
     //PORT.println (incomingByte);

     // send it out the IR port too!
     //Serial2.write(incomingByte);
  }
}

// if (Serial2.available() > 0){
//   incomingByte = Serial2.read();
//   Serial.print ("Omni-bot:  ");
//   Serial.println ((char)incomingByte);
//   }  


void forward(int speed){
analogWrite(ENA,speed);
digitalWrite(IN1,1);
digitalWrite(IN2,0);
analogWrite(ENB,speed);
digitalWrite(IN3,1);
digitalWrite(IN4,0);
}

void back(int speed){
analogWrite(ENA,speed);
digitalWrite(IN1,0);
digitalWrite(IN2,1);
analogWrite(ENB,speed);
digitalWrite(IN3,0);
digitalWrite(IN4,1);
}

void right(int speed){
analogWrite(ENA,speed);
digitalWrite(IN1,1);
digitalWrite(IN2,0);
analogWrite(ENB,speed);
digitalWrite(IN3,0);
digitalWrite(IN4,1);
}

void left(int speed){
analogWrite(ENA,speed);
digitalWrite(IN1,0);
digitalWrite(IN2,1);
analogWrite(ENB,speed);
digitalWrite(IN3,1);
digitalWrite(IN4,0);
}

void stop(){
analogWrite(ENA,0);
analogWrite(ENB,0);
}

void getDistance(){
  
   pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  
   pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);
  
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  if(inches>=12){
    feet = inches/12;
    inches = inches - feet*12;
    PORT.print(feet);
    PORT.print("ft,  ");
  }
  
  PORT.print(inches);
  PORT.println("in");
//  Serial.print(cm);
//  Serial.print("cm");
//  Serial.println();
}
long microsecondsToInches(long microseconds){
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds){
  return microseconds / 29 / 2;
}

void shoulder_movement(){
  for(pos_shoulder = 0; pos_shoulder < 180; pos_shoulder += 10){
    shoulder.write(pos_shoulder);
    delay(100);
  }
  for(pos_shoulder = 180; pos_shoulder>=1; pos_shoulder-=10){                                
    shoulder.write(pos_shoulder);
    delay(100);
  } 
}
 void wrist_movement(){
  for(pos_wrist = 0; pos_wrist < 180; pos_wrist += 10){                       
    wrist.write(pos_wrist);
    delay(100);
  }
  for(pos_wrist = 180; pos_wrist>=1; pos_wrist-=10){                                
    wrist.write(pos_wrist);
    delay(100);
  } 
}
void elbow_movement(){
  for(pos_elbow = 0; pos_elbow < 180; pos_elbow += 10){                       
    elbow.write(pos_elbow);
    delay(100);
  }
  for(pos_elbow = 180; pos_elbow>=1; pos_elbow-=10){                                
    elbow.write(pos_elbow);
    delay(100);
  }
} 
void pumpon (){
  digitalWrite(PUMP_PIN,1);
}
void pumpoff () {
  digitalWrite(PUMP_PIN,0);
}
void squirt(){
  digitalWrite(PUMP_PIN,1);
  delay(500);
  digitalWrite(PUMP_PIN,0);
}
void autonomous_mode(){
}
}



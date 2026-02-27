#include <Servo.h>

#define servoPin 11
#define vcc 12
#define gnd 13

const int penZUp = 0;
const int penZDown = 90;
const int penDelay = 50;

bool verbose = true;
Servo penServo;

void setup() {
  pinMode(servoPin, OUTPUT);
  pinMode(vcc, OUTPUT);
  pinMode(gnd, OUTPUT);

  digitalWrite(gnd, LOW);
  digitalWrite(vcc, HIGH);

  Serial.begin(9600);
  Serial.println("***************");

  penServo.attach(servoPin);
  penServo.write(penZUp);
  delay(100);
  

}

void loop() {
  penUp();
  delay(1000);
  penDown();
  delay(1000);

}

//  Raises pen
void penUp() { 
  penServo.write(penZUp); 
  delay(penDelay); 
  //Zpos=Zmax; 
  //digitalWrite(15, LOW);
  //digitalWrite(16, HIGH);
  if (verbose) { 
    Serial.println("Pen up!"); 
    
  } 
}
//  Lowers pen
void penDown() { 
  penServo.write(penZDown); 
  delay(penDelay); 
  //Zpos=Zmin; 
  //digitalWrite(15, HIGH);
  //digitalWrite(16, LOW);
  if (verbose) { 
    Serial.println("Pen down."); 
    
    
  } 
}

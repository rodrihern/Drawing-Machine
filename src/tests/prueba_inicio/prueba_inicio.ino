#include <Servo.h>

#define step_x 2
#define step_y 8
#define dir_x 3
#define dir_y 7
#define servoPin 11

bool verbose = true;

Servo penServo;

const int penZUp = 15;
const int penZDown = 90;

const int stepDelay = 1000;
const int penDelay = 50;

#define finCarrera_y 9
#define finCarrera_x 10

void setup() {
  pinMode(servoPin, OUTPUT);
  pinMode(step_x, OUTPUT);
  pinMode(step_y, OUTPUT);
  pinMode(dir_x, OUTPUT);
  pinMode(dir_y, OUTPUT);
  
  penServo.attach(servoPin);

  pinMode(finCarrera_y, INPUT_PULLUP);
  pinMode(finCarrera_x, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("***************");
  
  inicio();
}

void loop() {

  
}

void penUp() { 
  penServo.write(penZUp); 
  delay(penDelay); 
  //Zpos=Zmax; 

  if (verbose) { 
    Serial.println("Pen up!"); 
    
  } 
}


void inicio() {
  penUp(); 
  digitalWrite(dir_x, LOW);
  digitalWrite(dir_y, HIGH);

  while (digitalRead(finCarrera_y) == HIGH) {
    digitalWrite(step_y, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(step_y, LOW);
    delayMicroseconds(stepDelay);
  }
  
  while (digitalRead(finCarrera_x) == HIGH) {
    digitalWrite(step_x, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(step_x, LOW);
    delayMicroseconds(stepDelay);
  }


  
}

// Define pin connections & motor's steps per revolution
const int dirPinX = 3;
const int stepPinX = 2;
const int dirPinY = 7;
const int stepPinY = 8;

#define MS1 4
#define MS2 5
#define MS3 6

const int stepsPerRevolution = 400;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(stepPinX, OUTPUT);

  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
}

void loop()
{
  // Set motor direction clockwise
  digitalWrite(dirPinX, HIGH);
  digitalWrite(dirPinY, HIGH);


  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);  
  
  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution*16; x++)
  {
    digitalWrite(stepPinX, HIGH);
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(500/16);
    digitalWrite(stepPinX, LOW);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(500/16);
    
  }
  delay(1000); // Wait a second
  
  // Set motor direction counterclockwise
  digitalWrite(dirPinX, LOW);
  digitalWrite(dirPinY, LOW);

  
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);

  // Spin motor quickly
  for(int x = 0; x < stepsPerRevolution; x++)
  {
    digitalWrite(stepPinX, HIGH);
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinX, LOW);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(1000);
  }
  delay(1000); // Wait a second
}

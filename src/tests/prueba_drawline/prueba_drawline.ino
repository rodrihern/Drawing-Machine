float Xmin = 0;
float Xmax = 210;
float Ymin = 0;
float Ymax = 297;
float Zmin = 0;


float Xpos = Xmax;
float Ypos = Ymax;


#define servoPin 13
#define VCC 12

// Pines de los drivers
#define step_x 2
#define step_y 8
#define dir_x 3
#define dir_y 7

#define StepDelay 1
#define LineDelay 2

#define MS1 4
#define MS2 5
#define MS3 6

// fines de carrera
#define finCarrera_y 9
#define finCarrera_x 10

int StepsPerMillimeterX = 200;
int StepsPerMillimeterY = 200;

void setup() {
  pinMode(servoPin, OUTPUT);
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);

  pinMode(step_x, OUTPUT);
  pinMode(step_y, OUTPUT);
  pinMode(dir_x, OUTPUT);
  pinMode(dir_y, OUTPUT);

  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);

  
  pinMode(finCarrera_x, INPUT_PULLUP);
  pinMode(finCarrera_y, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("*****************");
}

boolean verbose = true;

void loop() {
  draw_line(10, 10);
}                     


void draw_line(float x1,float y1) {

  if (verbose)
  {
    Serial.print("fx1, fy1: ");
    Serial.print(x1);
    Serial.print(",");
    Serial.print(y1);
    Serial.println("");
  }  

  //  Bring instructions within limits
  if (x1 >= Xmax) { 
    x1 = Xmax; 
  }
  if (x1 <= Xmin) { 
    x1 = Xmin; 
  }
  if (y1 >= Ymax) { 
    y1 = Ymax; 
  }
  if (y1 <= Ymin) { 
    y1 = Ymin; 
  }

  if (verbose)
  {
    Serial.print("Xpos, Ypos: ");
    Serial.print(Xpos);
    Serial.print(",");
    Serial.print(Ypos);
    Serial.println("");
  }

  if (verbose)
  {
    Serial.print("x1, y1: ");
    Serial.print(x1);
    Serial.print(",");
    Serial.print(y1);
    Serial.println("");
  }

  //  Convert coordinates to steps
  x1 = (int)(x1*StepsPerMillimeterX);
  y1 = (int)(y1*StepsPerMillimeterY);
  float x0 = Xpos;
  float y0 = Ypos;

  //  Let's find out the change for the coordinates
  long dx = abs(x1-x0);
  long dy = abs(y1-y0);
  /*
  int sx = x0<x1 ? StepInc : -StepInc;
  int sy = y0<y1 ? StepInc : -StepInc;
  */
  // ESTO NO SE BIEN QUE HACE PERO POR AHORA LO DEJAMOS

  if (x0<x1) {
    digitalWrite(dir_x, HIGH);
  }
  else {
    digitalWrite(dir_x, LOW);
  }

  if (y0<y1) {
    digitalWrite(dir_y, HIGH);
  }
  else {
    digitalWrite(dir_y, LOW);
  }

  long i;
  long over = 0;

  if (dx > dy) {
    for (i=0; i<dx; ++i) {
      //myStepperX.onestep(sx,STEP);

      digitalWrite(step_x, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_x, LOW);
      delayMicroseconds(1500);

      
      over+=dy;
      if (over>=dx) {
        over-=dx;
        //myStepperY.onestep(sy,STEP); ESTO HAY QUE CAMBIAR

        digitalWrite(step_y, HIGH);
        delayMicroseconds(1500);
        digitalWrite(step_y, LOW);
        delayMicroseconds(1500);
 
      }
    delay(StepDelay);
    }
  }
  else {
    for (i=0; i<dy; ++i) {
      //myStepperY.onestep(sy,STEP);

      digitalWrite(step_y, HIGH);
      delayMicroseconds(1500);
      digitalWrite(step_y, LOW);
      delayMicroseconds(1500);
      
      over+=dx;
      if (over>=dy) {
        over-=dy;
        //myStepperX.onestep(sx,STEP); //ESTO HAY QUE CAMBIAR

        digitalWrite(step_x, HIGH);
        delayMicroseconds(1500);
        digitalWrite(step_x, LOW);
        delayMicroseconds(1500);
        
      }
      delay(1);
    }    
  }

  if (verbose)
  {
    Serial.print("dx, dy:");
    Serial.print(dx);
    Serial.print(",");
    Serial.print(dy);
    Serial.println("");
  }

  if (verbose)
  {
    Serial.print("Going to (");
    Serial.print(x0);
    Serial.print(",");
    Serial.print(y0);
    Serial.println(")");
  }

  //  Delay before any next lines are submitted
  delay(LineDelay);
  //  Update the positions
  Xpos = x1;
  Ypos = y1;
}

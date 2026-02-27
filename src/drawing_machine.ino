#include <Servo.h>


#define LARGO_BUFFER 512

// Posicion del Servo para up y down
const int penZUp = 90;
const int penZDown = 0;

/*
 * El servo tiene que estar en un pin de pwm
 */

// Pines del servo
#define servoPin 11


// Pines de los drivers
#define step_x 2
#define step_y 8
#define dir_x 3
#define dir_y 7

/*
 *
 * PROBAR CON MICROSTEPING PARA EVITAR QUE SALTE PASOS
 * SE MUEVE MAS FLUIDO Y TIENE MAS TORQUE
 
 */
#define MS1 4
#define MS2 5
#define MS3 6

// fines de carrera
#define finCarrera_y 9
#define finCarrera_x 10


// settings para dibujar
int modoDePaso = 16; // 1 FULL STEP, 2 1/2 STEP, 4 1/4 STEP, 8 1/8 STEP, 16 1/16 STEP

const int stepsPorVuelta = 200*modoDePaso;
const int stepDelay = 1000 / modoDePaso;    // mientras mas chico, mas rapido gira. no usar valores menores a 500/modoDePaso
const int lineDelay = 0;
const int penDelay = 300;

Servo penServo;

struct posicion {
  float x; 
  float y; 
  float z;
  
};

// Posicion actual del amigo
struct posicion actuatorPos;




// Cuantos pasos para que se mueva 1mm
float StepsPerMillimeterX = 700.0/139.0 * modoDePaso;
float StepsPerMillimeterY = 700.0/139.0 * modoDePaso;

// Limites del dibujo, en mm
float Xmin = 0;
float Xmax = 228;
float Ymin = 0;
float Ymax = 244;
float Zmin = 0;
float Zmax = 1;

float Xpos = Xmax;
float Ypos = Ymin;
float Zpos = Zmax; 

// Poner en True para depurar.
boolean verbose = false;

//  Needs to interpret 
//  G1 for moving
//  G4 P300 (wait 150ms)
//  M300 S30 (pen down)
//  M300 S50 (pen up)
//  Discard anything with a (
//  Discard any other command!

char linea[ LARGO_BUFFER ];
char c;
int indice = 0;
bool esComentario = false, esPuntoComa = false;




void setup() {
  
  pinMode(servoPin, OUTPUT);

  pinMode(step_x, OUTPUT);
  pinMode(step_y, OUTPUT);
  pinMode(dir_x, OUTPUT);
  pinMode(dir_y, OUTPUT);

  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  pinMode(finCarrera_x, INPUT_PULLUP);
  pinMode(finCarrera_y, INPUT_PULLUP);

  
  Serial.begin( 9600 );
  Serial.setTimeout(1);
  
  penServo.attach(servoPin);
  penServo.write(penZUp);
  delay(100);

  //  Notifications!!!
  Serial.println("Drawing Machine AKA Borges esta vivito y coleando:)");
  Serial.print("X va desde "); 
  Serial.print(Xmin); 
  Serial.print(" hasta "); 
  Serial.print(Xmax); 
  Serial.println(" mm."); 
  Serial.print("Y va desde "); 
  Serial.print(Ymin); 
  Serial.print(" hasta "); 
  Serial.print(Ymax); 
  Serial.println(" mm."); 
  


  
  //seteamos el step
  tipo_step(modoDePaso);

  inicio();

  delay(100);

}

void loop() {
  // Serial reception - Mostly from Grbl, added semicolon support
    while ( Serial.available()>0 ) {
      c = Serial.read();
      if (( c == '\n') || (c == '\r') || (c=='B')) {             // End of line reached
        if ( indice > 0 ) {                        // Line is complete. Then execute!
          linea[ indice ] = '\0';                   // Terminate string
          if (verbose) { 
            Serial.print( "Received : "); 
            Serial.println( linea ); 
          }
          processIncomingLine( linea, indice );
          indice = 0;
        } 
        else { 
          if (verbose) { 
            Serial.print( "Received : "); 
            Serial.println( linea ); 
          }
        }
        esComentario = false;
        esPuntoComa = false;
        //Serial.println("ok");    
      } 
      else {
        if ( (esComentario) || (esPuntoComa) ) {   // Throw away all comment characters
          if ( c == ')' )  esComentario = false;     // End of comment. Resume line.
        } 
        else {
          if ( c <= ' ' ) {                           // Throw away whitepace and control characters
          } 
          else if ( c == '/' ) {                    // Block delete not supported. Ignore character.
          } 
          else if ( c == '(' ) {                    // Enable comments flag and ignore all characters until ')' or EOL.
            esComentario = true;
          } 
          else if ( c == ';' ) {
            esPuntoComa = true;
          } 
          else if ( indice >= LARGO_BUFFER-1 ) {
            Serial.println( "ERROR - lineBuffer overflow" );
            esComentario = false;
            esPuntoComa = false;
          } 
          else if ( c >= 'a' && c <= 'z' ) {        // Upcase lowercase
            linea[ indice++ ] = c-'a'+'A';
          } 
          else {
            linea[ indice++ ] = c;
          }
        }
      }
    }
}


void processIncomingLine( char* linea, int charNB ) {
  int currentIndex = 0;
  char buffer[ 64 ];                                 // Hope that 64 is enough for 1 parameter
  struct posicion newPos;

  newPos.x = 0.0;
  newPos.y = 0.0;

  //  Needs to interpret 
  //  G1 for moving
  //  G4 P300 (wait 150ms)
  //  G1 X60 Y30
  //  G1 X30 Y50
  //  M300 S30 (pen down)
  //  M300 S50 (pen up)
  //  Discard anything with a (
  //  Discard any other command!

  while( currentIndex < charNB ) {
    switch ( linea[ currentIndex++ ] ) {              // Select command, if any
    case 'U':
      penUp(); 
      break;
    case 'D':
      penDown(); 
      break;
    case 'G':
      buffer[0] = linea[ currentIndex++ ];          // /!\ Dirty - Only works with 2 digit commands
      //      buffer[1] = linea[ currentIndex++ ];
      //      buffer[2] = '\0';
      buffer[1] = '\0';

      switch ( atoi( buffer ) ){                   // Select G command
      case 0:                                   // G00 & G01 - Movement or fast movement. Same here
      case 1:
        // /!\ Dirty - Suppose that X is before Y
        char* indexX = strchr( linea+currentIndex, 'X' );  // Get X/Y position in the string (if any)
        char* indexY = strchr( linea+currentIndex, 'Y' );
        if ( indexY <= 0 ) {
          newPos.x = atof( indexX + 1); 
          newPos.y = actuatorPos.y;
        } 
        else if ( indexX <= 0 ) {
          newPos.y = atof( indexY + 1);
          newPos.x = actuatorPos.x;
        } 
        else {
          newPos.y = atof( indexY + 1);
          *indexY = '\0';
          newPos.x = atof( indexX + 1);
        }
        draw_line(newPos.x, newPos.y );
        //        Serial.println("ok");
        actuatorPos.x = newPos.x;
        actuatorPos.y = newPos.y;
        break;
      }
      break;
    case 'M':
      buffer[0] = linea[ currentIndex++ ];        // /!\ Dirty - Only works with 3 digit commands
      buffer[1] = linea[ currentIndex++ ];
      buffer[2] = linea[ currentIndex++ ];
      buffer[3] = '\0';
      switch ( atoi( buffer ) ){
      case 300:
        {
          char* indexS = strchr( linea+currentIndex, 'S' );
          float Spos = atof( indexS + 1);
          //         Serial.println("ok");
          if (Spos == 30) { 
            penDown(); 
          }
          if (Spos == 50) { 
            penUp(); 
          }
          break;
        }
      case 114:                                // M114 - Repport position
        Serial.print( "Absolute position : X = " );
        Serial.print( actuatorPos.x );
        Serial.print( "  -  Y = " );
        Serial.println( actuatorPos.y );
        break;
      default:
        Serial.print( "Command not recognized : M");
        Serial.println( buffer );
      }
    }
  }



}

void tipo_step(int modo){
  if (modo == 2) {
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, LOW);
    digitalWrite(MS3, LOW);
  }
  else if (modo == 4){
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, HIGH);
    digitalWrite(MS3, LOW);
  }
  else if (modo == 8) {
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, HIGH);
    digitalWrite(MS3, LOW);
  }
  else if (modo == 16) {
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, HIGH);
    digitalWrite(MS3, HIGH);
  }
  else {
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, LOW);
    digitalWrite(MS3, LOW);
    modoDePaso = 1;
    
  }
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
  //x1 = (int)(x1*StepsPerMillimeterX);
  //y1 = (int)(y1*StepsPerMillimeterY);
  float x0 = Xpos;
  float y0 = Ypos;

  //  Let's find out the change for the coordinates
  float dx = abs(x1-x0);
  float dy = abs(y1-y0);

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
    Serial.print(x1);
    Serial.print(",");
    Serial.print(y1);
    Serial.println(")");
  }
  Serial.println("ok");

  dx *= StepsPerMillimeterX;
  dy *= StepsPerMillimeterY;

  if (x0<x1) {
    digitalWrite(dir_x, HIGH);
  }
  else {
    digitalWrite(dir_x, LOW);
  }

  if (y0<y1) {
    digitalWrite(dir_y, LOW);
  }
  else {
    digitalWrite(dir_y, HIGH);
  }

  long i;
  long over = 0;

  if (dx > dy) {
    for (i=0; i<dx; ++i) {

      digitalWrite(step_x, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(step_x, LOW);
      delayMicroseconds(stepDelay);

      
      over+=dy;
      if (over>=dx) {
        over-=dx;

        digitalWrite(step_y, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(step_y, LOW);
        delayMicroseconds(stepDelay);
 
      }
    delayMicroseconds(stepDelay);
    }
  }
  else {
    for (i=0; i<dy; ++i) {

      digitalWrite(step_y, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(step_y, LOW);
      delayMicroseconds(stepDelay);
      
      over+=dx;
      if (over>=dy) {
        over-=dy;

        digitalWrite(step_x, HIGH);
        delayMicroseconds(stepDelay);
        digitalWrite(step_x, LOW);
        delayMicroseconds(stepDelay);
        
      }
      delayMicroseconds(stepDelay);
    }    
  }

  

  //  Delay before any next lines are submitted
  delay(lineDelay);
  //  Update the positions
  Xpos = x1;
  Ypos = y1;
}

//  Raises pen
void penUp() { 
  penServo.write(penZUp); 
  delay(penDelay); 
  Zpos=Zmax; 

  if (verbose) { 
    Serial.println("Pen up!"); 
    
  } 
}
//  Lowers pen
void penDown() { 
  penServo.write(penZDown); 
  delay(penDelay); 
  Zpos=Zmin; 

  if (verbose) { 
    Serial.println("Pen down."); 
    
    
  } 
}
  

void inicio() {
  penUp(); 
  digitalWrite(dir_x, HIGH);
  digitalWrite(dir_y, HIGH);
  
  while (digitalRead(finCarrera_x) == HIGH) {
    digitalWrite(step_x, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(step_x, LOW);
    delayMicroseconds(stepDelay);
  }

  while (digitalRead(finCarrera_y) == HIGH) {
    digitalWrite(step_y, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(step_y, LOW);
    delayMicroseconds(stepDelay);
  }
}

  

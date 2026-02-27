#define finCarrera_y 9
#define finCarrera_x 10

void setup() {
  pinMode(finCarrera_y, INPUT_PULLUP);
  pinMode(finCarrera_x, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("*******");

}

void loop() {
  if (digitalRead(finCarrera_x) == LOW) {
    Serial.println("Stop x");
  }

  if (digitalRead(finCarrera_y) == LOW) {
    Serial.println("Stop y");
  }
}

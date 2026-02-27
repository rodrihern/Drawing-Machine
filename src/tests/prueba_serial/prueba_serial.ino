char x;
void setup() {
 Serial.begin(9600);
 Serial.setTimeout(1);
}
void loop() {
  while (Serial.available() > 0) {
    x = Serial.read();
    Serial.print("Received: ");
    Serial.println(x);
  }
}

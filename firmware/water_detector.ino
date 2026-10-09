const int SENSOR_PIN = A0;   // water sensor analog output
const int BUZZER_PIN = 8;    // buzzer
const int THRESHOLD  = 300;  // tune this by testing

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(SENSOR_PIN);
  Serial.println(value);

  if (value > THRESHOLD) {
    tone(BUZZER_PIN, 1000);   // water detected, sound buzzer
  } else {
    noTone(BUZZER_PIN);
  }
  delay(100);
}

 #include <Arduino.h>
 // =====  basic moter test for high speed ===== 
// Motor Pins
const int stepPin = 25;
const int dirPin = 26;
const int enPin = 27;

// YAHAN SPEED CONTROL KAREIN //
// Is number ko jitna chhota karenge, speed utni badhegi.
// Agar motor vibrate kare aur chale nahi, to is number ko thoda badha dein.
const int stepDelay = 400; // Microseconds mein delay

void setup() {
  Serial.begin(115200);
  Serial.println("\n\n--- High Speed Diagnostic Test ---");
  Serial.print("Step Delay set to: ");
  Serial.println(stepDelay);

  // Pins ko setup karna
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);

  // Motor driver ko ON karna
  digitalWrite(enPin, LOW);
  delay(100);
}

void loop() {
  // === TEST 1: Direction 1 (LOW) ===
  Serial.println("Test 1: Turning in one direction (3 seconds)...");
  digitalWrite(dirPin, LOW);
  delay(1);
  
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay); // Yahan change kiya hai
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay); // Yahan bhi change kiya hai
  }
  
  Serial.println("Motor stopped.");
  Serial.println("------------------------------------");
  delay(2000); // Rukne ka time thoda kam kar diya


  // === TEST 2: Direction 2 (HIGH) ===
  Serial.println("Test 2: Turning in OPPOSITE direction (3 seconds)...");
  digitalWrite(dirPin, HIGH);
  delay(1);
  
  startTime = millis();
  while (millis() - startTime < 3000) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay); // Yahan change kiya hai
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay); // Yahan bhi change kiya hai
  }

  Serial.println("Motor stopped.");
  Serial.println("====================================");
  delay(2000); // Rukne ka time thoda kam kar diya
}
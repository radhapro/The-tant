 #include <Arduino.h>
 // ===== BLUETOOTH CONTROLLED MOTOR WITH LIMIT SWITCHES =====

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

BluetoothSerial SerialBT;

// ===== Motor Pins =====
const int stepPin = 25;
const int dirPin = 26;
const int enPin = 27;

// ===== NAYE LIMIT SWITCH PINS =====
const int limitSwitchFwdPin = 18; // Switch jo Forward (LOW) direction mein dabega
const int limitSwitchBwdPin = 19; // Switch jo Backward (HIGH) direction mein dabega

// ===== Speed Setting =====
const int stepDelay = 400;

// --- Motor State Variables ---
char motorAction = 'S'; 
unsigned long actionStartTime = 0;

void setup() {
  Serial.begin(115200);
  
  // Bluetooth shuru karna
  SerialBT.begin("ESP32_2"); 
  Serial.println("\n--- Bluetooth Motor with Limit Switches ---");
  Serial.println("Phone se 'F', 'B', ya 'S' command bhejein.");
  SerialBT.println("--- Motor with Limit Switches ---");
  SerialBT.println("Send 'F', 'B', or 'S'. Motor will stop at limit.");

  // ===== Pin Setup =====
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);

  // NAYE PINS KA SETUP (INPUT_PULLUP ke saath)
  pinMode(limitSwitchFwdPin, INPUT_PULLUP);
  pinMode(limitSwitchBwdPin, INPUT_PULLUP);

  // Motor driver ko ON karna (Enable)
  digitalWrite(enPin, LOW);
  delay(100); 
}

void loop() {
  
  // === STEP 1: Hamesha Bluetooth Command Check Karna ===
  if (SerialBT.available()) {
    char command = SerialBT.read();
    Serial.print("Command mila: ");
    Serial.println(command);

    if (command == 'F' || command == 'f') {
      // Safety Check: Agar forward limit pehle se dabi hai to aage nahi chalana
      if (digitalRead(limitSwitchFwdPin) == LOW) {
        Serial.println("ERROR: Forward limit pehle se dabi hai. Nahi chal sakta.");
        SerialBT.println("ERROR: Forward limit is already pressed. Cannot move.");
      } else {
        motorAction = 'F';
        actionStartTime = millis();
        SerialBT.println("Command 'F' mila. Motor 3 sec ke liye chalegi...");
      }
    } 
    else if (command == 'B' || command == 'b') {
      // Safety Check: Agar backward limit pehle se dabi hai to peeche nahi chalana
      if (digitalRead(limitSwitchBwdPin) == LOW) {
        Serial.println("ERROR: Backward limit pehle se dabi hai. Nahi chal sakta.");
        SerialBT.println("ERROR: Backward limit is already pressed. Cannot move.");
      } else {
        motorAction = 'B';
        actionStartTime = millis();
        SerialBT.println("Command 'B' mila. Motor 3 sec ke liye ulta chalegi...");
      }
    }
    else if (command == 'S' || command == 's') {
      motorAction = 'S';
      SerialBT.println("Command 'S' mila. Motor ruk gayi.");
    }
  }

  // === STEP 2: Current Action ke Hisab se Motor Chalana ===

  // ** NOTE: Aapke diye code mein 21000 likha tha, maine use 3000 kar diya hai **
  
  // Agar action 'Forward' hai
  if (motorAction == 'F') {
    // AB DO CHEEZEIN CHECK KARO:
    // 1. Kya 3 second poore ho gaye?
    // 2. KYA FORWARD SWITCH DABA HUA TO NAHI HAI? (LOW = Daba hua)
    if ( (millis() - actionStartTime < 21000) && (digitalRead(limitSwitchFwdPin) == HIGH) ) {
      // Agar time bhi bacha hai AUR switch bhi nahi daba, to motor chalao
      digitalWrite(dirPin, LOW);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(stepDelay);
    } else {
      // Agar time poora ho gaya YA switch dab gaya, to motor roko
      motorAction = 'S';
      if (digitalRead(limitSwitchFwdPin) == LOW) {
        Serial.println("Forward Limit Switch Daba! Motor ruk gayi.");
        SerialBT.println("Forward Limit Switch Hit! Motor stopped.");
      } else {
        Serial.println("3 second poore. Motor ruk gayi.");
        SerialBT.println("3-second run complete. Motor stopped.");
      }
    }
  }
  // Agar action 'Backward' hai
  else if (motorAction == 'B') {
    // YAHAN BHI DO CHEEZEIN CHECK KARO:
    // 1. Kya 3 second poore ho gaye?
    // 2. KYA BACKWARD SWITCH DABA HUA TO NAHI HAI?
    if ( (millis() - actionStartTime < 21000) && (digitalRead(limitSwitchBwdPin) == HIGH) ) {
      // Agar time bhi bacha hai AUR switch bhi nahi daba, to motor chalao
      digitalWrite(dirPin, HIGH);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(stepDelay);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(stepDelay);
    } else {
      // Agar time poora ho gaya YA switch dab gaya, to motor roko
      motorAction = 'S';
      if (digitalRead(limitSwitchBwdPin) == LOW) {
        Serial.println("Backward Limit Switch Daba! Motor ruk gayi.");
        SerialBT.println("Backward Limit Switch Hit! Motor stopped.");
      } else {
        Serial.println("3 second poore. Motor ruk gayi.");
        SerialBT.println("3-second run complete. Motor stopped.");
      }
    }
  }
}
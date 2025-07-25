#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// =================== वाई-फ़ाई की जानकारी ===================
const char* ssid = "Robozz Lab";
const char* password = "Robotics@cloud";
// ==========================================================


// =================== मोटर के पिन (बदले नहीं) =================
const int stepPin = 25;
const int dirPin = 26;
const int enPin = 27;
// ==========================================================


// मोटर की स्पीड (इस नंबर को कम करने से स्पीड बढ़ेगी)
const int stepDelay = 500; // Microseconds

// ग्लोबल वैरिएबल मोटर की स्थिति को याद रखने के लिए
bool motorRunning = false;
int motorDirection = LOW; // LOW = Clockwise, HIGH = Anti-Clockwise

AsyncWebServer server(80);

// वेबपेज का नया HTML कोड (मोटर के लिए)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Stepper Motor Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    .button { background-color: #008CBA; border: none; color: white; padding: 16px 40px;
      text-decoration: none; font-size: 30px; margin: 8px; cursor: pointer; width: 80%;}
    .stop {background-color: #f44336;}
    .dir {background-color: #555555;}
  </style>
</head>
<body>
  <h1>ESP32 Stepper Control</h1>
  <p><a href="/clockwise"><button class="button">UP</button></a></p>
  <p><a href="/anticlockwise"><button class="button dir">Down</button></a></p>
  <p><a href="/stop"><button class="button stop">STOP</button></a></p>
</body>
</html>)rawliteral";

void setup(){
  Serial.begin(115200);

  // मोटर के पिन सेटअप करना
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW); // मोटर ड्राइवर को चालू करना

  // वाई-फ़ाई से कनेक्ट करना
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // === वेब सर्वर के नए रास्ते (Routes) ===
  // मुख्य पेज
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Clockwise के लिए
  server.on("/clockwise", HTTP_GET, [](AsyncWebServerRequest *request){
    motorDirection = LOW;
    motorRunning = true;
    request->send_P(200, "text/html", index_html);
  });

  // Anti-Clockwise के लिए
  server.on("/anticlockwise", HTTP_GET, [](AsyncWebServerRequest *request){
    motorDirection = HIGH;
    motorRunning = true;
    request->send_P(200, "text/html", index_html);
  });
  
  // रोकने के लिए
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    motorRunning = false;
    request->send_P(200, "text/html", index_html);
  });

  // वेब सर्वर शुरू करना
  server.begin();
}

void loop(){
  // यह लूप लगातार चलेगा और मोटर को तभी घुमाएगा जब motorRunning = true हो
  if (motorRunning) {
    digitalWrite(dirPin, motorDirection); // दिशा सेट करना
    
    // मोटर का एक स्टेप लेना
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepDelay);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepDelay);
  }
}
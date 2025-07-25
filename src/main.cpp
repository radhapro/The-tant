 #include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// ******************************************************
// ***** यहाँ अपना Wi-Fi का नाम और पासवर्ड डालें! ******
const char* ssid = "Robozz Lab";
const char* password = "Robotics@cloud";
// ******************************************************

// LED किस पिन पर है
const int ledPin = 2; 

// वेब सर्वर के लिए ऑब्जेक्ट बना रहे हैं (पोर्ट 80 पर)
AsyncWebServer server(80);

// यह HTML कोड है जो वेबपेज पर दिखेगा
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    .button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;
      text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
    .button2 {background-color: #f44336;}
  </style>
</head>
<body>
  <h1>ESP32 Web Server</h1>
  <p><a href="/on"><button class="button">ON</button></a></p>
  <p><a href="/off"><button class="button button2">OFF</button></a></p>
</body>
</html>)rawliteral";

void setup(){
  // सीरियल मॉनिटर शुरू करें ताकि हम IP एड्रेस देख सकें
  Serial.begin(115200);

  // LED पिन को आउटपुट सेट करें
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Wi-Fi से कनेक्ट होना शुरू करें
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // जब कनेक्ट हो जाए, तो IP एड्रेस प्रिंट करें
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // --- वेब सर्वर के रास्ते (Routes) सेट करें ---
  // जब कोई मुख्य पेज (/) पर आए, तो उसे HTML कोड भेजो
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // जब कोई /on पर आए, तो LED ऑन करो
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);
    request->send_P(200, "text/html", index_html);
  });

  // जब कोई /off पर आए, तो LED ऑफ करो
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);
    request->send_P(200, "text/html", index_html);
  });

  // वेब सर्वर शुरू करो
  server.begin();
}

void loop(){
  // लूप में कुछ नहीं करना है, सर्वर अपना काम खुद करता है
}
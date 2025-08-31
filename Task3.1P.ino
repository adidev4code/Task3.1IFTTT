//  Code:
  
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// WiFi credentials
char ssid[] = "Alfred Nobel";
char pass[] = "CUPunjab";

// IFTTT Webhooks
char server[] = "maker.ifttt.com";
String IFTTT_Key = "m65w3Oxxb5DrcXpvMmRzmNeUBfWbtuTIV00XdMXQz";
String Event_SunlightOn = "sunlight_started";
String Event_SunlightOff = "sunlight_stopped";

WiFiClient wifi;
HttpClient client = HttpClient(wifi, server, 80);

// Light sensor
const int lightSensorPin = A0;
int threshold = 600;  // Adjust depending on your sensor & lighting

// Time tracking
unsigned long burstStartTime = 0;
unsigned long totalSunlight = 0; // milliseconds
bool inSunlight = false;

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  int sensorValue = analogRead(lightSensorPin);
  Serial.print("Light: ");
  Serial.println(sensorValue);

  unsigned long now = millis();

  // Case 1: Sunlight starts
  if (sensorValue > threshold && !inSunlight) {
    inSunlight = true;
    burstStartTime = now;

    // Send trigger: sunlight started
    triggerIFTTT(Event_SunlightOn,
                 String(totalSunlight / 60000), // total minutes so far
                 "0",                           // burst just started
                 String(sensorValue));
  }

  // Case 2: Sunlight stops
  else if (sensorValue <= threshold && inSunlight) {
    inSunlight = false;
    unsigned long burstDuration = now - burstStartTime;
    totalSunlight += burstDuration;

    // Send trigger: sunlight stopped
    triggerIFTTT(Event_SunlightOff,
                 String(totalSunlight / 60000), // total minutes
                 String(burstDuration / 1000),  // last burst seconds
                 String(sensorValue));
  }

  delay(5000); // check every 5 sec
}

void triggerIFTTT(String event, String value1, String value2, String value3) {
  String url = "/trigger/" + event + "/with/key/" + IFTTT_Key;

  client.beginRequest();
  client.post(url);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Connection", "close");

  // Build JSON payload
  String json = "{\"value1\":\"" + value1 + "\", \"value2\":\"" + value2 + "\", \"value3\":\"" + value3 + "\"}";
  client.sendHeader("Content-Length", json.length());
  client.beginBody();
  client.print(json);
  client.endRequest();

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Event: ");
  Serial.println(event);
  Serial.print("Status: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

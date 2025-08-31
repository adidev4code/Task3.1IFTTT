//  Code:
  
#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>
#include <Wire.h>
#include <BH1750.h>

// ===== WiFi Configuration =====
char ssid[] = "Adidev";
char pass[] = "@admin123456789";

// ===== IFTTT Webhooks =====
char server[] = "maker.ifttt.com";
String IFTTT_Key = "m65w3Oxxb5DrcXpvMmRzmNeUBfWbtuTIV00XdMX-Qz-";
String Event_SunlightOn = "sunlight_started";
String Event_SunlightOff = "sunlight_stopped";

// ===== Global Objects =====
WiFiSSLClient wifi;  // secure client for HTTPS
HttpClient client = HttpClient(wifi, server, 443);
BH1750 lightMeter;

// ===== Light Threshold =====
float thresholdLux = 300.0; // adjust for terrarium lighting

// ===== Time Tracking =====
unsigned long burstStartTime = 0;
unsigned long totalSunlight = 0; // milliseconds
bool inSunlight = false;

// ===== Setup =====
void setup() {
  Serial.begin(9600);
  connectWiFi();
  initLightSensor();
}

// ===== Loop =====
void loop() {
  float lux = readLightLevel();
  processLightState(lux);
  delay(1000); // check every 1 sec
}

// ===== WiFi Connection =====
void connectWiFi() {
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }
  Serial.println("\nConnected to WiFi!");
}

// ===== Sensor Initialization =====
void initLightSensor() {
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("BH1750 ready!");
  } else {
    Serial.println("Error: BH1750 not detected!");
    while (1);
  }
}

// ===== Read Sensor =====
float readLightLevel() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  return lux;
}

// ===== Process Sunlight Logic =====
void processLightState(float lux) {
  unsigned long now = millis();

  if (lux > thresholdLux && !inSunlight) {
    inSunlight = true;
    burstStartTime = now;
    Serial.println("☀️ Sunlight detected!");
    triggerIFTTT(Event_SunlightOn,
                 String(totalSunlight / 60000),
                 "0",
                 String(lux));
  }
  else if (lux <= thresholdLux && inSunlight) {
    inSunlight = false;
    unsigned long burstDuration = now - burstStartTime;
    totalSunlight += burstDuration;
    Serial.println("🌙 Sunlight stopped!");
    triggerIFTTT(Event_SunlightOff,
                 String(totalSunlight / 60000),
                 String(burstDuration / 1000),
                 String(lux));
  }
}

// ===== IFTTT Trigger =====
void triggerIFTTT(String event, String value1, String value2, String value3) {
  String url = "/trigger/" + event + "/with/key/" + IFTTT_Key;

  Serial.print("Connecting to IFTTT for event: ");
  Serial.println(event);

  client.beginRequest();
  client.post(url);
  client.sendHeader("Content-Type", "application/json");
  client.sendHeader("Connection", "close");

  String json = "{\"value1\":\"" + value1 +
                "\", \"value2\":\"" + value2 +
                "\", \"value3\":\"" + value3 + "\"}";
  client.sendHeader("Content-Length", json.length());
  client.beginBody();
  client.print(json);
  client.endRequest();

  client.setTimeout(10000); // wait up to 10 seconds for response

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Event: ");
  Serial.println(event);
  Serial.print("Status: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
}

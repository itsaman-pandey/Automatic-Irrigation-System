#include <WiFi.h>
#include <WebServer.h>
#include "html.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
WebServer server(80);

const int sensor_pin = 39;
const int relaypin = 25;
unsigned long relayStartTime = 0;
unsigned long relayDuration = 0;
bool isRelayActive = false;
String relayEntries[2] = {"N/A", "N/A"};

const char* ssid = "aman";
const char* password = "12340000";

void MainPage() {
  String _html_page = html_page;
  server.send(200, "text/html", _html_page);
}

void ReadSensorValues() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    server.send(500, "text/plain", "Failed to read from BME280 sensor!");
    return;
  }

  String sensorData = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
  server.send(200, "application/json", sensorData);
}

void SoilMoisture() {
  double t = double(analogRead(sensor_pin));
  int moisture = (100 - ((t / 4095.00) * 100));
 // String moisture = String(moistureValue);
  server.send(200, "text/plain", String(moisture));
  
  // Check moisture level for automatic relay control
  if (moisture < 20 && !isRelayActive) {
    ActivateRelay();
  } else if (moisture >= 40 && isRelayActive) {
    DeactivateRelay();
  }
}

void ActivateRelay() {
  if (!isRelayActive) {
    isRelayActive = true;
    relayStartTime = millis();
    digitalWrite(relaypin, LOW);
    UpdateRelayStatus();
    server.send(200, "text/plain", "Relay Activated");
  }
}

void DeactivateRelay() {
  if (isRelayActive) {
    isRelayActive = false;
    relayDuration = millis() - relayStartTime;
    digitalWrite(relaypin, HIGH);
    UpdateRelayStatus();
    UpdateRelayEntries();
    server.send(200, "text/plain", "Relay Deactivated");
  }
}

void UpdateRelayStatus() {
  String relayStatus = isRelayActive ? "ON" : "OFF";
  server.send(200, "text/plain", relayStatus);
}

void UpdateRelayEntries() {
  relayEntries[1] = relayEntries[0];
  relayEntries[0] = "Start Time: " + String(relayStartTime) + " Duration: " + String(relayDuration) + " ms";
}

void setup() {
  Serial.begin(115200);
  pinMode(relaypin, OUTPUT);

  bool status;
  status = bme.begin(0x76);
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- BME CONNECTED --");
  Serial.println();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());

  server.on("/", MainPage);
  server.on("/readSensorValues", ReadSensorValues);
  server.on("/readMoisture", SoilMoisture);
  server.on("/activateRelay", ActivateRelay);
  server.on("/deactivateRelay", DeactivateRelay);
  server.on("/readRelayStatus", UpdateRelayStatus);
  server.begin();

  delay(1000);
}

void loop() {
  printValues();
  server.handleClient();
}
void printValues() {
  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from BME280 sensor!");
    return;
  }

  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Temperature: ");
  display.setTextSize(2);
  display.setCursor(0, 10);
  display.print(temperature);
  display.print(" C");

  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(humidity);
  display.print(" %");

  display.display();
}

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <DHT.h>
#include <ESPmDNS.h>
#include <Button2.h>

#define BOOT_PIN        0
#define LED_PIN         16
#define DTH_SENSOR_PIN  22
#define ADC_PIN         32
#define DHTTYPE DHT11 // DHT 22  (AM2302), AM2321

AsyncWebServer server(80);
DHT dht(DTH_SENSOR_PIN, DHTTYPE);
Button2 button(BOOT_PIN);

void smartConfigStart(Button2 &b)
{
    Serial.println("smartConfigStart...");
    WiFi.disconnect();
    WiFi.beginSmartConfig();
    while (!WiFi.smartConfigDone()) {
        Serial.print(".");
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        delay(200);
    }
    digitalWrite(LED_PIN, HIGH);
    WiFi.stopSmartConfig();
    Serial.println();
    Serial.print("smartConfigStop Connected:");
    Serial.print(WiFi.SSID());
    Serial.print("PSW: ");
    Serial.println(WiFi.psk());
}

void buttonClicked(const char *id)
{
    bool state = !digitalRead(LED_PIN);
    digitalWrite(LED_PIN, state);
    ESPDash.updateStatusCard("state", !state);
}

bool serverBegin()
{
    static bool isBegin = false;
    if (isBegin) {
        return true;
    }
    if (!ESPDash.init(server)) { // Initiate ESPDash and attach your Async webserver instance
        Serial.println("SPIFFS Mount Failed. Formatted... Please Upload Static DASH Files again.");
        return false;
    }
    isBegin = true;
    if (MDNS.begin("soil")) {
        Serial.println("MDNS responder started");
    }
    // Add Respective Cards
    ESPDash.addTemperatureCard("temp1", "Temperature", 0, 0);
    ESPDash.addHumidityCard("hum1", "Humidity", 0);
    ESPDash.addHumidityCard("soil", "Soil Humidity", 0);
    ESPDash.addButtonCard("btn1", "LED Button");
    ESPDash.addStatusCard("state", "LED Status", digitalRead(LED_PIN));
    ESPDash.attachButtonClick(buttonClicked);
    server.begin();
    MDNS.addService("http", "tcp", 80);
    return true;
}


void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
    WiFi.mode(WIFI_STA);
    WiFi.begin();

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.printf("WiFi connect fail!,please restart retry,or long press BOOT button enter smart config mode\n");
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
    button.setLongClickHandler(smartConfigStart);
}

void loop()
{
    static uint64_t timestamp;
    button.loop();
    if (millis() - timestamp > 1000 ) {
        timestamp = millis();
        if (WiFi.status() == WL_CONNECTED) {
            if (serverBegin()) {
                float h = dht.readHumidity();
                float t = dht.readTemperature();
                if (!isnan(h) && !isnan(t) ) {
                    ESPDash.updateTemperatureCard("temp1", (int)t);
                    ESPDash.updateHumidityCard("hum1", (int)h);
                } else {
                    Serial.println("Failed to read from DHT sensor!");
                }
                ESPDash.updateHumidityCard("soil", map(analogRead(ADC_PIN), 0, 4096, 100, 0));
            }
        } else
            digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
}
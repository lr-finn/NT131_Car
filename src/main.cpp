#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>

#include "auto_mode.h"
#include "motors.h"
#include "http.h"

// Define pin connections
#include "config.h"

// WiFi
const char *ssid = "Finn";
const char *password = "123456789";

// Setup
void setupUltrasonic()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void setup()
{
    Serial.begin(115200);
    motors::setup();
    setupUltrasonic();
    initializeServo();

    // Phát WiFi ở chế độ Access Point
    WiFi.softAP("ESP32_AP", "12345678");
    Serial.println("ESP32 đang phát WiFi:");
    Serial.print("SSID: ");
    Serial.println(WiFi.softAPSSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    http::initServer();
}

void loop() {
    http::listen();
}


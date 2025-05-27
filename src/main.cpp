#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Servo.h>
#include <WebServer.h>
#include <ArduinoOTA.h>

#include "motors.h"

// Define pin connections
#define TRIG_PIN 32
#define ECHO_PIN 33
#define SERVO_PIN 15

// WiFi
const char *ssid = "Finn";
const char *password = "123456789";

Servo servo;

// Setup
void setupUltrasonic()
{
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

// Measure distance using ultrasonic sensor
long measureDistance()
{
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    return int(duration / 29 / 2); // Convert to cm
}

// Scan and move based on distance
void scanAndMove()
{
    long distance = measureDistance();
    Serial.print("distance: ");
    Serial.println(distance);

    if (distance < 20)
    { // If an obstacle is detected within 20 cm
        stopMotors();
        servo.write(45); // Adjust servo position if needed
        delay(2000);

        // Decide direction based on distance readings
        long leftDistance = measureDistance(); // Measure left distance
        Serial.print("Letf distance: ");
        Serial.println(leftDistance);
        delay(2000);
        servo.write(135); // Adjust servo position to look right
        delay(2000);
        long rightDistance = measureDistance(); // Measure right distance
        Serial.print("RIgth distance: ");
        Serial.println(rightDistance);
        delay(2000);

        if ((leftDistance > 20) && (leftDistance > rightDistance))
        {
            forwardLeft();
            delay(2000);
        }
        else if ((leftDistance < rightDistance) && (rightDistance > 20))
        {
            forwardRight();
            delay(2000);
        }
        else if ((leftDistance < 20) && (rightDistance < 20))
        {
            backwardLeft();
            delay(2000);
        }
    }
    else
    {
        moveForward();
        delay(2000);
    }

    servo.write(90); // Reset servo position
    delay(2000);
}

void setup()
{
    Serial.begin(115200);
    setupMotors();
    setupUltrasonic();
    servo.attach(SERVO_PIN);
    servo.write(90); // Set initial servo position

    // Phát WiFi ở chế độ Access Point
    WiFi.softAP("ESP32_AP", "12345678");
    Serial.println("ESP32 đang phát WiFi:");
    Serial.print("SSID: ");
    Serial.println(WiFi.softAPSSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());

    Serial.println("Connected to WiFi");

    server.on("/", handleHttpEvent);
    server.begin();
}

void loop()
{
    server.handleClient();
    // scanAndMove(); // Continuously scan and move based on distance
}
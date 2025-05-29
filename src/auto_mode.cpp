#include "Arduino.h"
#include <ESP32Servo.h>
#include "auto_mode.h"
#include "motors.h"
#include "config.h"

Servo servo;

void initializeServo()
{
    servo.attach(SERVO_PIN);
    servo.write(90); // Set initial servo position
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

void autoModeTask(void *pvParameters)
{
    Serial.println("Auto mode started");
    while (true)
    {
        scanAndMove();
    }
}

void scanAndMove()
{
    servo.write(90); // Reset servo position
    delay(500); // Allow servo to stabilize

    long distance = measureDistance();
    Serial.print("distance: ");
    Serial.println(distance);

    if (distance < 20)
    { // If an obstacle is detected within 20 cm
        motors::stopMotors();
        delay(1000);
        servo.write(45); // Adjust servo position if needed
        delay(1000);

        // Decide direction based on distance readings
        long leftDistance = measureDistance(); // Measure left distance
        Serial.print("Letf distance: ");
        Serial.println(leftDistance);
        delay(1000);
        servo.write(135); // Adjust servo position to look right
        delay(1000);
        long rightDistance = measureDistance(); // Measure right distance
        Serial.print("RIgth distance: ");
        Serial.println(rightDistance);
        delay(1000);

        if ((leftDistance > 20) && (leftDistance > rightDistance))
        {
            motors::forwardLeft();
            delay(100);
        }
        else if ((leftDistance < rightDistance) && (rightDistance > 20))
        {
            motors::forwardRight();
            delay(100);
        }
        else if ((leftDistance < 20) && (rightDistance < 20))
        {
            motors::backwardLeft();
            delay(100);
        }
    }
    else
    {
        motors::moveForward();
        delay(100);
    }
}
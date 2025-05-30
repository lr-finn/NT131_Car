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

    long duration = pulseIn(ECHO_PIN, HIGH, 20000); // Thêm timeout 20ms để tránh treo chương trình
    if (duration == 0)
    {
        return 400; // Khoảng cách ngoài phạm vi đo (giả định cảm biến tối đa 400 cm)
    }
    return duration / 29 / 2; // Chuyển đổi thành cm
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
    delay(500);      // Allow servo to stabilize

    long distance = measureDistance();
    Serial.print("distance: ");
    Serial.println(distance);

    if (distance < 25)
    { // If an obstacle is detected within 20 cm
        motors::stopMotors();
        delay(1000);
        servo.write(45); // Adjust servo position if needed
        delay(1000);
        long rightDistance = measureDistance(); // Measure left distance
        Serial.print("Right distance: ");
        Serial.println(rightDistance);
        delay(1000);
        servo.write(135); // Adjust servo position to look right
        delay(1000);
        long leftDistance = measureDistance(); // Measure right distance
        Serial.print("Left distance: ");
        Serial.println(leftDistance);
        delay(1000);
        servo.write(90); // Reset servo position
        delay(500);      // Allow servo to stabilize
        if ((leftDistance > 20) && (leftDistance > rightDistance))
        {
            motors::moveBackward();
            delay(200);
            motors::stopMotors();
            delay(500);
            motors::turnLeft();
            delay(500);
            motors::stopMotors();
            delay(1000);
        }
        else if ((leftDistance < rightDistance) && (rightDistance > 20))
        {
            motors::moveBackward();
            delay(200);
            motors::stopMotors();
            delay(500);
            motors::turnRight();
            delay(500);
            motors::stopMotors();
            delay(1000);
        }
        else if ((leftDistance < 25) && (rightDistance < 25))
        {
            motors::moveBackward();
            delay(200);
            motors::stopMotors();
            delay(1000);
            motors::turnRight();
            delay(500);
            motors::stopMotors();
            delay(1000);
        }
    }
    else
    {
        motors::moveForward();
    }
}
#include "http.h"
#include "motors.h"
#include <Arduino.h>
#include <WebServer.h>
#include "config.h"
#include "auto_mode.h"

WebServer server(HTTP_PORT);

void handleHttpEvent();

void http::initServer()
{
    server.on("/", handleHttpEvent);
    server.begin();
    Serial.println("HTTP server started on port " + HTTP_PORT);
}

void http::listen() {
    server.handleClient();
}

TaskHandle_t autoModeTaskHandle = NULL;

void startAutoModeTask() {
    if (autoModeTaskHandle != NULL) return; // Task already running
    xTaskCreatePinnedToCore(
        autoModeTask, // Function to be called
        "ScanAndMoveTask", // Name of the task
        10000, // Stack size in words
        NULL, // Task input parameter
        1, // Task priority
        &autoModeTaskHandle, // Task handle
        0 // Core where the task should run (1 for core 1, 0 for core 0)
    );
}

void stopAutoModeTask() {
    if (autoModeTaskHandle != NULL) {
        vTaskDelete(autoModeTaskHandle);
        autoModeTaskHandle = NULL;
    }
}

void handleHttpEvent()
{
    static bool mode = 0;
    String state = server.arg("State");

    if (state == "W") {
        startAutoModeTask();
        mode = 1;
        server.send(200, "text/plain", "Auto Mode");
    }
    else if (state == "w") {
        stopAutoModeTask();
        mode = 0;
        server.send(200, "text/plain", "Manual Mode");
    }

    if (mode)
    {
        server.send(200, "text/plain", "Auto Mode is running");
        return;
    }
    
    if (state == "F")
    {
        motors::moveForward();
        server.send(200, "text/plain", "Forward");
    }
    else if (state == "B")
    {
        motors::moveBackward();
        server.send(200, "text/plain", "Backward");
    }
    else if (state == "L")
    {
        motors::turnLeft();
        server.send(200, "text/plain", "Left");
    }
    else if (state == "R")
    {
        motors::turnRight();
        server.send(200, "text/plain", "Right");
    }
    else if (state == "G")
    {
        motors::forwardLeft();
        server.send(200, "text/plain", "Forward Left");
    }
    else if (state == "I")
    {
        motors::forwardRight();
        server.send(200, "text/plain", "Forward Right");
    }
    else if (state == "H")
    {
        motors::backwardLeft();
        server.send(200, "text/plain", "Backward Left");
    }
    else if (state == "J")
    {
        motors::backwardRight();
        server.send(200, "text/plain", "Backward Right");
    }
    else if (state == "S")
    {
        motors::stopMotors();
        server.send(200, "text/plain", "Stop");
    }
    else
    {
        server.send(400, "text/plain", "Invalid Command");
    }
}
#include "http.h"
#include "motors.h"
#include <Arduino.h>
#include <WebServer.h>
#include "config.h"

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

void handleHttpEvent()
{
    Serial.println("Handle connected");
    String state = server.arg("State");
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
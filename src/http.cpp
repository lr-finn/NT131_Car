#include "http.h"
#include "motors.h"
#include <Arduino.h>
#include <WebServer.h>

WebServer server(80);

void handleHttpEvent()
{
    Serial.println("Handle connected");
    String state = server.arg("State");
    if (state == "F")
    {
        moveForward();
        server.send(200, "text/plain", "Forward");
    }
    else if (state == "B")
    {
        moveBackward();
        server.send(200, "text/plain", "Backward");
    }
    else if (state == "L")
    {
        turnLeft();
        server.send(200, "text/plain", "Left");
    }
    else if (state == "R")
    {
        turnRight();
        server.send(200, "text/plain", "Right");
    }
    else if (state == "G")
    {
        forwardLeft();
        server.send(200, "text/plain", "Forward Left");
    }
    else if (state == "I")
    {
        forwardRight();
        server.send(200, "text/plain", "Forward Right");
    }
    else if (state == "H")
    {
        backwardLeft();
        server.send(200, "text/plain", "Backward Left");
    }
    else if (state == "J")
    {
        backwardRight();
        server.send(200, "text/plain", "Backward Right");
    }
    else if (state == "S")
    {
        stopMotors();
        server.send(200, "text/plain", "Stop");
    }
    else
    {
        server.send(400, "text/plain", "Invalid Command");
    }
}
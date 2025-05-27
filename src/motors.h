#pragma once

namespace motors {
    void setup();
    
    // Control functions for the car
    void moveForward();
    void moveBackward();
    void turnLeft();
    void turnRight();
    void forwardLeft();
    void forwardRight();
    void backwardLeft();
    void backwardRight();
    void stopMotors();

}
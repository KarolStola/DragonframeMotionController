#ifndef DRAGONFRAME_MOTION_CONTROLLER_H
#define DRAGONFRAME_MOTION_CONTROLLER_H

#include <Arduino.h>

class DragonframeMotionController
{
public:
    void Initialize(class DragonframeDevice * newDragonframeDevice);
    void HandleMessage(const String & message);
    virtual ~DragonframeMotionController();

private:
    bool IsMessageHi(String & message);
    void ParseMessage(String & message);

    const char * messageHi = "hi";
    const char * messageCheckMotorMovingStatus = "ms";
    const char * messageMoveMotor = "mm";
    const char * messageQueryMotorPosition = "mp";
    const char * messageStopMotor = "sm";
    const char * messageStopAllMotors = "sa";
    const char * messageJogMotor = "jm";
    const char * messageInchMotor = "im";
    const char * messageSetMaxStepsPerSecond = "pr";
    const char * messageZeroMotorPosition = "zm";
    const char * messageSetMotorPosition = "np";
    const char * messagePerformMotionBlur = "bf";

    class DragonframeDevice * dragonframeDevice = nullptr;
};

#endif
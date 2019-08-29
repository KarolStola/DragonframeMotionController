#ifndef DRAGONFRAME_MOTION_CONTROLLER_H
#define DRAGONFRAME_MOTION_CONTROLLER_H

#include <Arduino.h>

class DragonframeMotionController
{
public:
    void Initialize(class DragonframeDevice * newDragonframeDevice);
    void ParseInput(const String & input);

    virtual ~DragonframeMotionController();

private:
    void SendMessage(String & message);

    bool IsMessageHi(String & message);
    bool IsMessageQueryAreMotorsMoving(String & message);

    void HandleMessageHi(String & message);
    void HandleMessageQueryAreMotorsMoving(String & message);

    void ParseMessage(String & message);
    bool IsEndOfMessage(const String & message, int characterIndex);
    bool IsOmittedCharacter(char character);

    const char * messageHi = "hi";
    const char * messageQueryAreMotorsMoving = "ms";
    const char * messageMoveMotorTo = "mm";
    const char * messageQueryMotorPosition = "mp";
    const char * messageStopMotor = "sm";
    const char * messageStopAllMotors = "sa";
    const char * messageJogMotor = "jm";
    const char * messageInchMotor = "im";
    const char * messageSetMaxStepsPerSecond = "pr";
    const char * messageZeroMotorPosition = "zm";
    const char * messageSetMotorPosition = "np";
    const char * messagePerformMotionBlur = "bf";
    const char * messageEnding = "\r\n";

    class DragonframeDevice * dragonframeDevice = nullptr;
};

#endif
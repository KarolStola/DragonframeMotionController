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
    bool IsMessageMoveMotorTo(String & message);

    void SendHi();
    void SendMotorMovingStatuses();
    void SendCurrentPosition(int motor, int currentPosition);
    void SendMotorMovingTo(int motor, int requestedPosition);

    void HandleMessageMoveMotorTo(String & message);


    void ParseMessage(String & message);
    bool IsEndOfMessage(const String & message, int characterIndex);
    bool IsOmittedCharacter(char character);
    int GetNumberOfDigitsIn(int number);
    std::vector<int> GetArguments(String & message);

    static constexpr char * messageHi = "hi";
    static constexpr char * messageQueryAreMotorsMoving = "ms";
    static constexpr char * messageMoveMotorTo = "mm";
    static constexpr char * messageQueryMotorPosition = "mp";
    static constexpr char * messageStopMotor = "sm";
    static constexpr char * messageStopAllMotors = "sa";
    static constexpr char * messageJogMotor = "jm";
    static constexpr char * messageInchMotor = "im";
    static constexpr char * messageSetMaxStepsPerSecond = "pr";
    static constexpr char * messageZeroMotorPosition = "zm";
    static constexpr char * messageSetMotorPosition = "np";
    static constexpr char * messagePerformMotionBlur = "bf";
    static constexpr char * messageEnding = "\r\n";
    static constexpr int argumentsStartIndex = 3;

    class DragonframeDevice * dragonframeDevice = nullptr;
};

#endif
#ifndef DRAGONFRAME_MOTION_CONTROLLER_H
#define DRAGONFRAME_MOTION_CONTROLLER_H

#include <Arduino.h>
#include <DelayedMemberTask.h>

class DragonframeMotionController
{
public:
    DragonframeMotionController(class DragonframeDevice & dragonframeDevice);
    void ParseInput(const String & input);
    void Update();
    char GetIncomingMessageDelimiter();

private:
    void SendMessage(String & message);

    bool IsMessageHi(String & message);
    bool IsMessageQueryAreMotorsMoving(String & message);
    bool IsMessageMoveMotorTo(String & message);
    bool IsMessageQueryMotorPosition(String & message);
    bool IsMessageStopMotor(String & message);
    bool IsMessageStopAllMotors(String & message);
    bool IsMessageJogMotor(String & message);
    bool IsMessageInchMotor(String & message);
    bool IsMessageSetJogSpeed(String & message);
    bool IsMessageZeroMotorPosition(String & message);
    bool IsMessageSetMotorPosition(String & message);

    void SendHi();
    void SendMotorMovingStatuses();
    void SendCurrentPosition(int motor, int currentPosition);
    void SendMotorMovingTo(int motor, int requestedPosition);
    void SendStopMotorResponse(int motor);
    void SendStopAllMotorsResponse();
    void SendJogMotorResponse(int motor);
    void SendInchMotorResponse(int motor);
    void SendSetJogSpeedResponse(int motor, int stepsPerSecond);
    void SendZeroMotorPositionResponse(int motor);
    void SendSetMotorPositionResponse(int motor, int position);
    void SendMovementPositionUpdates();

    void HandleMessageMoveMotorTo(String & message);
    void HandleMessageQueryMotorPosition(String & message);
    void HandleMessageStopMotor(String & message);
    void HandleMessageStopAllMotors();
    void HandleMessageJogMotor(String & message);
    void HandleMessageInchMotor(String & message);
    void HandleMessageSetJogSpeed(String & message);
    void HandleMessageZeroMotorPosition(String & message);
    void HandleMessageSetMotorPosition(String & message);

    bool IsValidMotor(int motorIndex);
    void ParseMessage(String & message);
    bool IsEndOfMessage(const String & message, int characterIndex);
    bool IsEndingCharacter(char character);
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
    static constexpr char * messageSetJogSpeed = "pr";
    static constexpr char * messageZeroMotorPosition = "zm";
    static constexpr char * messageSetMotorPosition = "np";
    static constexpr char * messagePerformMotionBlur = "bf";
    static constexpr char * messageEnding = "\r\n";
    static constexpr char incomingMessageDelimiter = '\n';
    static constexpr int argumentsStartIndex = 3;

    int movementPositionUpdateInterval = 50;
    class DragonframeDevice & dragonframeDevice;
    DelayedMemberTask<DragonframeMotionController> movementPositionUpdateTask;
};

#endif
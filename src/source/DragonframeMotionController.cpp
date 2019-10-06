#define DEBUG_LOG 0

#include "DragonframeMotionController.h"
#include "DragonframeDevice.h"

DragonframeMotionController::DragonframeMotionController(DragonframeDevice & dragonframeDevice)
    : dragonframeDevice(dragonframeDevice)
    , movementPositionUpdateTask(DelayedTaskTimeResolution::Milliseconds, this, & DragonframeMotionController::SendMovementPositionUpdates)
{
    dragonframeDevice.BindAsMessageHandler(*this);
    movementPositionUpdateTask.Delay(movementPositionUpdateInterval);
}

void DragonframeMotionController::ParseInput(const String & input)
{
    String message;
    
    for(int i = 0; i < input.length(); i++)
    {
        if (IsEndingCharacter(input[i]))
        {
            ParseMessage(message);
            message = "";
        }
        else
        {
            message += input[i];
        }
    }

}

void DragonframeMotionController::Update()
{
    movementPositionUpdateTask.Update();
}

char DragonframeMotionController::GetIncomingMessageDelimiter()
{
    return incomingMessageDelimiter;
}

bool DragonframeMotionController::IsEndingCharacter(char character)
{
    for(int i = 0; messageEnding[i] != '\0' ; i++)
    {
        if(character == messageEnding[i])
        {
            return true;
        }
    }
    return false;
}

void DragonframeMotionController::SendMessage(String & message)
{
#if DEBUG_LOG
    Serial.print("RES: ");
    Serial.println(message);
#endif
    dragonframeDevice.SendMessage(message);
}


void DragonframeMotionController::ParseMessage(String & message)
{
    if(message.length() < 2)
    {
        return;
    }

#if DEBUG_LOG
    Serial.print("MSG: ");
    Serial.println(message);
#endif

    if(IsMessageHi(message))
    {
        SendHi();
    }
    else if (IsMessageQueryAreMotorsMoving(message))
    {
        SendMotorMovingStatuses();
    }
    else if(IsMessageMoveMotorTo(message))
    {
        HandleMessageMoveMotorTo(message);
    }
    else if(IsMessageQueryMotorPosition(message))
    {
        HandleMessageQueryMotorPosition(message);
    }
    else if(IsMessageStopMotor(message))
    {
        HandleMessageStopMotor(message);
    }
    else if(IsMessageStopAllMotors(message))
    {
        HandleMessageStopAllMotors();
    }
    else if(IsMessageSetJogSpeed(message))
    {
        HandleMessageSetJogSpeed(message);
    }
    else if(IsMessageJogMotor(message))
    {
        HandleMessageJogMotor(message);
    }
    else if(IsMessageInchMotor(message))
    {
        HandleMessageInchMotor(message);
    }
    else if(IsMessageZeroMotorPosition(message))
    {
        HandleMessageZeroMotorPosition(message);
    }
    else if(IsMessageSetMotorPosition(message))
    {
        HandleMessageSetMotorPosition(message);
    }
}

bool DragonframeMotionController::IsMessageHi(String & message)
{
    return message.startsWith(messageHi);
}

bool DragonframeMotionController::IsMessageQueryAreMotorsMoving(String & message)
{
    return message.startsWith(messageQueryAreMotorsMoving);
}

bool DragonframeMotionController::IsMessageMoveMotorTo(String & message)
{
    return message.startsWith(messageMoveMotorTo);
}

bool DragonframeMotionController::IsMessageQueryMotorPosition(String & message)
{
    return message.startsWith(messageQueryMotorPosition);
}

bool DragonframeMotionController::IsMessageStopMotor(String & message)
{
    return message.startsWith(messageStopMotor);
}

bool DragonframeMotionController::IsMessageStopAllMotors(String & message)
{
    return message.startsWith(messageStopAllMotors);
}

bool DragonframeMotionController::IsMessageJogMotor(String & message)
{
    return message.startsWith(messageJogMotor);
}

bool DragonframeMotionController::IsMessageInchMotor(String & message)
{
    return message.startsWith(messageInchMotor);
}

bool DragonframeMotionController::IsMessageSetJogSpeed(String & message)
{
    return message.startsWith(messageSetJogSpeed);
}

bool DragonframeMotionController::IsMessageZeroMotorPosition(String & message)
{
    return message.startsWith(messageZeroMotorPosition);
}

bool DragonframeMotionController::IsMessageSetMotorPosition(String & message)
{
    return message.startsWith(messageSetMotorPosition);
}

void DragonframeMotionController::SendHi()
{
    static String message =
        String(messageHi)
        + " " + dragonframeDevice.GetProtocolMajorVersion()
        + " " + dragonframeDevice.GetNumberOfAxes()
        + " " + dragonframeDevice.GetProtocolFullVersion()
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendMotorMovingStatuses()
{
    auto reply = String(messageQueryAreMotorsMoving) + ' ';

    for(int i = 1; i <= dragonframeDevice.GetNumberOfAxes(); i++)
    {
        reply += dragonframeDevice.GetIsMotorMoving(i) ? '1' : '0';
    }

    reply += messageEnding;

    SendMessage(reply);
}

void DragonframeMotionController::HandleMessageMoveMotorTo(String & message)
{
    auto arguments = GetArguments(message);
    
    if(arguments.size() < 2)
    {
        return;
    }

    auto motor = arguments[0];
    auto requestedPosition = arguments[1];

    if(!IsValidMotor(motor))
    {
        return;
    }

    auto currentPosition = dragonframeDevice.GetCurrentStep(motor);

    if(currentPosition == requestedPosition)
    {
        SendCurrentPosition(motor, currentPosition);
    }
    else
    {
        dragonframeDevice.MoveMotorTo(motor, requestedPosition);
        SendMotorMovingTo(motor, requestedPosition);
    }
}

void DragonframeMotionController::HandleMessageQueryMotorPosition(String & message)
{
    auto arguments = GetArguments(message);

    if(arguments.size() < 1)
    {
        return;
    }

    auto motor = arguments[0];

    if(!IsValidMotor(motor))
    {
        return;
    }

    auto motorPosition = dragonframeDevice.GetCurrentStep(motor);
    SendCurrentPosition(motor, motorPosition);
}

void DragonframeMotionController::HandleMessageStopMotor(String & message)
{
    auto arguments = GetArguments(message);

    if(arguments.size() < 1)
    {
        return;
    }

    auto motor = arguments[0];

    if(!IsValidMotor(motor))
    {
        return;
    }

    dragonframeDevice.StopMotor(motor);
    SendStopMotorResponse(motor);
}

void DragonframeMotionController::HandleMessageStopAllMotors()
{
    dragonframeDevice.StopAllMotors();
    SendStopAllMotorsResponse();
}

void DragonframeMotionController::HandleMessageJogMotor(String & message)
{
    auto arguments = GetArguments(message);

    if(arguments.size() < 2)
    {
        return;
    }

    auto motor = arguments[0];
    auto position = arguments[1];
    
    if(!IsValidMotor(motor))
    {
        return;
    }

    dragonframeDevice.JogMotorTo(motor, position);
    SendJogMotorResponse(motor);
}

void DragonframeMotionController::HandleMessageInchMotor(String & message)
{
    auto arguments = GetArguments(message);

    if(arguments.size() < 2)
    {
        return;
    }

    auto motor = arguments[0];
    auto position = arguments[1];
    
    if(!IsValidMotor(motor))
    {
        return;
    }

    dragonframeDevice.InchMotorTo(motor, position);
    SendInchMotorResponse(motor);
}

void DragonframeMotionController::HandleMessageSetJogSpeed(String & message)
{
    auto arguments = GetArguments(message);

    if(arguments.size() < 2)
    {
        return;
    }

    auto motor = arguments[0];
    auto stepsPerSecond = arguments[1];
    
    if(!IsValidMotor(motor))
    {
        return;
    }

    dragonframeDevice.SetJogSpeedForMotor(motor, stepsPerSecond);
    SendSetJogSpeedResponse(motor, stepsPerSecond);
}

void DragonframeMotionController::HandleMessageZeroMotorPosition(String & message)
{
    auto arguments = GetArguments(message);

    if(arguments.size() < 1)
    {
        return;
    }

    auto motor = arguments[0];
    
    if(!IsValidMotor(motor))
    {
        return;
    }

    dragonframeDevice.ZeroMotorPosition(motor);
    SendZeroMotorPositionResponse(motor);
}

void DragonframeMotionController::HandleMessageSetMotorPosition(String & message)
{
    auto arguments = GetArguments(message);

    if(arguments.size() < 2)
    {
        return;
    }

    auto motor = arguments[0];
    auto position = arguments[1];
    
    if(!IsValidMotor(motor))
    {
        return;
    }
    
    dragonframeDevice.SetMotorPosition(motor, position);
    SendSetMotorPositionResponse(motor, position);
}

void DragonframeMotionController::SendCurrentPosition(int motor, int currentPosition)
{
    auto message
        = String(messageQueryMotorPosition)
        + " " + motor
        + " " + currentPosition
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendMotorMovingTo(int motor, int requestedPosition)
{
    auto message
        = String(messageMoveMotorTo)
        + " " + motor
        + " " + requestedPosition
        + messageEnding;
    
    SendMessage(message);
}

void DragonframeMotionController::SendStopMotorResponse(int motor)
{
    auto message
        = String(messageStopMotor)
        + " " + motor
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendStopAllMotorsResponse()
{
    auto message
        = String(messageStopAllMotors)
        + messageEnding;
    
    SendMessage(message);
}

void DragonframeMotionController::SendJogMotorResponse(int motor)
{
    auto message
        = String(messageJogMotor)
        + " " + motor;
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendInchMotorResponse(int motor)
{
    auto message
        = String(messageInchMotor)
        + " " + motor
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendSetJogSpeedResponse(int motor, int stepsPerSecond)
{
    auto message
        = String(messageSetJogSpeed)
        + " " + motor
        + " " + stepsPerSecond
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendZeroMotorPositionResponse(int motor)
{
    auto message =
        String(messageZeroMotorPosition)
        + " " + motor
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendSetMotorPositionResponse(int motor, int position)
{
    auto message =
        String(messageSetMotorPosition)
        + " " + motor
        + " " + position
        + messageEnding;
    
    SendMessage(message);
}

bool DragonframeMotionController::IsValidMotor(int motorIndex)
{
    return motorIndex <= dragonframeDevice.GetNumberOfAxes();
}


void DragonframeMotionController::SendMovementPositionUpdates()
{
    for(int i = 1; i <= dragonframeDevice.GetNumberOfAxes(); i++)
    {
        if(dragonframeDevice.GetIsMotorMoving(i))
        {
            SendCurrentPosition(i, dragonframeDevice.GetCurrentStep(i));
        }
    }
    movementPositionUpdateTask.Delay(movementPositionUpdateInterval);
}


std::vector<int> DragonframeMotionController::GetArguments(String & message)
{
    std::vector<int> arguments;
    auto currentIndex = argumentsStartIndex;

    while(currentIndex < message.length())
    {
        auto currentArgument = message.substring(currentIndex).toInt();
        auto argumentDigits = GetNumberOfDigitsIn(currentArgument);
        currentIndex += argumentDigits + 1;
        arguments.push_back(currentArgument);
    }

    return arguments;
}


int DragonframeMotionController::GetNumberOfDigitsIn(int number)
{
    int numberOfDigits = 0;
    while(number > 0)
    {
        number /= 10;
        numberOfDigits++;
    }
    return numberOfDigits;
}

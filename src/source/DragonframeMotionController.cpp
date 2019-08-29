#include "DragonframeMotionController.h"
#include "DragonframeDevice.h"

void DragonframeMotionController::Initialize(DragonframeDevice * newDragonframeDevice)
{
    dragonframeDevice = newDragonframeDevice;
    dragonframeDevice->BindAsMessageHandler(*this);
}

void DragonframeMotionController::ParseInput(const String & input)
{
    String message;
    
    for(int i = 0; i < input.length(); i++)
    {
        if(IsEndOfMessage(input, i))
        {
            ParseMessage(message);
            message = "";
        }
        else if (!IsOmittedCharacter(input[i]))
        {
            message += input[i];
        }    
    }
}

bool DragonframeMotionController::IsEndOfMessage(const String & message, int characterIndex)
{
    return message[characterIndex] == '\n' && characterIndex > 0 && message[characterIndex-1] == '\r';
}

bool DragonframeMotionController::IsOmittedCharacter(char character)
{
    return character == '\r';
}

void DragonframeMotionController::SendMessage(String & message)
{
    Serial.println(message);
    dragonframeDevice->SendMessage(message);
}


void DragonframeMotionController::ParseMessage(String & message)
{
    if(message.length() < 2)
    {
        return;
    }

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
    message.startsWith(messageMoveMotorTo);
}

void DragonframeMotionController::SendHi()
{
    static String message =
        String(messageHi)
        + " " + dragonframeDevice->GetProtocolMajorVersion()
        + " " + dragonframeDevice->GetNumberOfAxes()
        + " " + dragonframeDevice->GetProtocolFullVersion()
        + messageEnding;

    SendMessage(message);
}

void DragonframeMotionController::SendMotorMovingStatuses()
{
    auto reply = String(messageQueryAreMotorsMoving) + ' ';

    for(int i = 0; i < dragonframeDevice->GetNumberOfAxes(); i++)
    {
        reply += dragonframeDevice->GetIsMotorMoving(i) ? '1' : '0';
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
    auto currentPosition = dragonframeDevice->GetMotorCurrentPositionInSteps(motor);

    if(currentPosition == requestedPosition)
    {
        SendCurrentPosition(motor, currentPosition);
    }
    else
    {
        dragonframeDevice->MoveMotorTo(motor, requestedPosition);
        SendMotorMovingTo(motor, requestedPosition);
    }
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


DragonframeMotionController::~DragonframeMotionController()
{
    delete(dragonframeDevice);
}

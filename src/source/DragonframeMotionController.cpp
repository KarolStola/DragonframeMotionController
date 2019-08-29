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
    if(IsMessageHi(message))
    {
        HandleMessageHi(message);
    }
    else if (IsMessageQueryAreMotorsMoving(message))
    {
        HandleMessageQueryAreMotorsMoving(message);
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

void DragonframeMotionController::HandleMessageHi(String & message)
{
    static String reply =
        String(messageHi)
        + " " + dragonframeDevice->GetProtocolMajorVersion()
        + " " + dragonframeDevice->GetNumberOfAxes()
        + " " + dragonframeDevice->GetProtocolFullVersion()
        + messageEnding;

    SendMessage(reply);
}

void DragonframeMotionController::HandleMessageQueryAreMotorsMoving(String & message)
{
    auto reply = String(messageQueryAreMotorsMoving) + ' ';

    for(int i = 0; i < dragonframeDevice->GetNumberOfAxes(); i++)
    {
        reply += dragonframeDevice->GetIsMotorMoving(i) ? '1' : '0';
    }

    reply += messageEnding;

    SendMessage(reply);
}

DragonframeMotionController::~DragonframeMotionController()
{
    delete(dragonframeDevice);
}

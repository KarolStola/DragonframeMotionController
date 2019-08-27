#include "DragonframeMotionController.h"
#include "DragonframeDevice.h"

void DragonframeMotionController::Initialize(DragonframeDevice * newDragonframeDevice)
{
    dragonframeDevice = newDragonframeDevice;
    dragonframeDevice->BindAsMessageHandler(*this);
}

void DragonframeMotionController::HandleMessage(const String & message)
{
    String currentMessage;
    
    for(int i = 0; i < message.length(); i++)
    {
        if(message[i] == '\n' && i > 0 && message[i-1] == '\r')
        {
            ParseMessage(currentMessage);
            currentMessage = "";
        }
        else if (message[i] != '\r')
        {
            currentMessage += message[i];
        }    
    }
}

void DragonframeMotionController::ParseMessage(String & message)
{
    if(IsMessageHi(message))
    {
        auto replyHi =
            String(messageHi)
            + " " + dragonframeDevice->GetMajorVersion()
            + " " + dragonframeDevice->GetNumberOfAxes()
            + " " + dragonframeDevice->GetFullVersion()
            + "\r\n" ;
        dragonframeDevice->SendMessage(replyHi);
    }
}

bool DragonframeMotionController::IsMessageHi(String & message)
{
    return message.startsWith(messageHi);
}


DragonframeMotionController::~DragonframeMotionController()
{
    delete(dragonframeDevice);
}

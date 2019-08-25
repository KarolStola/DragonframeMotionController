#include "DragonframeMotionController.h"
#include "DragonframeDevice.h"

DragonframeMotionController::DragonframeMotionController(DragonframeDevice * dragonframeDevice)
    : dragonframeDevice(dragonframeDevice)
{
    dragonframeDevice->BindAsMessageHandler(this);
}


void DragonframeMotionController::HandleMessage(const String & message)
{
    
    if(IsMessageHi(message))
    {
        Serial.println(message.c_str());

        auto replyHi =
            messageHi
            + " " + dragonframeDevice->GetMajorVersion()
            + " " + dragonframeDevice->GetNumberOfAxes()
            + " " + dragonframeDevice->GetFullVersion()
            + "\r\n" ;
        dragonframeDevice->SendMessage(replyHi);
    }
}

bool DragonframeMotionController::IsMessageHi(const String & message)
{
    //return message.startsWith(messageHi) == '1';
}

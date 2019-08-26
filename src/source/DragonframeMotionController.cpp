#include "DragonframeMotionController.h"
#include "DragonframeDevice.h"

DragonframeMotionController::DragonframeMotionController(DragonframeDevice * dragonframeDevice)
    : dragonframeDevice(dragonframeDevice)
{
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
        Serial.println("fdsafadsf");
        /*auto replyHi =
            messageHi
            + " " + dragonframeDevice->GetMajorVersion()
            + " " + dragonframeDevice->GetNumberOfAxes()
            + " " + dragonframeDevice->GetFullVersion()
            + "\r\n" ;
        dragonframeDevice->SendMessage(replyHi);*/
    }
}

bool DragonframeMotionController::IsMessageHi(String & message)
{
    Serial.print(message);
    auto test = message.startsWith(messageHi);
    Serial.println(test);
    //return message[0] == messageHi[0] && message[1] == messageHi[1];
    return message.startsWith(messageHi);
}

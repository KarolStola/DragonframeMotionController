#ifndef DRAGONFRAME_DEVICE_H
#define DRAGONFRAME_DEVICE_H

#include <Arduino.h>

class DragonframeDevice
{
public:
    virtual int GetMajorVersion() = 0;
    virtual const String & GetFullVersion() = 0;
    virtual int GetNumberOfAxes() = 0;
    virtual void SendMessage(const String & message) = 0;
    virtual void BindAsMessageHandler(class DragonframeMotionController * motionController) = 0;
};

#endif

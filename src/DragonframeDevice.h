#ifndef DRAGONFRAME_DEVICE_H
#define DRAGONFRAME_DEVICE_H

#include <Arduino.h>

class DragonframeDevice
{
public:
    virtual const char * GetMajorVersion() = 0;
    virtual const char * GetFullVersion() = 0;
    virtual const char * GetNumberOfAxes() = 0;
    virtual void SendMessage(const String & message) = 0;
    virtual void BindAsMessageHandler(class DragonframeMotionController & motionController) = 0;
};

#endif

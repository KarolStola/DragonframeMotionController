#ifndef DRAGONFRAME_MOTION_CONTROLLER_H
#define DRAGONFRAME_MOTION_CONTROLLER_H

#include <Arduino.h>

class DragonframeMotionController
{
public:
    void Initialize(class DragonframeDevice * newDragonframeDevice);
    void HandleMessage(const String & message);
    virtual ~DragonframeMotionController();

private:
    bool IsMessageHi(String & message);
    void ParseMessage(String & message);

    const char * messageHi = "hi";

    class DragonframeDevice * dragonframeDevice = nullptr;
};

#endif
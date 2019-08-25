#ifndef DRAGONFRAME_MOTION_CONTROLLER_H
#define DRAGONFRAME_MOTION_CONTROLLER_H

#include <Arduino.h>

class DragonframeMotionController
{
public:
    DragonframeMotionController(class DragonframeDevice * dragonframeDevice);
    void HandleMessage(const String & message);

private:
    bool IsMessageHi(const String & message);

    const String messageHi = "hi";

    class DragonframeDevice * dragonframeDevice = nullptr;
};

#endif
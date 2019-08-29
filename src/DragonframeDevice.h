#ifndef DRAGONFRAME_DEVICE_H
#define DRAGONFRAME_DEVICE_H

#include <Arduino.h>

class DragonframeDevice
{
public:
    const char * GetProtocolFullVersion();

    virtual void BindAsMessageHandler(class DragonframeMotionController & motionController) = 0;
    virtual int GetProtocolMajorVersion() = 0;  // First number in version number   X.0.0
    virtual int GetProtocolMinorVersion() = 0;  // Second number in version number  0.X.0
    virtual int GetProtocolFixVersion() = 0;    // Third number in version number   0.0.X
    virtual int GetNumberOfAxes() = 0;
    virtual int GetMotorCurrentPositionInSteps(int motorIndex) = 0;
    virtual bool GetIsMotorMoving(int motorIndex) = 0;


    virtual void MoveMotorTo(int motorIndex, int stepPosition) = 0;
    virtual void JogMotorTo(int motorIndex, int stepPosition) = 0;
    virtual void InchMotorTo(int motorIndex, int stepPosition) = 0;
    virtual void StopMotor(int motorIndex) = 0;
    virtual void StopAllMotors() = 0;
    virtual void SetJogSpeedForMotor(int motorIndex, int stepsPerSecond) = 0;
    virtual void ZeroMotorPosition(int motorIndex) = 0;
    virtual void SetMotorPosition(int motorIndex, int motorPosition) = 0;

    virtual void SendMessage(const String & message) = 0;
};

#endif

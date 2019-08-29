#include "DragonframeDevice.h"

const char * DragonframeDevice::GetProtocolFullVersion()
{
    static String fullVersion = String() + GetProtocolMajorVersion() + "." + GetProtocolMinorVersion() + "." + GetProtocolFixVersion();
    return fullVersion.c_str();
}
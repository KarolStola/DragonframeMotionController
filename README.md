# Dragonframe Motion Controller
Arduino library for controlling generic Dragonframe Motion Controllers.

Please note, that the implementation currently does not support the motion blur handling. Feel free to submit the merge or feature request if you'd like it to be added.

## Instalation
1. Install prerequisite library: https://github.com/KarolStola/DelayedTasks
2. Clone the repository into your sketchbook location. The path can be set/viewed in File => Preferences => Settings tab.

## Usage
1. Implement the interface of ```DragonframeDevice```.
2. Create static instance of ```DragonframeDevice```.
3. Create static Instance of ```DrafonframeMotionController``` and pass the implemented ```DragonframeDevice``` as an argument to the constructor.
4. Call the ```DragonframeMotionController::Update()``` function in the ```void loop()```.

### Minimal Example
```
#include <Arduino.h>
#include <DragonframeMotionController.h>
#include "MyDragonframeDevice.h"

MyDragonframeDevice myDevice;
DragonframeMotionController motionController(myDevice);

void setup()
{
}

void loop()
{
    motionController.Update();
}

```

For a complete example of implementation, please refer to https://github.com/KarolStola/DragonframeWithOpti

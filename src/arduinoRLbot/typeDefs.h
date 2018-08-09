#ifndef RewardsForActionsAfterObservingAState_h
#define RewardsForActionsAfterObservingAState_h

struct RewardsForActionsAfterObservingAState
{
    int ff; //forward forward
    int fb;
    int bf;
    int bb; //backward backward
};

#endif

#ifndef MemoryOfRewardsForStateActionPairs_h
#define MemoryOfRewardsForStateActionPairs_h

//Including a hashmap type is too expensive, so this will have to do
//Not sure if a linear search will be performance issue
struct MemoryOfRewardsForStateActionPairs
{
    String key[8];
    RewardsForActionsAfterObservingAState value[8];
};

#endif

#ifndef State_h
#define State_h

struct State
{
    char sensorLeft;
    char sensorMiddle;
    char sensorRight;
    char memoryKey[3];
};

#endif

#ifndef Action_h
#define Action_h

struct Action
{
    int leftWheelDirection;
    int rightWheelDirection;
};

#endif

#ifndef StateActionPair_h
#define StateActionPair_h

struct StateActionPair
{
    State state;
    Action action;
};

#endif

#ifndef Sensor_h
#define Sensor_h

struct Sensor
{
    int s0Pin;
    int s1Pin;
    int s2Pin;
    int s3Pin;
    int outputPin;
};

#endif

#ifndef ServoInstructions_h
#define ServoInstructions_h

struct ServoInstructions
{
    int signalPin;
    int signalToSet;
};

#endif

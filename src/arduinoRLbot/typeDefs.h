struct RewardsForActionsAfterObservingAState
{
    int ff; //forward forward
    int fb;
    int bf;
    int bb; //backward backward
};

//Including a hashmap type is too expensive, so this will have to do
//Not sure if a linear search will be performance issue
struct MemoryOfRewardsForStateActionPairs
{
    String key[8];
    RewardsForActionsAfterObservingAState value[8];
};

struct State
{
    char sensorLeft;
    char sensorMiddle;
    char sensorRight;
    char memoryKey[3];
};

struct Action
{
    int leftWheelDirection;
    int rightWheelDirection;
};

struct StateActionPair
{
    State state;
    Action action;
};

struct Sensor
{
    int s0Pin;
    int s1Pin;
    int s2Pin;
    int s3Pin;
    int outputPin;
};

struct ServoInstructions
{
    int signalPin;
    int signalToSet;
};

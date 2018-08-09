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

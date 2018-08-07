#include "LinkedList.h"

/*
 * In this reinforcement learning scenario, the software agent will: 
 *    a) make an observation of the environment state, or of the robot's state in the environment
 *    b) if that state is a fail, and if it is, pop back through list of state-action pairs and give negative rewards. Go back to a)
 *    c) else, give a positive reward for the last state-action pair in list
 *    d) check to see which action has the best reward for the observed state previously
 *    e) complete that action
 *    f) add the state-action pair to the list
 *    REPEAT
 * 
 * It would be preferable to use a HashMap, but, the limitations of an Arduino mean using a fixed length array is more achieveable. However, this means extra care must be paid to naming things, so noone gets confused.
 * Also it would be great to write a minimal unit test suite.
 * 
 * To mitigate against the robot just learning to go backwards and forwards, the robot should detect if it moves a total of zero revs in 10 turns, if so, negative rewards for recorded moves
 * Alternatively, you might use a button to give it a negative reward. 
 */

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

//This belongs with the other IO stuff, probably in a separate file. Learn how to use function pointers for callbacks.


char CategoriseFrequencyTrio(
    int redFrequency,
    int greenFrequency,
    int blueFrequency)
{
    if (redFrequency < 30 &&
        greenFrequency < 30 &&
        blueFrequency < 30)
    {
        return 'b';
    }
    else if (redFrequency < 30 &&
        greenFrequency > 150 &&
        blueFrequency > 150)
    {
        return 'r';
    }
    return 'w';
}

int GetFrequency(Sensor sensor, bool s2_signal, bool s3_signal)
{
    digitalWrite(sensor.s2Pin, s2_signal);
    digitalWrite(sensor.s3Pin, s3_signal);
    return pulseIn(sensor.outputPin, LOW);
}

char GetColour(Sensor sensor)
{
    int redFrequency = GetFrequency(sensor, false, false); //LOW, LOW
    int greenFrequency = GetFrequency(sensor, true, true); //HIGH, HIGH
    int blueFrequency = GetFrequency(sensor, false, true); //LOW, HIGH
    return CategoriseFrequencyTrio(redFrequency, greenFrequency, blueFrequency);
}

State ObserveState(
    Sensor sensorLeft,
    Sensor sensorMiddle,
    Sensor sensorRight)
{
    State state;
    state.sensorLeft = GetColour(sensorLeft);
    state.sensorMiddle = GetColour(sensorMiddle);
    state.sensorRight = GetColour(sensorRight);

    state.memoryKey[0] = state.sensorLeft;
    state.memoryKey[1] = state.sensorMiddle;
    state.memoryKey[2] = state.sensorRight;

    return state;
}

bool CheckHasObservedRed(State state)
{
    if (state.sensorLeft == 'r' || 
        state.sensorMiddle == 'r' ||
        state.sensorRight == 'r')
    {
        return true;
    }
    return false;
}

RewardsForActionsAfterObservingAState RecallRewardsForActionsAfterObservingAState(State state, MemoryOfRewardsForStateActionPairs memoryOfRewardsForStateActionPairs)
{
    for (int i = 0; i < 8; i++)
    {
        if(memoryOfRewardsForStateActionPairs.key[i] == state.memoryKey)
        {
            return memoryOfRewardsForStateActionPairs.value[i];
        }
    }
}

Action DecideNextAction(RewardsForActionsAfterObservingAState rewardsForActionsAfterObservingAState)
{
    Action nextAction;
    nextAction.leftWheelDirection = 1300; //AntiClockwise
    nextAction.rightWheelDirection = 1700; //Clockwise
    int highestValue = rewardsForActionsAfterObservingAState.ff;

    if (rewardsForActionsAfterObservingAState.fb > highestValue)
    {
        nextAction.leftWheelDirection = 1300; //Clockwise
        nextAction.rightWheelDirection = 1300; //Clockwise
        highestValue = rewardsForActionsAfterObservingAState.fb;
    }
    if (rewardsForActionsAfterObservingAState.bf > highestValue)
    {
        nextAction.leftWheelDirection = 1700; //AntiClockwise
        nextAction.rightWheelDirection = 1700; //AntiClockwise
        highestValue = rewardsForActionsAfterObservingAState.bf;
    }
    if (rewardsForActionsAfterObservingAState.bb > highestValue)
    {
        nextAction.leftWheelDirection = 1700; //Clockwise
        nextAction.rightWheelDirection = 1300; //AntiClockwise
    }
    return nextAction;
}

void ReverseRecentStateActionPairsAndApplyNegativeRewards(LinkedList<StateActionPair> mostRecentStateActionPairs)
{
    while(true)
    {
        if (mostRecentStateActionPairs.size() == 0)
        {
            return;
        }
        StateActionPair recentStateActionPair;
        recentStateActionPair = mostRecentStateActionPairs.pop();
        //ReverseAction(recentStateActionPair.Action);
        //NegativeRewardForStateActionPair(recentStateActionPair);
    }
}

MemoryOfRewardsForStateActionPairs memoryOfRewardsForStateActionPairs;

LinkedList<StateActionPair> mostRecentStateActionPairs;

Sensor sensorLeft;
Sensor sensorMiddle;
Sensor sensorRight;

//IO LOGIC

void ConfigureSensor(Sensor *sensor)
{
    pinMode(sensor->s0Pin, OUTPUT);
    pinMode(sensor->s1Pin, OUTPUT);
    pinMode(sensor->s2Pin, OUTPUT);
    pinMode(sensor->s3Pin, OUTPUT);
    pinMode(sensor->outputPin, INPUT);
    digitalWrite(sensor->s0Pin, HIGH);
    digitalWrite(sensor->s1Pin, LOW);
}

void setup()
{   
    sensorLeft = { 1, 2, 3, 4, 13 };
    sensorMiddle = { 5, 6, 7, 8, 14};
    sensorRight = { 9, 10, 11, 12, 15 };
    
    ConfigureSensor(&sensorLeft);
    ConfigureSensor(&sensorMiddle);
    ConfigureSensor(&sensorRight);

    mostRecentStateActionPairs = LinkedList<StateActionPair>();

    memoryOfRewardsForStateActionPairs = {
        { "www", "wwb", "wbw", "wbb", "bww", "bwb", "bbw", "bbb" },
        { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
    };

    //For debugging
    //Serial.begin(9600);
}

void loop()
{
    State state = ObserveState(sensorLeft, sensorMiddle, sensorRight);
    bool hasObservedRed = CheckHasObservedRed(state);
    if(hasObservedRed)
    {
        ReverseRecentStateActionPairsAndApplyNegativeRewards(mostRecentStateActionPairs);
    }
    else
    {
        //mark the last move added to the move stack as positive
    }
    RewardsForActionsAfterObservingAState rewardsForActionsAfterObservingAState = RecallRewardsForActionsAfterObservingAState(state, memoryOfRewardsForStateActionPairs);
    Action nextAction = DecideNextAction(rewardsForActionsAfterObservingAState);    
}


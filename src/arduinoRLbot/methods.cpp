#include <LinkedList.h>
#include "typeDefs.h"

char CategoriseFrequencyTrio(
    int redFrequency,
    int greenFrequency,
    int blueFrequency)
{
    if (redFrequency < 30,
        greenFrequency < 30,
        blueFrequency < 30)
    {
        return 'b';
    }
    else if (redFrequency < 30,
        greenFrequency > 150,
        blueFrequency > 150)
    {
        return 'r';
    }
    return 'w';
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
        StateActionPair recentStateActionPair = mostRecentStateActionPairs.pop();
        //ReverseAction(recentStateActionPair.Action);
        //NegativeRewardForStateActionPair(recentStateActionPair);
    }
}

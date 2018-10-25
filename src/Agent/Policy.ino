#include "LinkedList.h"
#include "Models.h"

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

char GetColour(SensorModel sensor)
{
    int redFrequency = GetFrequency(sensor, false, false); //LOW, LOW
    int greenFrequency = GetFrequency(sensor, true, true); //HIGH, HIGH
    int blueFrequency = GetFrequency(sensor, false, true); //LOW, HIGH
    return CategoriseFrequencyTrio(redFrequency, greenFrequency, blueFrequency);
}

State ObserveState(
    SensorModel sensorLeft,
    SensorModel sensorMiddle,
    SensorModel sensorRight)
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

RewardsForActionsAfterObservingAState RecallRewardsForActionsAfterObservingAState(
    State state,
    MemoryOfRewardsForStateActionPairs memoryOfRewardsForStateActionPairs)
{
    for (int i = 0; i < 8; i++)
    {
        if(memoryOfRewardsForStateActionPairs.key[i] == state.memoryKey)
        {
            return memoryOfRewardsForStateActionPairs.value[i];
        }
    }
    //This should never occur, and it might be better to have triggers for LEDs for these kinds of issues.
    return { 0, 0, 0, 0 };
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

int DetermineNegativeReward(int recentStateActionPairsCount)
{
    int reward = recentStateActionPairsCount - 2;
    if (reward < 0)
    {
        reward = 1;
    }
    return reward;
}

int ReverseDirection(int originalDirection)
{
    if (originalDirection == 1300)
    {
        return 1700;
    }
    if (originalDirection == 1700)
    {
        return 1300;
    }
    return originalDirection;
}

Action ReverseAction(Action originalAction)
{
    Action reversedAction;
    reversedAction.leftWheelDirection = ReverseDirection(originalAction.leftWheelDirection);
    reversedAction.rightWheelDirection = ReverseDirection(originalAction.rightWheelDirection);
    return reversedAction;
}

void ApplyAction(
    ServoModel servoLeft,
    ServoModel servoRight,
    Action action)
{
    TurnWheel(servoLeft, action.leftWheelDirection);
    TurnWheel(servoRight, action.rightWheelDirection);
}



void ReverseRecentStateActionPairsAndApplyNegativeRewards(
    LinkedList<StateActionPair> mostRecentStateActionPairs,
    ServoModel servoLeft,
    ServoModel servoRight)
{
    while(true)
    {
        int recentStateActionPairsCount = mostRecentStateActionPairs.size();
        if (recentStateActionPairsCount == 0)
        {
            return;
        }
        int negativeReward = DetermineNegativeReward(recentStateActionPairsCount);
        StateActionPair recentStateActionPair = mostRecentStateActionPairs.pop();
        Action reversedAction = ReverseAction(recentStateActionPair.action);
        ApplyAction(servoLeft, servoRight, reversedAction);
        //NegativeRewardForStateActionPair(recentStateActionPair);
    }
}

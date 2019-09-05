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

char GetColour(SensorModel *sensor)
{
    int redFrequency = GetFrequency(sensor, false, false); //LOW, LOW
    int greenFrequency = GetFrequency(sensor, true, true); //HIGH, HIGH
    int blueFrequency = GetFrequency(sensor, false, true); //LOW, HIGH
    return CategoriseFrequencyTrio(redFrequency, greenFrequency, blueFrequency);
}

State ObserveState(
    SensorModel *sensorLeft,
    SensorModel *sensorMiddle,
    SensorModel *sensorRight)
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

RewardsForActions RecallRewardsForActions(
    State state,
    MemoryOfRewardsForActions *memoryOfRewardsForActions)
{
    int numberOfRewardsForStates = 8; //sizeof(memoryOfRewardsForActions->states)
    for (int i = 0; i < numberOfRewardsForStates; i++)
    {
        if(memoryOfRewardsForActions->states[i] == state.memoryKey)
        {
            return memoryOfRewardsForActions->rewardsForActions[i];
        }
    }
}

Action DecideNextAction(RewardsForActions rewardsForActions)
{
    Action nextAction;
    nextAction.leftWheelDirection = 1300; //AntiClockwise
    nextAction.rightWheelDirection = 1700; //Clockwise
    nextAction.memoryKey[0] = 'f';
    nextAction.memoryKey[1] = 'f';
    int highestValue = rewardsForActions.ff;

    if (rewardsForActions.fb > highestValue)
    {
        nextAction.leftWheelDirection = 1300; //Clockwise
        nextAction.rightWheelDirection = 1300; //Clockwise
        nextAction.memoryKey[0] = 'f';
        nextAction.memoryKey[1] = 'b';
        highestValue = rewardsForActions.fb;
    }
    if (rewardsForActions.bf > highestValue)
    {
        nextAction.leftWheelDirection = 1700; //AntiClockwise
        nextAction.rightWheelDirection = 1700; //AntiClockwise
        nextAction.memoryKey[0] = 'b';
        nextAction.memoryKey[1] = 'f';
        highestValue = rewardsForActions.bf;
    }
    if (rewardsForActions.bb > highestValue)
    {
        nextAction.leftWheelDirection = 1700; //Clockwise
        nextAction.rightWheelDirection = 1300; //AntiClockwise
        nextAction.memoryKey[0] = 'b';
        nextAction.memoryKey[1] = 'b';
    }
    return nextAction;
}

int DetermineNegativeReward(int stateActionPairsYetToBeRewarded)
{
    int rewardBias = 1;
    int reward = 0 - stateActionPairsYetToBeRewarded + rewardBias;
    if (reward >= 0)
    {
        reward = -1;
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

void ApplyRewardForStateActionPair(
    StateActionPair stateActionPair,
    MemoryOfRewardsForActions *memoryOfRewardsForActions,
    int reward)
{
    int numberOfRewardsForStates = 8; //sizeof(memoryOfRewardsForActions->states)
    for (int i = 0; i < numberOfRewardsForStates; i++)
    {
        if(memoryOfRewardsForActions->states[i] == stateActionPair.state.memoryKey)
        {
            if (stateActionPair.action.memoryKey == "ff")
            {
                memoryOfRewardsForActions->rewardsForActions->ff = memoryOfRewardsForActions->rewardsForActions->ff + reward;
            }
            else if (stateActionPair.action.memoryKey == "fb")
            {
                memoryOfRewardsForActions->rewardsForActions->fb = memoryOfRewardsForActions->rewardsForActions->fb + reward;
            }
            else if (stateActionPair.action.memoryKey == "bf")
            {
                memoryOfRewardsForActions->rewardsForActions->bf = memoryOfRewardsForActions->rewardsForActions->bf + reward;
            }
            else if (stateActionPair.action.memoryKey == "bb")
            {
                memoryOfRewardsForActions->rewardsForActions->bb = memoryOfRewardsForActions->rewardsForActions->bb + reward;
            }
        }
    }
}

void NegativePolicy(
    LinkedList<StateActionPair> *mostRecentStateActionPairs,
    MemoryOfRewardsForActions *memoryOfRewardsForActions,
    ServoModel servoLeft,
    ServoModel servoRight)
{
    int recentStateActionPairsCount = mostRecentStateActionPairs->size();
    for (int i; i < recentStateActionPairsCount; i++)
    {
        int negativeReward = DetermineNegativeReward(mostRecentStateActionPairs->size());
        StateActionPair recentStateActionPair = mostRecentStateActionPairs->pop();
        Action reversedAction = ReverseAction(recentStateActionPair.action);
        ApplyAction(servoLeft, servoRight, reversedAction);
        ApplyRewardForStateActionPair(recentStateActionPair, memoryOfRewardsForActions, negativeReward);
    }
}

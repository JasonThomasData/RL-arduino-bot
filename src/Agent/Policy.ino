#include "LinkedList.h"

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
    State state = CreateState(
        GetColour(sensorLeft),
        GetColour(sensorMiddle),
        GetColour(sensorRight));
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
    Action nextAction = CreateAction_Forward();
    int highestValue = rewardsForActions.ff;

    if (rewardsForActions.bf > highestValue)
    {
        nextAction = CreateAction_TurnLeft();
        highestValue = rewardsForActions.bf;
    }
    if (rewardsForActions.fb > highestValue)
    {
        nextAction = CreateAction_TurnRight();
        highestValue = rewardsForActions.fb;
    }
    if (rewardsForActions.bb > highestValue)
    {
        nextAction = CreateAction_Backward();
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
                memoryOfRewardsForActions->rewardsForActions[i].ff = memoryOfRewardsForActions->rewardsForActions[i].ff + reward;
            }
            else if (stateActionPair.action.memoryKey == "bf")
            {
                memoryOfRewardsForActions->rewardsForActions[i].bf = memoryOfRewardsForActions->rewardsForActions[i].bf + reward;
            }
            else if (stateActionPair.action.memoryKey == "fb")
            {
                memoryOfRewardsForActions->rewardsForActions[i].fb = memoryOfRewardsForActions->rewardsForActions[i].fb + reward;
            }
            else if (stateActionPair.action.memoryKey == "bb")
            {
                memoryOfRewardsForActions->rewardsForActions->bb = memoryOfRewardsForActions->rewardsForActions[i].bb + reward;
            }
        }
    }
}

void FailStatePolicy(
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
        Action reversedAction = CreateReversedAction(recentStateActionPair.action);
        ApplyAction(servoLeft, servoRight, reversedAction);
        ApplyRewardForStateActionPair(recentStateActionPair, memoryOfRewardsForActions, negativeReward);
    }
}

void StandardPolicy(
    State state,
    LinkedList<StateActionPair> *mostRecentStateActionPairs,
    MemoryOfRewardsForActions *memoryOfRewardsForActions,
    ServoModel servoLeft,
    ServoModel servoRight)
{
    //reward the last move added to the move stack as positive, if there exist any
    RewardsForActions rewardsForActions = RecallRewardsForActions(state, memoryOfRewardsForActions);
    Action nextAction = DecideNextAction(rewardsForActions);
    //Complete action
    //Create a new state-action-pair
    //Add state-action-pair to recent-memory-stack, truncate if necessary
}

#include "LinkedList.h"
#include "typeDefs.h"
#include "StateActionPairs.h"
#include "Rewards.h"

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

MemoryOfRewardsForStateActionPairs memoryOfRewardsForStateActionPairs;

LinkedList<StateActionPair> mostRecentStateActionPairs;

Sensor sensorLeft;
Sensor sensorMiddle;
Sensor sensorRight;

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


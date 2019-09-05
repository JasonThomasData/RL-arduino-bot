#include "LinkedList.h"
#include "StateActionPairs.h"
#include "Rewards.h"
#include "Models.h"

MemoryOfRewardsForActions memoryOfRewardsForActions;

LinkedList<StateActionPair> mostRecentStateActionPairs;

SensorModel sensorLeft;
SensorModel sensorMiddle;
SensorModel sensorRight;

ServoModel servoLeft;
ServoModel servoRight;

void setup()
{   
    sensorLeft = { 5, 6, 7 };
    sensorMiddle = { 8, 9, 10};
    sensorRight = { 11, 12, 13 };

    ConfigureSensor(&sensorLeft);
    ConfigureSensor(&sensorMiddle);
    ConfigureSensor(&sensorRight);

    servoLeft = { 16, 500 };
    servoRight = { 17, 500 };

    mostRecentStateActionPairs = LinkedList<StateActionPair>();

    memoryOfRewardsForActions = {
        { "www", "wwb", "wbw", "wbb", "bww", "bwb", "bbw", "bbb" },
        { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
    };

    //For debugging
    //Serial.begin(9600);
}

void loop()
{
    State state = ObserveState(&sensorLeft, &sensorMiddle, &sensorRight);
    bool hasObservedRed = CheckHasObservedRed(state);

    //These conditions have mismatched levels of abstraction
    if(hasObservedRed)
    {
        //New name candidate - FailStatePolicy
        NegativePolicy(&mostRecentStateActionPairs, &memoryOfRewardsForActions, servoLeft, servoRight);
    }
    else
    {
        //Perhaps place this in a new function
        //Call it - StandardPolicy, or synonym of normal
        
        //reward the last move added to the move stack as positive, if there exist any
        RewardsForActions rewardsForActions = RecallRewardsForActions(state, &memoryOfRewardsForActions);
        Action nextAction = DecideNextAction(rewardsForActions);
        //Complete action
        //Create a new state-action-pair
        //Add state-action-pair to recent-memory-stack
    }
}

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
    sensorMiddle = { 8, 9, 10 };
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

    if(hasObservedRed)
    {
        FailStatePolicy(&mostRecentStateActionPairs, &memoryOfRewardsForActions, servoLeft, servoRight);
    }
    else
    {
        StandardPolicy(state, &mostRecentStateActionPairs, &memoryOfRewardsForActions, servoLeft, servoRight);
    }
}

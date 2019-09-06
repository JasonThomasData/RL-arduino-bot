#include "src/Agent/Models.h"

int mockGetFrequency(SensorModel *sensor, bool s2_signal, bool s3_signal) { return 0; }
#define GetFrequency mockGetFrequency

void mockTurnWheel(ServoModel servo, int wheelDirection) { }
#define TurnWheel mockTurnWheel

#include <assert.h>
#include <iostream>

#define String std::string //C++ has no String type
#include "src/Agent/StateActionPairs.h"
#include "src/Agent/StateActionFactory.ino"
#include "src/Agent/Rewards.h"
#include "src/Agent/Policy.ino"

void GivenFrequenciesProvided_WhenCategoriseFrequencyTrio_ThenResultsAsExpected()
{
    assert(CategoriseFrequencyTrio(0, 0, 0) == 'b');
    assert(CategoriseFrequencyTrio(29, 29, 29) == 'b');
    assert(CategoriseFrequencyTrio(29, 151, 151) == 'r');
    assert(CategoriseFrequencyTrio(0, 255, 255) == 'r');
    assert(CategoriseFrequencyTrio(29, 150, 151) == 'w');
    assert(CategoriseFrequencyTrio(29, 151, 150) == 'w');
    assert(CategoriseFrequencyTrio(30, 29, 29) == 'w');
    assert(CategoriseFrequencyTrio(29, 30, 29) == 'w');
    assert(CategoriseFrequencyTrio(29, 29, 30) == 'w');
}

void GivenState_rwb_WhenCheckHasObservedRed_ThenReturns_True()
{
    State state;
    state.sensorLeft = 'r';
    state.sensorMiddle = 'w';
    state.sensorRight = 'b';
    assert(CheckHasObservedRed(state) == true);
}

void GivenState_wbw_WhenCheckHasObservedRed_ThenReturns_False()
{
    State state;
    state.sensorLeft = 'w';
    state.sensorMiddle = 'b';
    state.sensorRight = 'w';
    assert(CheckHasObservedRed(state) == false);
}

void GivenAllFrequenciesAre0_WhenGetColour_ThenReturns_b()
{
    //Relies on pulseIn mocked to return 0
    SensorModel sensor;
    assert(GetColour(&sensor) == 'b');
}

void GivenState_bbb_WhenObserveState_ThenReturnsExpectedState()
{
    SensorModel sensor;
    State state = ObserveState(&sensor, &sensor, &sensor);

    assert(state.sensorLeft == 'b');
    assert(state.sensorMiddle == 'b');
    assert(state.sensorRight == 'b');
    assert(state.memoryKey[0] == 'b');
    assert(state.memoryKey[1] == 'b');
    assert(state.memoryKey[2] == 'b');
}

void GivenState_wbb_WhenRecallRewardsForActions_ThenReturnsExpectedRewards()
{
    //Arrange
    State state;
    state.sensorLeft = 'w';
    state.sensorMiddle = 'b';
    state.sensorRight = 'b';
    state.memoryKey[0] = 'w';
    state.memoryKey[1] = 'b';
    state.memoryKey[2] = 'b';

    MemoryOfRewardsForActions memoryOfRewardsForActions;
    memoryOfRewardsForActions = {
        { "www", "wwb", "wbw", "wbb", "bww", "bwb", "bbw", "bbb" },
        { { 1, 2, 3, 4 }, { 0, 1, 2, 3 }, { -1, 0, 1, 2 }, { -2, -1, 0, 1 }, { -3, -2, -1, 0 }, { -4, -3, -2, -1 }, { -5, -4, -3, -2 }, { -6, -5, -4, -3 } }
    };

    RewardsForActions expectedRewardsForActions;
    expectedRewardsForActions = memoryOfRewardsForActions.rewardsForActions[3];

    //Act
    RewardsForActions rewardsForActions;
    rewardsForActions = RecallRewardsForActions(state, &memoryOfRewardsForActions);

    //Assert
    assert(rewardsForActions.ff == expectedRewardsForActions.ff);
    assert(rewardsForActions.fb == expectedRewardsForActions.fb);
    assert(rewardsForActions.bf == expectedRewardsForActions.bf);
    assert(rewardsForActions.bb == expectedRewardsForActions.bb);
}

void GivenRewards_0_5_10_minus10_WhenDecideNextAction_ThenReturns_1700_1700()
{
    //Arrange
    RewardsForActions rewardsForActions;
    rewardsForActions = { 0, -5, 10, 7 };

    Action expectedAction = { 1700, 1700 };

    //Act
    Action action = DecideNextAction(rewardsForActions);
    
    //Assert
    assert(action.leftWheelDirection == expectedAction.leftWheelDirection);
    assert(action.rightWheelDirection == expectedAction.rightWheelDirection);
}

void GivenRewards_minus20_minus5_minus10_minus20_WhenDecideNextAction_ThenReturns_1300_1300()
{
    //Arrange
    RewardsForActions rewardsForActions;
    rewardsForActions = { -20, -5, -10, -20 };

    Action expectedAction = { 1300, 1300 };

    //Act
    Action action = DecideNextAction(rewardsForActions);
    
    //Assert
    assert(action.leftWheelDirection == expectedAction.leftWheelDirection);
    assert(action.rightWheelDirection == expectedAction.rightWheelDirection);
}

void GivenAction_WhenReverseAction_ThenReversedActionReturned()
{
    //Arrange
    Action action;
    action.leftWheelDirection = 1300;
    action.rightWheelDirection = 1700;

    //Act
    Action reversedAction = CreateReversedAction(action);

    //Assert
    assert(reversedAction.leftWheelDirection == 1700);
    assert(reversedAction.rightWheelDirection == 1300);
}

void GivenSizeOfRecentMovesStack_ThenReturnsExpectedNegativeReward()
{
    assert(DetermineNegativeReward(0) == -1);
    assert(DetermineNegativeReward(1) == -1);
    assert(DetermineNegativeReward(3) == -2);
    assert(DetermineNegativeReward(5) == -4);
}

void GivenStateActionPairAndPositiveReward_WhenApplyRewardForStateActionPair_ThenMemoryOfRewardsForActionsUpdated()
{
    //Arrange
    State state;
    state.sensorLeft = 'w';
    state.sensorMiddle = 'b';
    state.sensorRight = 'w';
    state.memoryKey[0] = 'w';
    state.memoryKey[1] = 'b';
    state.memoryKey[2] = 'w';
    
    Action action;
    action.leftWheelDirection = 1300;
    action.rightWheelDirection = 1700;
    action.memoryKey[0] = 'f';
    action.memoryKey[1] = 'f';

    StateActionPair stateActionPair = { state, action };

    int reward = 1;

    MemoryOfRewardsForActions memoryOfRewardsForActions = {
        { "www", "wwb", "wbw", "wbb", "bww", "bwb", "bbw", "bbb" },
        { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
    };

    //Act
    assert(memoryOfRewardsForActions.rewardsForActions[2].ff == 0);
    ApplyRewardForStateActionPair(stateActionPair, &memoryOfRewardsForActions, reward);

    //Assert
    assert(memoryOfRewardsForActions.rewardsForActions[2].ff == 1);
}

void GivenAgentEncountersFailedState_WhenFailStatePolicy_ThenRecentMoverStackEmptyAndMemoryUpdatedWithRewards()
{
    //Arrange
    State state1;
    state1.sensorLeft = 'w';
    state1.sensorMiddle = 'b';
    state1.sensorRight = 'w';
    state1.memoryKey[0] = 'w';
    state1.memoryKey[1] = 'b';
    state1.memoryKey[2] = 'w';
    
    Action action1;
    action1.leftWheelDirection = 1300;
    action1.rightWheelDirection = 1700;
    action1.memoryKey[0] = 'f';
    action1.memoryKey[1] = 'f';

    StateActionPair stateActionPair1 = { state1, action1 };

    State state2;
    state2.sensorLeft = 'w';
    state2.sensorMiddle = 'w';
    state2.sensorRight = 'b';
    state2.memoryKey[0] = 'w';
    state2.memoryKey[1] = 'w';
    state2.memoryKey[2] = 'b';
    
    Action action2;
    action2.leftWheelDirection = 1300;
    action2.rightWheelDirection = 1700;
    action2.memoryKey[0] = 'f';
    action2.memoryKey[1] = 'f';

    StateActionPair stateActionPair2 = { state2, action2 };

    LinkedList<StateActionPair> mostRecentStateActionPairs = LinkedList<StateActionPair>();
    mostRecentStateActionPairs.add(stateActionPair1);
    mostRecentStateActionPairs.add(stateActionPair2);

    MemoryOfRewardsForActions memoryOfRewardsForActions = {
        { "www", "wwb", "wbw", "wbb", "bww", "bwb", "bbw", "bbb" },
        { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
    };

    ServoModel servoLeft;
    ServoModel servoRight;

    //Act
    assert(mostRecentStateActionPairs.size() == 2);
    assert(memoryOfRewardsForActions.rewardsForActions[1].ff == 0);
    assert(memoryOfRewardsForActions.rewardsForActions[2].ff == 0);
    FailStatePolicy(&mostRecentStateActionPairs, &memoryOfRewardsForActions, servoLeft, servoRight);

    //Assert
    assert(mostRecentStateActionPairs.size() == 0);
    assert(memoryOfRewardsForActions.rewardsForActions[1].ff != 0);
    assert(memoryOfRewardsForActions.rewardsForActions[2].ff != 0);
}

void GivenAgentEncountersFailedState_WhenFailStatePolicy_ThenRecentMoverStackEmptyAndMemoryUpdatedWithRewards()
{
    //Arrange
    State previousState1;
    previousState1.sensorLeft = 'w';
    previousState1.sensorMiddle = 'b';
    previousState1.sensorRight = 'w';
    previousState1.memoryKey[0] = 'w';
    previousState1.memoryKey[1] = 'b';
    previousState1.memoryKey[2] = 'w';
    
    Action previousAction1;
    previousAction1.leftWheelDirection = 1300;
    previousAction1.rightWheelDirection = 1700;
    previousAction1.memoryKey[0] = 'f';
    previousAction1.memoryKey[1] = 'f';

    StateActionPair previousStateActionPair1 = { previousState1, previousAction1 };

    State previousState2;
    previousState2.sensorLeft = 'w';
    previousState2.sensorMiddle = 'w';
    previousState2.sensorRight = 'b';
    previousState2.memoryKey[0] = 'w';
    previousState2.memoryKey[1] = 'w';
    previousState2.memoryKey[2] = 'b';
    
    Action previousAction2;
    previousAction2.leftWheelDirection = 1300;
    previousAction2.rightWheelDirection = 1700;
    previousAction2.memoryKey[0] = 'f';
    previousAction2.memoryKey[1] = 'f';

    StateActionPair previousStateActionPair2 = { previousState2, previousAction2 };

    LinkedList<StateActionPair> mostRecentStateActionPairs = LinkedList<StateActionPair>();
    mostRecentStateActionPairs.add(previousStateActionPair1);
    mostRecentStateActionPairs.add(previousStateActionPair2);

    MemoryOfRewardsForActions memoryOfRewardsForActions = {
        { "www", "wwb", "wbw", "wbb", "bww", "bwb", "bbw", "bbb" },
        { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
    };

    ServoModel servoLeft;
    ServoModel servoRight;

    State currentState;
    currentState.sensorLeft = 'w';
    currentState.sensorMiddle = 'w';
    currentState.sensorRight = 'w';
    currentState.memoryKey[0] = 'w';
    currentState.memoryKey[1] = 'w';
    currentState.memoryKey[2] = 'w';

    //Act
    assert(mostRecentStateActionPairs.size() == 2);
    assert(memoryOfRewardsForActions.rewardsForActions[1].ff == 0);
    assert(memoryOfRewardsForActions.rewardsForActions[2].ff == 0);
    StandardPolicy(state, &mostRecentStateActionPairs, &memoryOfRewardsForActions, servoLeft, servoRight);

    //Assert
    assert(mostRecentStateActionPairs.size() == 3);
    assert(memoryOfRewardsForActions.rewardsForActions[1].ff == 0);
    assert(memoryOfRewardsForActions.rewardsForActions[2].ff == 1);
}

int main()
{
    GivenFrequenciesProvided_WhenCategoriseFrequencyTrio_ThenResultsAsExpected();
    GivenState_rwb_WhenCheckHasObservedRed_ThenReturns_True();
    GivenState_wbw_WhenCheckHasObservedRed_ThenReturns_False();
    GivenAllFrequenciesAre0_WhenGetColour_ThenReturns_b();
    GivenState_bbb_WhenObserveState_ThenReturnsExpectedState();
    GivenState_wbb_WhenRecallRewardsForActions_ThenReturnsExpectedRewards();
    GivenRewards_0_5_10_minus10_WhenDecideNextAction_ThenReturns_1700_1700();
    GivenRewards_minus20_minus5_minus10_minus20_WhenDecideNextAction_ThenReturns_1300_1300();
    GivenAction_WhenReverseAction_ThenReversedActionReturned();
    GivenSizeOfRecentMovesStack_ThenReturnsExpectedNegativeReward();
    GivenStateActionPairAndPositiveReward_WhenApplyRewardForStateActionPair_ThenMemoryOfRewardsForActionsUpdated();
    GivenAgentEncountersFailedState_WhenFailStatePolicy_ThenMostRecentStateActionPairsIsEmptyAndMemoryUpdatedWithRewards();
    GivenAgentDoesNotEncounterFailedState_WhenStandardPolicy_ThenMostRecentStateActionPairsIsNotEmptyAndMemoryUpdatedWithRewards();

    std::cout<< "Seems to work"<< std::endl;
}

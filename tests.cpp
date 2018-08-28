#include "src/Agent/Models.h"

int mockGetFrequency(SensorModel sensor, bool s2_signal, bool s3_signal) { return 0; }
#define GetFrequency mockGetFrequency

void mockTurnWheel(ServoModel servo, int wheelDirection) { }
#define TurnWheel mockTurnWheel

#include <iostream>
#define String std::string //C++ has no String type

#include "src/Agent/StateActionPairs.h"
#include "src/Agent/Rewards.h"
#include "src/Agent/Policy.ino"

#include "src/Agent/LinkedList.h"

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
    assert(GetColour(sensor) == 'b');
}

void GivenState_bbb_WhenObserveState_ThenReturnsExpectedState()
{
    SensorModel sensor;
    State state = ObserveState(sensor, sensor, sensor);

    assert(state.sensorLeft == 'b');
    assert(state.sensorMiddle == 'b');
    assert(state.sensorRight == 'b');
    assert(state.memoryKey[0] == 'b');
    assert(state.memoryKey[1] == 'b');
    assert(state.memoryKey[2] == 'b');
}

void GivenState_wbb_WhenRecallRewardsForActionsAfterObservingAState_ThenReturnsExpectedRewards()
{
    //Arrange
    State state;
    state.sensorLeft = 'w';
    state.sensorMiddle = 'b';
    state.sensorRight = 'b';
    state.memoryKey[0] = 'w';
    state.memoryKey[1] = 'b';
    state.memoryKey[2] = 'b';

    MemoryOfRewardsForStateActionPairs memoryOfRewardsForStateActionPairs;
    memoryOfRewardsForStateActionPairs = {
        { "www", "wwb", "wbw", "wbb", "bww", "bwb", "bbw", "bbb" },
        { { 1, 2, 3, 4 }, { 0, 1, 2, 3 }, { -1, 0, 1, 2 }, { -2, -1, 0, 1 }, { -3, -2, -1, 0 }, { -4, -3, -2, -1 }, { -5, -4, -3, -2 }, { -6, -5, -4, -3 } }
    };

    RewardsForActionsAfterObservingAState expectedRewardsForActionsAfterObservingAState;
    expectedRewardsForActionsAfterObservingAState = memoryOfRewardsForStateActionPairs.value[3];

    //Act
    RewardsForActionsAfterObservingAState rewardsForActionsAfterObservingAState;
    rewardsForActionsAfterObservingAState = RecallRewardsForActionsAfterObservingAState(state, memoryOfRewardsForStateActionPairs);

    //Assert
    assert(rewardsForActionsAfterObservingAState.ff == expectedRewardsForActionsAfterObservingAState.ff);
    assert(rewardsForActionsAfterObservingAState.fb == expectedRewardsForActionsAfterObservingAState.fb);
    assert(rewardsForActionsAfterObservingAState.bf == expectedRewardsForActionsAfterObservingAState.bf);
    assert(rewardsForActionsAfterObservingAState.bb == expectedRewardsForActionsAfterObservingAState.bb);
}

void GivenRewards_0_5_10_minus10_WhenDecideNextAction_ThenReturns_1700_1700()
{
    //Arrange
    RewardsForActionsAfterObservingAState rewardsForActionsAfterObservingAState;
    rewardsForActionsAfterObservingAState = { 0, -5, 10, 7 };

    Action expectedAction = { 1700, 1700 };

    //Act
    Action action = DecideNextAction(rewardsForActionsAfterObservingAState);
    
    //Assert
    assert(action.leftWheelDirection == expectedAction.leftWheelDirection);
    assert(action.rightWheelDirection == expectedAction.rightWheelDirection);
}

void GivenRewards_minus20_minus5_minus10_minus20_WhenDecideNextAction_ThenReturns_1300_1300()
{
    //Arrange
    RewardsForActionsAfterObservingAState rewardsForActionsAfterObservingAState;
    rewardsForActionsAfterObservingAState = { -20, -5, -10, -20 };

    Action expectedAction = { 1300, 1300 };

    //Act
    Action action = DecideNextAction(rewardsForActionsAfterObservingAState);
    
    //Assert
    assert(action.leftWheelDirection == expectedAction.leftWheelDirection);
    assert(action.rightWheelDirection == expectedAction.rightWheelDirection);
}

void GivenWheelDirection_ReturnsReversedDirection()
{
    assert(ReverseDirection(1300) == 1700);
    assert(ReverseDirection(1700) == 1300);
    assert(ReverseDirection(1500) == 1500);
}

void GivenAction_WhenReverseAction_ThenReversedActionReturned()
{
    //Arrange
    Action action;
    action.leftWheelDirection = 1300;
    action.rightWheelDirection = 1700;

    //Act
    Action reversedAction = ReverseAction(action);

    //Assert
    assert(reversedAction.leftWheelDirection == 1700);
    assert(reversedAction.rightWheelDirection == 1300);
}

void GivenNothingInList_WhenSizeCalled_Returns0()
{
    LinkedList<int> list = LinkedList<int>();
    assert(list.size() == 0);
}

void GivenNothingInList_WhenAddNodeAtPlace0_ReturnsTrue()
{
    LinkedList<int> list = LinkedList<int>();
    assert(list.add(0, 1) == true);
}

void GivenOneNodeInList_WhenAddNodeIndexLargerThanListSize_ThenNodeAddedToEndOfList()
{
    //Arrange
    LinkedList<int> list = LinkedList<int>();
    list.add(1);

    //Act
    list.add(5, 2);
    list.add(4, 3);

    //Assert
    assert(list.get(0) == 1);
    assert(list.get(1) == 2);
    assert(list.get(2) == 3);
}

void GivenNothingInList_WhenAddMinus_ReturnsFalse()
{
    //Arrange
    LinkedList<int> list = LinkedList<int>();

    //Act Assert
    try {
        list.add(-1, 1);
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception catched : " << e.what() << std::endl;
    } 
}

void GivenThreeNodesInList_WhenSizeCalled_Returns3()
{
    //Arrange
    LinkedList<int> list = LinkedList<int>();
    list.add(1);
    list.add(2);
    list.add(3);

    //Act Assert
    assert(list.size() == 3);
}

int main()
{
    GivenFrequenciesProvided_WhenCategoriseFrequencyTrio_ThenResultsAsExpected();
    GivenState_rwb_WhenCheckHasObservedRed_ThenReturns_True();
    GivenState_wbw_WhenCheckHasObservedRed_ThenReturns_False();
    GivenAllFrequenciesAre0_WhenGetColour_ThenReturns_b();
    GivenState_bbb_WhenObserveState_ThenReturnsExpectedState();
    GivenState_wbb_WhenRecallRewardsForActionsAfterObservingAState_ThenReturnsExpectedRewards();
    GivenRewards_0_5_10_minus10_WhenDecideNextAction_ThenReturns_1700_1700();
    GivenRewards_minus20_minus5_minus10_minus20_WhenDecideNextAction_ThenReturns_1300_1300();
    GivenWheelDirection_ReturnsReversedDirection();
    GivenAction_WhenReverseAction_ThenReversedActionReturned();

    std::cout<< "Seems to work"<< std::endl;

    GivenNothingInList_WhenSizeCalled_Returns0();
    GivenIndexNotInList_WhenGetNode_ReturnsNullPointer();
    GivenOneNodeInList_WhenAddNodeIndexLargerThanListSize_ThenNodeAddedToEndOfList();
    GivenNothingInList_WhenAddNodeAtPlace0_ReturnsTrue();
    //GivenNothingInList_WhenAddMinus_ReturnsFalse();
    GivenThreeNodesInList_WhenSizeCalled_Returns3();
}

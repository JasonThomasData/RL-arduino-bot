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


//g++ -std=c++14 tests.cpp -o tests && ./tests

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

void GivenNothingInList_WhenUnshiftCalled_ThenListSize1()
{
    LinkedList<int> list = LinkedList<int>();
    list.unshift(1);

    assert(list.size() == 1);
}

void GivenOneNodeInList_WhenUnshiftCalled_ThenListSize2()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(1);
    list.unshift(2);

    assert(list.size() == 2);
}

void GivenOneNodeInList_WhenUnshiftCalled_ThenExistingNodeLast()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(1);
    list.unshift(2);

    assert(list.get(0) == 2);
    assert(list.get(1) == 1);
}

void GivenNothingInList_WhenSetIsCalled_ThenReturnsFalse()
{
    LinkedList<int> list = LinkedList<int>();

    assert(list.set(-1, 1) == false);
    assert(list.set(0, 1) == false);
    assert(list.set(1, 1) == false);
}

void GivenThreeNodesInList_WhenSetIsCalledAtPlace1_ThenSecondNodeIsSet()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    list.set(1, 10);

    assert(list.get(0) == 0);
    assert(list.get(1) == 10);
    assert(list.get(2) == 2);
}

void GivenNothingInList_WhenPopIsCalled_ThenReturnsFalse()
{
    LinkedList<int> list = LinkedList<int>();
    assert(list.pop() == false);
}

void GivenTwoNodesInList_WhenPopIsCalled_ThenReturnsLastAdded()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);

    assert(list.pop() == 1);
}

void GivenTwoNodesInList_WhenPopIsCalled_ThenListIsShorter()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);

    assert(list.size() == 2);
    list.pop();
    assert(list.size() == 1);
}

void GivenNothingInList_WhenShiftIsCalled_ThenReturnsFalse()
{
    LinkedList<int> list = LinkedList<int>();
    assert(list.shift() == false);
}

void GivenOneNodeInList_WhenShiftIsCalled_ThenReturnsData()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(5);

    assert(list.shift() == 5);
}

void GivenOneNodeInList_WhenShiftIsCalled_ThenListEmpty()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(5);

    assert(list.size() == 1);
    list.shift();
    assert(list.size() == 0);
}

void GivenThreeNodesInList_WhenShiftIsCalled_ThenReturnsFirstData()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    assert(list.shift() == 0);
}

void GivenThreeNodesInList_WhenShiftIsCalled_ThenListIsShorter()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    assert(list.size() == 3);
    list.shift();
    assert(list.size() == 2);
}

void GivenNothingInList_WhenRemoveIsCalled_ThenFalseIsReturned()
{
    LinkedList<int> list = LinkedList<int>();

    assert(list.remove(0) == false);
}

void GivenThreeNodesInList_WhenRemoveIsCalledAtPlace0_ThenFirstNodeDataIsReturned()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    assert(list.remove(0) == 0);
}

void GivenThreeNodesInList_WhenRemoveIsCalledAtPlace2_ThenLastNodeDataIsReturned()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    assert(list.remove(2) == 2);
}

void GivenThreeNodesInList_WhenRemoveIsCalled_ThenListIsShorter()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    assert(list.size() == 3);
    list.remove(1);
    assert(list.size() == 2);
}

void GivenNothingInList_WhenGetIsCalled_ThenReturnsFalse()
{
    LinkedList<int> list = LinkedList<int>();

    assert(list.get(0) == false);
}

void GivenThreeNodesInList_WhenGetIsCalled_ThenReturnsData()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    assert(list.get(1) == 1);
}

void GivenNothingInList_WhenClearIsCalled_ThenSizeUnchanged()
{
    LinkedList<int> list = LinkedList<int>();

    assert(list.size() == 0);
    list.clear();
    assert(list.size() == 0);
}

void GivenThreeInList_WhenClearIsCalled_ThenListEmpty()
{
    LinkedList<int> list = LinkedList<int>();
    list.add(0);
    list.add(1);
    list.add(2);

    list.clear();

    assert(list.size() == 0);
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

    std::cout<< "Seems to work"<< std::endl;

    GivenNothingInList_WhenSizeCalled_Returns0();
    GivenOneNodeInList_WhenAddNodeIndexLargerThanListSize_ThenNodeAddedToEndOfList();
    GivenNothingInList_WhenAddNodeAtPlace0_ReturnsTrue();
    GivenThreeNodesInList_WhenSizeCalled_Returns3();
    GivenNothingInList_WhenUnshiftCalled_ThenListSize1();
    GivenOneNodeInList_WhenUnshiftCalled_ThenListSize2();
    GivenOneNodeInList_WhenUnshiftCalled_ThenExistingNodeLast();
    GivenNothingInList_WhenSetIsCalled_ThenReturnsFalse();
    GivenThreeNodesInList_WhenSetIsCalledAtPlace1_ThenSecondNodeIsSet();
    GivenNothingInList_WhenPopIsCalled_ThenReturnsFalse();
    GivenTwoNodesInList_WhenPopIsCalled_ThenReturnsLastAdded();
    GivenTwoNodesInList_WhenPopIsCalled_ThenListIsShorter();
    GivenNothingInList_WhenShiftIsCalled_ThenReturnsFalse();
    GivenOneNodeInList_WhenShiftIsCalled_ThenReturnsData();
    GivenOneNodeInList_WhenShiftIsCalled_ThenListEmpty();
    GivenThreeNodesInList_WhenShiftIsCalled_ThenReturnsFirstData();
    GivenThreeNodesInList_WhenShiftIsCalled_ThenListIsShorter();
    GivenNothingInList_WhenRemoveIsCalled_ThenFalseIsReturned();
    GivenThreeNodesInList_WhenRemoveIsCalledAtPlace0_ThenFirstNodeDataIsReturned();
    GivenThreeNodesInList_WhenRemoveIsCalledAtPlace2_ThenLastNodeDataIsReturned();
    GivenThreeNodesInList_WhenRemoveIsCalled_ThenListIsShorter();
    GivenNothingInList_WhenGetIsCalled_ThenReturnsFalse();
    GivenThreeNodesInList_WhenGetIsCalled_ThenReturnsData();
    GivenNothingInList_WhenClearIsCalled_ThenSizeUnchanged();
    GivenThreeInList_WhenClearIsCalled_ThenListEmpty();
}

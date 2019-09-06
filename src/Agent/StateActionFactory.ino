#include "StateActionPairs.h"

State CreateState(
    char sensorLeft,
    char sensorMiddle,
    char sensorRight)
{
    State state;
    state.sensorLeft = sensorLeft;
    state.sensorMiddle = sensorMiddle;
    state.sensorRight = sensorRight;

    state.memoryKey[0] = sensorLeft;
    state.memoryKey[1] = sensorMiddle;
    state.memoryKey[2] = sensorRight;
    return state;
}

Action _CreateAction(
    int leftWheelDirection,
    int rightWheelDirection,
    char leftWheelKey,
    char rightWheelKey
    )
{
    Action action;
    action.leftWheelDirection = leftWheelDirection;
    action.rightWheelDirection = rightWheelDirection;
    action.memoryKey[0] = leftWheelKey;
    action.memoryKey[1] = rightWheelKey;
    return action;
}

Action CreateAction_Forward()
{
    Action action = _CreateAction(
        1300, //AntiClockwise
        1700, //Clockwise
        'f',
        'f');
    return action;
}

Action CreateAction_TurnLeft()
{
    Action action = _CreateAction(1700, 1700, 'b', 'f');
    return action;
}

Action CreateAction_TurnRight()
{
    Action action = _CreateAction(1300, 1300, 'f', 'b');
    return action;
}

Action CreateAction_Backward()
{
    Action action = _CreateAction(1700, 1300, 'b', 'b');
    return action;
}

int _ReverseDirection(int originalDirection)
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

Action CreateReversedAction(Action originalAction)
{
    Action reversedAction;
    reversedAction.leftWheelDirection = _ReverseDirection(originalAction.leftWheelDirection);
    reversedAction.rightWheelDirection = _ReverseDirection(originalAction.rightWheelDirection);
    return reversedAction;
}

StateActionPair CreateStateActionPair(
    State state,
    Action action)
{
    StateActionPair stateActionPair = { state, action };
    return stateActionPair;
}

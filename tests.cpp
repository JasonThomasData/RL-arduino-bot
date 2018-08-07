//Mock methods
void mockPinMode(int pin, int direction) { }
void mockDigitalWrite(int pin, bool frequency) { }
int mockPulseIn(int pin, bool frequency) { return 0; }
#define pinMode mockPinMode
#define digitalWrite mockDigitalWrite
#define pulseIn mockPulseIn

//Mock types
#define String std::string
#define OUTPUT 0
#define INPUT 1
#define HIGH true
#define LOW false

#include <iostream>
#include "src/arduinoRLbot/arduinoRLbot.ino"

void GivenFrequenciesProvided_Then_ResultsAsExpected()
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

void GivenState_rwb_Then_HashObservedRed_True()
{
    State state;
    state.sensorLeft = 'r';
    state.sensorMiddle = 'w';
    state.sensorRight = 'b';
    assert(CheckHasObservedRed(state) == true);
}

void GivenState_wbw_Then_HashObservedRed_False()
{
    State state;
    state.sensorLeft = 'w';
    state.sensorMiddle = 'b';
    state.sensorRight = 'w';
    assert(CheckHasObservedRed(state) == false);
}

int main()
{
    GivenFrequenciesProvided_Then_ResultsAsExpected();
    GivenState_rwb_Then_HashObservedRed_True();
    GivenState_wbw_Then_HashObservedRed_False();
}

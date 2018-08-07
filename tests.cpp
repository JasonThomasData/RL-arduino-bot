void mockPinMode(int pin, int direction) { }
void mockDigitalWrite(int pin, bool frequency) { }
int mockPulseIn(int pin, bool frequency) { return 50; }

//Mock types
#define String std::string
#define OUTPUT 0
#define INPUT 1
#define HIGH true
#define LOW false

//Mock methods
#define pinMode mockPinMode
#define digitalWrite mockDigitalWrite
#define pulseIn mockPulseIn

#include <iostream>
#include "src/arduinoRLbot/arduinoRLbot.ino"


int mockFunc(int toAdd)
{
    return toAdd + 1;
}

int main()
{
    std::cout<< mockFunc(1)<< std::endl;
}

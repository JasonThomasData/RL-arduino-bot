#ifndef SensorModel_h
#define SensorModel_h
struct SensorModel
{
    int s0Pin;
    int s1Pin;
    int s2Pin;
    int s3Pin;
    int outputPin;
};
#endif

#ifndef ServoModel_h
#define ServoModel_h
struct ServoModel
{
    int pin;
    int moveDuration;
};
#endif

//ARE THESE THE SAME AS ACTIONS? SHOULD THEY BE?

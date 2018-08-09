#ifndef Sensor_h
#define Sensor_h
struct Sensor
{
    int s0Pin;
    int s1Pin;
    int s2Pin;
    int s3Pin;
    int outputPin;
};
#endif

#ifndef ServoInstructions_h
#define ServoInstructions_h
struct ServoInstructions
{
    int signalPin;
    int signalToSet;
};
#endif

#include "typeDefs.h"

void ConfigureSensor(Sensor *sensor)
{
    pinMode(sensor->s0Pin, OUTPUT);
    pinMode(sensor->s1Pin, OUTPUT);
    pinMode(sensor->s2Pin, OUTPUT);
    pinMode(sensor->s3Pin, OUTPUT);
    pinMode(sensor->outputPin, INPUT);
    digitalWrite(sensor->s0Pin, HIGH);
    digitalWrite(sensor->s1Pin, LOW);
}

int GetFrequency(Sensor sensor, bool s2_signal, bool s3_signal)
{
    digitalWrite(sensor.s2Pin, s2_signal);
    digitalWrite(sensor.s3Pin, s3_signal);
    return pulseIn(sensor.outputPin, LOW);
}

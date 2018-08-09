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

#include "typeDefs.h"

int GetFrequency(Sensor sensor, bool s2_signal, bool s3_signal)
{
    digitalWrite(sensor.s2Pin, s2_signal);
    digitalWrite(sensor.s3Pin, s3_signal);
    return pulseIn(sensor.outputPin, LOW);
}

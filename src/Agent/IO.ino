#include "Models.h"
#include <Servo.h>

void ConfigureSensor(SensorModel *sensor)
{
    pinMode(sensor->s2Pin, OUTPUT);
    pinMode(sensor->s3Pin, OUTPUT);
    pinMode(sensor->outputPin, INPUT);
}

int GetFrequency(SensorModel *sensor, bool s2_signal, bool s3_signal)
{
    Serial.println(sensor->outputPin);
    digitalWrite(sensor->s2Pin, s2_signal);
    digitalWrite(sensor->s3Pin, s3_signal);
    return pulseIn(sensor->outputPin, LOW);
}

//Need pointers here, if we intend to modify the servoModel state... using pointers might also give a perf boost
void TurnWheel(ServoModel servo, int wheelDirection)
{
    Servo driver;
    driver.attach(servo.pin);
    driver.write(wheelDirection);
    delay(servo.moveDuration);
}

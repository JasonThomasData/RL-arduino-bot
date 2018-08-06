#include <Servo.h> 

Servo myservo;

void setup() 
{ 
  myservo.attach(9);
} 

void loop()
{
  myservo.write(1200);  // 1500 is mid-point
} 

//The robot will live in a world where there is only white black and red.
//The numbers are the D* numbers on the board itself. S0 is D5, etc.
//The ratios here were set with 5V going to the TCS230
//THESE UNITS ARE UNRELIABLE AT 3.3V. USE 5V
//THIS TEST IS FOR A SINGLE SENSOR ONLY
//Note - not using S0 or S1 pins, because, need to use pins for servos also... not enough pins

/*
//LEFT
#define S2 5
#define S3 6
#define sensorOut 7

//MIDDLE
#define S2 8
#define S3 9
#define sensorOut 10
*/

//RIGHT
#define S2 8
#define S3 9
#define sensorOut 10

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

//Seems that Red and black have good contrast, anything not red or black can be white (AKA other)
String determineColour(int redFrequency, int greenFrequency, int blueFrequency)
{
  if (redFrequency <= 20 && (greenFrequency >= 20 || blueFrequency >= 20))
  {
    return "red";
  }
  else if (redFrequency >= 30 && (greenFrequency >= 30 || blueFrequency >= 30))
  {
    return "black";
  }
  return "white";
}

void setup() {
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  pinMode(sensorOut, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  redFrequency = pulseIn(sensorOut, LOW);
  
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  greenFrequency = pulseIn(sensorOut, LOW);

  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  blueFrequency = pulseIn(sensorOut, LOW);

  String colour = determineColour(redFrequency, greenFrequency, blueFrequency);

  Serial.print(" R = ");
  Serial.print(redFrequency);
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  Serial.print(" B = ");
  Serial.print(blueFrequency);
  Serial.print(" colour = ");
  Serial.println(colour);

  delay(2000);
}

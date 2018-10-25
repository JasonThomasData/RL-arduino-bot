//The robot will live in a world where there is only white black and red.
//The numbers are the D* numbers on the board itself. S0 is D5, etc.
//The ratios here were set with 3.3V going to the TCS230

#define S0 5
#define S1 6
#define S2 8
#define S3 9
#define sensorOut 7

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

String determineColour(int redFrequency, int greenFrequency, int blueFrequency)
{
  if (redFrequency <= 120 && greenFrequency <= 120 && blueFrequency <= 120)
  {
    return "white";
  }
  else if (redFrequency >= 600 && greenFrequency >= 600 && blueFrequency >= 500)
  {
    return "black";
  }
  else if (redFrequency <= 180 && greenFrequency >= 300 && blueFrequency >= 200)
  {
    return "red";
  }
  return "other";
}

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  pinMode(sensorOut, INPUT);
  
  digitalWrite(S0,HIGH); //Can we do without these? This will mean a higher frequency to check. Any consequences?
  digitalWrite(S1,LOW);
  
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

  delay(100);
}

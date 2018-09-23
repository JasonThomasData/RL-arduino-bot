//The robot will live in a world where there is only white black and red.
//The numbers are the D* numbers on the board itself. S0 is D5, etc.

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
  if (redFrequency <= 100 && greenFrequency <= 100 && blueFrequency <= 100)
  {
    return "white";
  }
  else if (redFrequency >= 400 && greenFrequency >= 400 && blueFrequency >= 400)
  {
    return "black";
  }
  else if (redFrequency <= 150 && greenFrequency >= 200 && blueFrequency >= 200)
  {
    return "red";
  }
  return "other";
}

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
   // Begins serial communication 
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

  Serial.print("R = ");
  Serial.print(redFrequency);
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  Serial.print(" B = ");
  Serial.println(blueFrequency);

  Serial.println(colour);

  delay(100);
}

#define buttR 2
#define buttB 3
#define m1 11
#define m2 12
#define trigPin 9
#define echoPin 10
#define red 5
#define grn 6
#define blu 7
long duration, cm, inches;
long redVal = 6;
long blueVal = 10;
long depth = 15; //depth of the container
// Red button = 6cm, Blue button = 12cm
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buttR, INPUT);
  pinMode(buttB, INPUT);
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(grn, OUTPUT);
  pinMode(blu, OUTPUT);
  pinMode(red, OUTPUT);

  // pin for the lasermodule
  //pinMode(laserPin, INPUT);
}

void photoresistorLaserLED()
{
  if (analogRead(A0) <= 160 && analogRead(A0) > 12)
  {
    analogWrite(grn, map(analogRead(A0), 12, 160, 0, 255));
    analogWrite(red, map(analogRead(A0), 12, 160, 255, 0));    
    digitalWrite(blu, HIGH);
  }
  else if (analogRead(A0) >= 161 && analogRead(A0) <= 250)
  {
    digitalWrite(blu, HIGH);
    digitalWrite(grn, HIGH);
    digitalWrite(red, LOW);
    while (ultraRead() < depth)
      {
        digitalWrite(m1, LOW);
        digitalWrite(m2, HIGH);
        delay(10);
      }
  }
  else if (analogRead(A0) <= 12)
  {
    digitalWrite(red, HIGH);
    digitalWrite(grn, HIGH);
    digitalWrite(blu, HIGH);
  }
  else if (analogRead(A0) > 250)
  {
    digitalWrite(blu, HIGH);
    digitalWrite(grn, HIGH);
    digitalWrite(red, HIGH);
  }
}

void manualMotor()
{
  photoresistorLaserLED();
  if ((digitalRead(buttR) == LOW && digitalRead(buttB) == LOW) || digitalRead(buttR) == HIGH && digitalRead(buttB) == HIGH)
  {
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
  }
  else if (digitalRead(buttR) == HIGH && digitalRead(buttB) == LOW)
  {
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
  }
  else
  {
    digitalWrite(m2, HIGH);
    digitalWrite(m1, LOW);
  }
}

void autoMotor()
{
  photoresistorLaserLED();
  if ((digitalRead(buttR) == LOW && digitalRead(buttB) == LOW) || digitalRead(buttR) == HIGH && digitalRead(buttB) == HIGH)
  {
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
  }
  else if (digitalRead(buttR) == HIGH && digitalRead(buttB) == LOW)
  {
    if (ultraRead() == redVal)
    {
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      Serial.println("lvl at redVal");
      for (int i = 0; i <= 2; i++)
      {
        digitalWrite(13, HIGH);
        delay(50);
        digitalWrite(13, LOW);
        delay(100);
      }
    }
    else if (ultraRead() > redVal)
    {
      Serial.println("pumping to redVal");
      while (ultraRead() > redVal)
      {
        digitalWrite(m1, HIGH);
        digitalWrite(m2, LOW);
        delay(10);
      }
    }
    else
    {
      Serial.println("draining to redVal");
      while (ultraRead() < redVal)
      {
        digitalWrite(m1, LOW);
        digitalWrite(m2, HIGH);
        delay(10);
      }
    }
  }
  else
  {
    if (ultraRead() == blueVal)
    {
      Serial.println("lvl at blueVal");
      digitalWrite(m1, LOW);
      digitalWrite(m2, LOW);
      for (int i = 0; i <= 2; i++)
      {
        digitalWrite(13, HIGH);
        delay(50);
        digitalWrite(13, LOW);
        delay(100);
      }
    }
    else if (ultraRead() > blueVal)
    {
      Serial.println("pumping to blueVal");
      while (ultraRead() > blueVal)
      {
        digitalWrite(m1, HIGH);
        digitalWrite(m2, LOW);
        delay(10);
      }
    }
    else
    {
      Serial.println("draining to blueVal");
      while (ultraRead() < blueVal)
      {
        digitalWrite(m1, LOW);
        digitalWrite(m2, HIGH);
        delay(10);
      }
    }
  }
}

long ultraRead() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  cm = (duration / 2) / 29.1;   // Divide by 29.1 or multiply by 0.0343
  inches = (duration / 2) / 74; // Divide by 74 or multiply by 0.0135
  //  Serial.print(inches);
  //  Serial.print("in, ");
  //  Serial.println();
  delayMicroseconds(10);
  if (cm <= 45)
  {
    Serial.print(cm);
    Serial.println("cm");
    return cm;
  }
  else
  {
    ultraRead();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  autoMotor();
//  Serial.println(analogRead(A0));
}



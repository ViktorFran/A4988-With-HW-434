#include <Arduino.h>
#include <TM1637Display.h>
#define ENA PA8
#define DIR PA9
#define STEP PA10
#define encodeA PA0
#define encodeB PA4
#define encodeButton PB13
#define i2cCl PB6
#define i2cDl PB7b

const int totStep = 3200;
volatile unsigned int speed =0;
bool enable = true;
const byte CLK = PB6;   // define CLK pin (any digital pin)
const byte DIO = PB7;   // define DIO pin (any digital pin)
TM1637Display display(CLK, DIO);

void delay_(float x)
{ // allows for delays <1ms
  if (x > 1.0)
  {
    delay(x);
  }
  else
  {
    delayMicroseconds(x * 1000.0); //convert to usec
  }
}
void motorStep(int steps, float rpm)
{
  float t = 60000.0 / (rpm * 3200 * 16 * 2.0);
  // unsigned int timer=millis();
  if (steps < 0)
  {                       // set direction (use mSteps>0 to reverse)
    digitalWrite(DIR, 0); // counter-clockwise
  }
  else
  {
    digitalWrite(DIR, 1); // clockwise
  }
  digitalWrite(ENA, LOW); // enable motor
  for (int i = 0; i < abs(steps); i++)
  { // STEP pulses
    digitalWrite(STEP, HIGH);
    delay_(t);
    digitalWrite(STEP, LOW);
    delay_(t);
  }
}

void detectA()
{
  if (digitalRead(encodeA) == HIGH)
  {
    if (digitalRead(encodeB) == LOW)
    {
      speed++;
    }
    else
    {
      speed--;
    }
  }
  else
  {
    if (digitalRead(encodeB) == HIGH){
      speed++;
    }
    else
    {
      speed--;
    }
  }
}

void detectB()
{
  if (digitalRead(encodeB) == HIGH)
  {
    if (digitalRead(encodeA) == LOW)
    {
      speed++;
    }
    else
    {
      speed--;
    }
  }
  else
  {
    if (digitalRead(encodeA) == HIGH){
      speed++;
    }
    else
    {
      speed--;
    }
  }
}

void startStop(){
  enable = !enable;
}

void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(encodeA, INPUT_PULLUP);
  pinMode(encodeB, INPUT_PULLUP);
  pinMode(encodeButton, INPUT_PULLUP);
  attachInterrupt(encodeA, detectA, FALLING);
  attachInterrupt(encodeB, detectB, FALLING);
  attachInterrupt(encodeButton, startStop, RISING);
  display.setBrightness(0x0f);
  display.(12.34);
}

void loop()
{
//  display.showNumberDec(speed); 
}

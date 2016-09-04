/*
 * Rahul Iyer
 */

#include <Servo.h>
#include <Wire.h>
#include <ArduinoNunchuk.h>

#define BAUDRATE 19200

#define CHUCK_ZERO 133
#define CHUCK_MAX 255
#define ESC_ZERO 90
#define SPEED_MAX 130

ArduinoNunchuk nunchuk = ArduinoNunchuk();

int escOutputValue = ESC_ZERO;
float currentOutputValue = ESC_ZERO;

int resetCounter = 0;
Servo ESC;

void setup()
{
  ESC.attach(9);
  Serial.begin(BAUDRATE);
  
  nunchuk.init();
  delay(100);
}

void loop()
{
  resetCounter++;
  
  if (resetCounter==40)
  {
    resetCounter=0;
    nunchuk.init();
    delay(100);
  }
  
  nunchuk.update();
  
//  Serial.print(nunchuk.analogX, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.analogY, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.accelX, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.accelY, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.accelZ, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.zButton, DEC);
//  Serial.print(' ');
//  Serial.print(nunchuk.cButton, DEC);
//  Serial.print(' ');
  
  int yValue = nunchuk.analogY;
  
  if (yValue<CHUCK_ZERO)
  {
    yValue = CHUCK_ZERO; // no backwards
  }

//  escOutputValue = map(yValue, CHUCK_ZERO, CHUCK_MAX, ESC_ZERO, SPEED_MAX); //linear relationship between joystick value and ESC output value--deemed not good enough :P

  if (yValue == CHUCK_ZERO)
  {
    escOutputValue = ESC_ZERO; //if joystick is at zero, no power to motor (coast)
    currentOutputValue = ESC_ZERO;  //reset
  }
  else
  {
    int maxPossibleSpeed = map (yValue, CHUCK_ZERO, CHUCK_MAX, ESC_ZERO, SPEED_MAX);
    
    if (escOutputValue > maxPossibleSpeed)
    {
      escOutputValue = maxPossibleSpeed;
    }
    else
    {
      float increment = (1.0 * (yValue - CHUCK_ZERO)) / (CHUCK_MAX - CHUCK_ZERO);
  
      currentOutputValue += increment;
      escOutputValue = (int) currentOutputValue;
      
      if (escOutputValue > maxPossibleSpeed)
      {
        escOutputValue = maxPossibleSpeed;
      }
    }
  }
  
  ESC.write(escOutputValue);
  Serial.println(escOutputValue); //for debugging
}

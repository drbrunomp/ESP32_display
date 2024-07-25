#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);


const int ReadPin = 32;
int       VdigPot = 0;
float     TensaoPot = 0,
          TensaoDig = 0;


void setup()
{
 	lcd.begin();
	lcd.backlight();
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(12);
}

float signalConv(float ArgAnalog)
{  
  TensaoDig = ArgAnalog * 0.80586e-3;
  return(TensaoDig);
}

void readAnalog()
{
  VdigPot = analogRead(ReadPin); 
  TensaoPot = signalConv(VdigPot);
}

void loop()
{
  readAnalog();
  lcd.setCursor(0, 0);
	lcd.printf("Digital:    %4d", VdigPot);
  lcd.setCursor(0, 1);
	lcd.printf("Tensao:     %.2f ", TensaoPot);
}



#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_I2C.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);


const int ReadPin = 32,
          Valv1Pin = 25,
          Valv2Pin = 26,
          PumpPin = 27,
          ButtonPin = 4;
int       Valv1 = 0,
          Valv2 = 0,
          Pump = 0,
          VdigPot = 0,
          Button = 0;
float     TensaoPot = 0,
          TensaoDig = 0,
          LevelTankFunction = 0,
          LevelTank = 0;

void setup()
{
 	lcd.begin();
	lcd.backlight();
  Serial.begin(9600);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  analogSetWidth(12);
  pinMode(Valv1Pin, OUTPUT);
  pinMode(Valv2Pin, OUTPUT);
  pinMode(PumpPin, OUTPUT);
  pinMode(ButtonPin, INPUT);

}
float signalConv(float ArgAnalog)
{  
  TensaoDig = ArgAnalog * 0.80586e-3;
  return(TensaoDig);
}

float levelConv(float ArgLevel)
{
  LevelTankFunction = ArgLevel / 0.00033;
  return(LevelTankFunction);
}

void resultFunctions()
{
  VdigPot = analogRead(ReadPin); 
  Button = digitalRead(ButtonPin);
  TensaoPot = signalConv(VdigPot);
  LevelTank = levelConv(TensaoPot);
}

void printSerial()
{
  if(Serial.availableForWrite() != NULL)
  {
    Serial.printf("Valor Digital: %d \n", VdigPot);
    Serial.printf("Tensão Elétrica: %.2f V \n",TensaoPot);
    Serial.printf("Nivel do Tanque: %.0f Litros \n",LevelTank);
    Serial.printf("V1: %d \n", Valv1);
    Serial.printf("V2: %d \n", Valv2);
    Serial.printf("Pump: %d \n", Pump);
    delay(500);
  }
}

void controlLogic()
{
  if(LevelTank > (0.75 * 10000))
  {
    Valv1 = LOW;
    Valv2 = HIGH;
    Pump = LOW;
  }
  else if(LevelTank > (0.50 * 10000) && LevelTank <= (0.75 * 10000))
  {
    Valv1 = HIGH;
    Valv2 = HIGH;
    Pump = HIGH;
  }
  else
  {
    Valv1 = HIGH;
    Valv2 = LOW;
    Pump = HIGH;
  }
}

void driveOutputs()
{
  digitalWrite(Valv1Pin, Valv1);
  digitalWrite(Valv2Pin, Valv2);
  digitalWrite(PumpPin, Pump);
}

void lcdPrint()
{
  if(Button == HIGH)     //Botão NF (PULL UP)
  {
    lcd.setCursor(0, 0);
	  lcd.printf("Voltage:  %.2f", TensaoPot);
    lcd.setCursor(15, 0);
    lcd.print("V");
    lcd.setCursor(0, 1);
	  lcd.printf("Level:   %5.0f", LevelTank);
     lcd.setCursor(15, 1);
    lcd.print("L");
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
	  lcd.printf("V1: %d", Valv1);
    lcd.setCursor(9, 0);
	  lcd.printf("V2: %d", Valv2);
    lcd.setCursor(0, 1);
	  lcd.printf("Pump: %d", Pump);
  }
}

void loop() 
{
  resultFunctions();
  printSerial(); 
  controlLogic();  
  driveOutputs();
  lcdPrint();
}
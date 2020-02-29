#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ShiftRegisterIO.h"
#include "ConfigInterface.h"
#include "TemperatureInterface.h"
#include "DisplayInterface.h"
#include <Adafruit_MAX31865.h>
#include <Esp.h>

#include <TaskScheduler.h>

//Delegates for platform.io
void displayTask_Callback();
void measureTask_Callback();
void sendDataTask_Callback();
void readyLED(bool state);

//Scheduled Task setup
#define _TASK_SLEEP_ON_IDLE_RUN
Scheduler runner;
Task displayTask(500, TASK_FOREVER, &displayTask_Callback, &runner, true);
Task measureTask(700, TASK_FOREVER, &measureTask_Callback, &runner, true);
Task sendDataTask(700, TASK_FOREVER, &sendDataTask_Callback, &runner, true);

// Display Configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DisplayInterface displayInterface;

//PT100 Configurartion
Adafruit_MAX31865 thermo = Adafruit_MAX31865(D8, D7, D6, D5);
TemperatureInterface temperatureInterface;
//#define RREF 430.0  old Version
#define RREF 240.0
#define RNOMINAL 100.0

// ShiftRegister Configuration
ShiftRegisterIO shiftRegisterIO;
SR_IO sr_io;

#define RMUX_IN A0

//Configuration
ConfigInterface configInterface;
Configuratrion config;
MeterData meterData[4];
int displayState = 0;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...");
  delay(100);

  shiftRegisterIO.init();
  shiftRegisterIO.write(&sr_io);
  shiftRegisterIO.write(&sr_io);

  //pinMode(RMUX_IN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      Serial.print("Error while connecting to Display"); // Don't proceed, loop forever
  }

  displayInterface.boot(&display);

  configInterface.init();
  configInterface.loadConfig(&config, meterData);

  temperatureInterface.init(thermo);

  //runner.init();
  //runner.addTask(displayTask);
  //runner.addTask(measureTask);
  delay(1000);
  //displayTask.enable();
  //measureTask.enable();
  //shiftRegisterIO.led_READY(&shiftRegisterIO, &sr_io, true);
  runner.startNow();

  //shiftRegisterIO.ledBlink(1000);
}

void loop()
{
  //shiftRegisterIO.led_READY(&shiftRegisterIO, &sr_io, true);

  runner.execute();

  //1. Display
  //displayInterface.displayMeter(&display, &meterData[0]);
  //2.1 Take Measurements
  //2.2 Calculate
  //3. Send Measuremnts
  //4. check user input
  // do all over again

  //display.clearDisplay();
  //display.setCursor(10, 10);

  // int analogValue = analogRead(RMUX_IN);
  // Serial.print("Analaog: ");
  // Serial.println(String(analogValue));
  // //display.println(analogValue);

  // channel_RJ1.temperature_up_Celcius = temperatureInterface.readTemperature(thermo, RNOMINAL, config.RREF_RJ1_T1, true);

  // sr_io = shiftRegisterIO.t_MuxSelect(sr_io, 7);
  // shiftRegisterIO.write(sr_io);
  // channel_RJ1.temperature_down_Celcius = temperatureInterface.readTemperature(&thermo, RNOMINAL, config.RREF_RJ1_T2, true);

  // meterData[0].mux_up = 5;
  // meterData[0].mux_down = 7;
  // Serial.print(meterData[0].RREF_up);
  // //meterData[0].RREF_up = 240;
  //meterData[0].RREF_down = 240;
  //temperatureInterface.readTemperature(thermo, &sr_io, &meterData[0]);

  // Serial.print("UP ");
  // Serial.println(String(meterData[0].temperature_up_Celcius));
  // Serial.print("DOWN ");
  // Serial.println(String(meterData[0].temperature_down_Celcius));

  //readTemperature(0, 0, true);
  //sr_io = shiftRegisterIO.t_MuxSelect(sr_io, -1);
  //shiftRegisterIO.write(sr_io);

  //display.display();
  //delay(1000);
}

void displayTask_Callback()
{
  shiftRegisterIO.led_READY(&shiftRegisterIO, &sr_io, false);
  switch (displayState)
  {
  case 0:
    displayInterface.boot(&display);
    displayState++;
    break;

  case 1: //Show Meter 1
    displayInterface.displayMeter(&display, &meterData[0]);
    //displayState++;
    break;

  case 2: //Show Meter 2
    displayInterface.displayMeter(&display, &meterData[1]);
    displayState++;
    break;

  case 3: //Show Meter 3
    displayInterface.displayMeter(&display, &meterData[2]);
    displayState++;
    break;

  case 4: //Show Meter 4
    displayInterface.displayMeter(&display, &meterData[3]);
    displayState = 1;
    break;

  default:
    break;
  }
}

void measureTask_Callback()
{
  shiftRegisterIO.led_READY(&shiftRegisterIO, &sr_io, false);

  unsigned long start = millis();

  shiftRegisterIO.led_RJ1(&shiftRegisterIO, &sr_io, true);
  temperatureInterface.readTemperature(thermo, &sr_io, &meterData[0]);
  shiftRegisterIO.checkMeterResistance(&shiftRegisterIO, &sr_io, &meterData[0]);
  if (meterData[0].waterMeterState)
  {
    shiftRegisterIO.led_statusRJ1(&shiftRegisterIO, &sr_io, true);
  }
  else
  {
    shiftRegisterIO.led_statusRJ1(&shiftRegisterIO, &sr_io, false);
  }
  if (meterData[0].mux_resistance_edgeDetect)
  {
    meterData->water_CounterValue_m3 += 5;
    meterData->delta_HeatEnergy_J += 4200 * 5 * (meterData->temperature_up_Celcius_mean - meterData->temperature_down_Celcius_mean);
    meterData->absolute_HeatEnergy_MWh = meterData->delta_HeatEnergy_J * 0.000000000277778;
    Serial.println(meterData->absolute_HeatEnergy_MWh);
  }

  shiftRegisterIO.led_RJ1(&shiftRegisterIO, &sr_io, false);

  //shiftRegisterIO.led_RJ2(&shiftRegisterIO, &sr_io, true);
  //temperatureInterface.readTemperature(thermo, &sr_io, &meterData[1]);
  //shiftRegisterIO.led_RJ2(&shiftRegisterIO, &sr_io, false);

  //shiftRegisterIO.led_RJ3(&shiftRegisterIO, &sr_io, true);
  //temperatureInterface.readTemperature(thermo, &sr_io, &meterData[2]);
  //shiftRegisterIO.led_RJ3(&shiftRegisterIO, &sr_io, false);

  //shiftRegisterIO.led_RJ4(&shiftRegisterIO, &sr_io, true);
  //temperatureInterface.readTemperature(thermo, &sr_io, &meterData[3]);
  //shiftRegisterIO.led_RJ4(&shiftRegisterIO, &sr_io, false);

  unsigned long end = millis();
  unsigned long duration = end - start;
  Serial.print("Duration:");
  Serial.println(duration);
}

void readyLED(bool state)
{
  sr_io.LED_Ready = state;
  shiftRegisterIO.write(&sr_io);
}
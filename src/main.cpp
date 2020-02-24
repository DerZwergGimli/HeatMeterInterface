#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "ShiftRegisterIO.h"
#include "ConfigInterface.h"
#include "TemperatureInterface.h"
#include <Adafruit_MAX31865.h>
#include <Esp.h>

// Display Configuration
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1    // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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

void readTemperature(int port, int sensor, bool printSerial);

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
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println("Booting...");
  display.display();

  configInterface.init();
  configInterface.loadConfig(&config, meterData);
  configInterface.serialPrintConfig(config);

  temperatureInterface.init(thermo);

  //shiftRegisterIO.ledBlink(1000);
}

void loop()
{
  //1. Display
  //2.1 Take Measurements
  //2.2 Calculate
  //3. Send Measuremnts
  //4. check user input
  // do all over again

  display.clearDisplay();
  display.setCursor(10, 10);

  int analogValue = analogRead(RMUX_IN);
  Serial.print("Analaog: ");
  Serial.println(String(analogValue));
  display.println(analogValue);

  // channel_RJ1.temperature_up_Celcius = temperatureInterface.readTemperature(thermo, RNOMINAL, config.RREF_RJ1_T1, true);

  // sr_io = shiftRegisterIO.t_MuxSelect(sr_io, 7);
  // shiftRegisterIO.write(sr_io);
  // channel_RJ1.temperature_down_Celcius = temperatureInterface.readTemperature(&thermo, RNOMINAL, config.RREF_RJ1_T2, true);

  meterData[0].mux_up = 5;
  meterData[0].mux_down = 7;
  Serial.print(meterData[0].RREF_up);
  //meterData[0].RREF_up = 240;
  //meterData[0].RREF_down = 240;
  temperatureInterface.readTemperature(thermo, &sr_io, &meterData[0]);

  Serial.print("UP ");
  Serial.println(String(meterData[0].temperature_up_Celcius));
  Serial.print("DOWN ");
  Serial.println(String(meterData[0].temperature_down_Celcius));

  //readTemperature(0, 0, true);
  //sr_io = shiftRegisterIO.t_MuxSelect(sr_io, -1);
  //shiftRegisterIO.write(sr_io);

  display.display();
  delay(1000);
}

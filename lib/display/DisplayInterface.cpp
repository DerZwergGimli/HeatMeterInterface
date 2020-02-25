#include "DisplayInterface.h"
#include "ConfigInterface.h"

DisplayInterface ::DisplayInterface()
{
}

void DisplayInterface::boot(Adafruit_SSD1306 *display)
{
    display->clearDisplay();
    display->setTextSize(2); // Draw 2X-scale text
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(10, 0);
    display->println("Booting...");
    display->display();
}

void DisplayInterface ::displayMeter(Adafruit_SSD1306 *display, struct MeterData *meterData)
{
    display->clearDisplay();
    display->setCursor(0, 0);
    display->setTextSize(2);
    display->println(meterData->meterID);

    display->setTextSize(1);
    display->print("Engery: ");
    display->println(meterData->hardware_CounterValue);

    display->print("Water: ");
    display->println(meterData->water_CounterValue_m3);

    display->print("Up: ");
    display->print(meterData->temperature_up_Celcius);
    display->println(" 'C");

    display->print("Down: ");
    display->print(meterData->temperature_down_Celcius);
    display->println(" 'C");

    display->display();
}

void DisplayInterface::printHello(Adafruit_SSD1306 *display)
{
    display->clearDisplay();
    display->setCursor(0, 0);
    display->setTextSize(2);
    display->print("Hello");
    display->display();
}
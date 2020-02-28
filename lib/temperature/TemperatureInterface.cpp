#include "TemperatureInterface.h"
#include <Adafruit_MAX31865.h>
#include "ShiftRegisterIO.h"
#include "ConfigInterface.h"
#include "MAX31865.h"
#include <SPI.h>

TemperatureInterface::TemperatureInterface()
{
}

void TemperatureInterface::init(Adafruit_MAX31865 thermo)
{
    thermo.begin(MAX31865_2WIRE); // set to 2WIRE or 4WIRE as necessary
}

void TemperatureInterface::readTemperature(Adafruit_MAX31865 thermo, struct SR_IO *sr_io, struct MeterData *meterData)
{
    // int : port = jack number either 1...4
    // int : sensor = number of sensor either 1 or 2
    // bool : printSerial is for serial debugging
    float RNOMINAL = 100.0;
    //TODO: EDIT TO Pinter
    ShiftRegisterIO shiftRegisterIO;

    //      UP
    shiftRegisterIO.t_MuxSelect(sr_io, meterData->mux_up);
    shiftRegisterIO.write(sr_io);
    float temp_up = thermo.temperature(RNOMINAL, meterData->RREF_up);

    if (meterData->temperature_up_Celcius_mean == 0)
    {
        //meterData->temperature_up_Celcius_smooth.setInitial(temp_up * 100);
        meterData->temperature_up_Celcius_mean = temp_up;
        meterData->temperature_up_Celcius_sum = 0;
        meterData->temperature_up_Celcius_numberOfPoints = 0;
    }
    SPI.begin();
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    SPI.setDataMode(SPI_MODE3);
    MAX31865_RTD rtd(MAX31865_RTD::RTD_PT100, D8, 240);
    rtd.configure(true, true, false, false, MAX31865_FAULT_DETECTION_NONE,
                  true, true, 0x0000, 0x7fff);
    //delay(100);
    rtd.read_all();

    if (rtd.status() == 0)
    {
        double temperature = rtd.temperature();
        Serial.print(" T = ");
        Serial.print(temperature, 1);
        Serial.println(" deg C");
    }
    else
    {
        Serial.print("RTD fault register: ");
        Serial.print(rtd.status());
        Serial.print(": ");
        if (rtd.status() & MAX31865_FAULT_HIGH_THRESHOLD)
        {
            Serial.println("RTD high threshold exceeded");
        }
        else if (rtd.status() & MAX31865_FAULT_LOW_THRESHOLD)
        {
            Serial.println("RTD low threshold exceeded");
        }
        else if (rtd.status() & MAX31865_FAULT_REFIN)
        {
            Serial.println("REFIN- > 0.85 x V_BIAS");
        }
        else if (rtd.status() & MAX31865_FAULT_REFIN_FORCE)
        {
            Serial.println("REFIN- < 0.85 x V_BIAS, FORCE- open");
        }
        else if (rtd.status() & MAX31865_FAULT_RTDIN_FORCE)
        {
            Serial.println("RTDIN- < 0.85 x V_BIAS, FORCE- open");
        }
        else if (rtd.status() & MAX31865_FAULT_VOLTAGE)
        {
            Serial.println("Overvoltage/undervoltage fault");
        }
        else
        {
            Serial.println("Unknown fault; check connection");
        }
    }

    thermo.begin();

    meterData->temperature_up_Celcius = temp_up;
    //meterData->temperature_up_Celcius_mean = ((float)meterData->temperature_up_Celcius_smooth.calc25(temp_up * 100)) / 100;
    meterData->temperature_up_Celcius_sum += temp_up;
    meterData->temperature_up_Celcius_numberOfPoints++;
    meterData->temperature_up_Celcius_mean = meterData->temperature_up_Celcius_sum / meterData->temperature_up_Celcius_numberOfPoints;

    //      DOWN
    shiftRegisterIO.t_MuxSelect(sr_io, meterData->mux_down);
    shiftRegisterIO.write(sr_io);
    float temp_down = thermo.temperature(RNOMINAL, meterData->RREF_down);
    if (meterData->temperature_down_Celcius_mean == 0)
    {
        //meterData->temperature_down_Celcius_smooth.setInitial(temp_down * 100);
        meterData->temperature_down_Celcius_mean = temp_down;
        meterData->temperature_down_Celcius_sum = 0;
        meterData->temperature_down_Celcius_numberOfPoints = 0;
    }

    meterData->temperature_down_Celcius = temp_down;
    //meterData->temperature_down_Celcius_mean = ((float)meterData->temperature_down_Celcius_smooth.calc25(temp_down * 100)) / 100;
    meterData->temperature_down_Celcius_sum += temp_down;
    meterData->temperature_down_Celcius_numberOfPoints++;
    meterData->temperature_down_Celcius_mean = meterData->temperature_down_Celcius_sum / meterData->temperature_down_Celcius_numberOfPoints;

    // shiftRegisterIO.t_MuxSelect(sr_io, meterData->mux_up);
    // shiftRegisterIO.write(sr_io);
    // meterData->temperature_up_Celcius = thermo.temperature(RNOMINAL, meterData->RREF_up);

    // shiftRegisterIO.t_MuxSelect(sr_io, meterData->mux_down);
    // shiftRegisterIO.write(sr_io);
    // meterData->temperature_down_Celcius = thermo.temperature(RNOMINAL, meterData->RREF_down);

    // uint16_t rtd = thermo.readRTD();
    // float ratio = rtd;
    // //ratio /= 32768;
    // ratio /= 32768;
    // thermo.temperature(RNOMINAL, RREF);

    // if (printSerial)
    // {
    //     Serial.println("###########################################");
    //     Serial.print("RTD value: ");
    //     Serial.println(rtd);
    //     Serial.print("Ratio = ");
    //     Serial.println(ratio, 8);
    //     Serial.print("Resistance = ");
    //     Serial.println(RREF * ratio, 8);
    //     Serial.print("Temperature = ");
    //     Serial.println(thermo.temperature(RNOMINAL, RREF));

    //     uint8_t fault = thermo.readFault();
    //     if (fault)
    //     {
    //         Serial.print("Fault 0x");
    //         Serial.println(fault, HEX);
    //         if (fault & MAX31865_FAULT_HIGHTHRESH)
    //         {
    //             Serial.println("RTD High Threshold");
    //         }
    //         if (fault & MAX31865_FAULT_LOWTHRESH)
    //         {
    //             Serial.println("RTD Low Threshold");
    //         }
    //         if (fault & MAX31865_FAULT_REFINLOW)
    //         {
    //             Serial.println("REFIN- > 0.85 x Bias");
    //         }
    //         if (fault & MAX31865_FAULT_REFINHIGH)
    //         {
    //             Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    //         }
    //         if (fault & MAX31865_FAULT_RTDINLOW)
    //         {
    //             Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    //         }
    //         if (fault & MAX31865_FAULT_OVUV)
    //         {
    //             Serial.println("Under/Over voltage");
    //         }
    //         thermo.clearFault();
    //     }
    //     Serial.println();
    // }
    // return thermo.temperature(RNOMINAL, RREF);

    //shiftRegisterIO.t_MuxSelect(sr_io, 99);
    //shiftRegisterIO.write(sr_io);
}
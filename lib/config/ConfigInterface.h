#ifndef ConfigInterface_h
#define ConfigInterface_h

#include "ExpSmooth.h"

struct Configuratrion
{
    int ID;
    float RREF_RJ1_T1;
    float RREF_RJ1_T2;
    float RREF_RJ2_T1;
    float RREF_RJ2_T2;
    float RREF_RJ3_T1;
    float RREF_RJ3_T2;
    float RREF_RJ4_T1;
    float RREF_RJ4_T2;
};

struct MeterData
{
    short meterID;
    bool waterMeterState;
    bool ledState;
    long hardware_CounterValue;
    float temperature_up_Celcius;
    ExpSmooth temperature_up_Celcius_smooth;
    float temperature_up_Celcius_mean;
    float temperature_up_Celcius_sum;
    int temperature_up_Celcius_numberOfPoints;
    float temperature_down_Celcius;
    ExpSmooth temperature_down_Celcius_smooth;
    float temperature_down_Celcius_mean;
    float temperature_down_Celcius_sum;
    int temperature_down_Celcius_numberOfPoints;
    float water_CounterValue_m3;

    float delta_HeatEnergy_J;
    float mass_kg;
    float specific_HeatCapacity_JpKGtK;
    float deltaTemperature_K;

    float RREF_up;
    float RREF_down;

    int mux_up;
    int mux_down;
    int mux_resistance;
    int mux_resistance_threshold;
    bool mux_resistance_edgeDetect;
};

class ConfigInterface
{
public:
    ConfigInterface();

    bool init();
    bool loadConfig(struct Configuratrion *config, MeterData (&meterData)[4]);
    bool saveConfig(Configuratrion config);
    void serialPrintConfig(Configuratrion config);
};

#endif
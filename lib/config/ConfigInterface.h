#ifndef ConfigInterface_h
#define ConfigInterface_h

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
    float temperature_down_Celcius;
    float water_CounterValue_m3;

    float delta_HeatEnergy_J;
    float mass_kg;
    float specific_HeatCapacity_JpKGtK;
    float deltaTemperature_K;

    float RREF_up;
    float RREF_down;
    int mux_up;
    int mux_down;
};

class ConfigInterface
{
public:
    ConfigInterface();

    bool init();
    bool loadConfig(struct Configuratrion *config, struct MeterData (&meterData)[4]);
    bool saveConfig(Configuratrion config);
    void serialPrintConfig(Configuratrion config);
};

#endif
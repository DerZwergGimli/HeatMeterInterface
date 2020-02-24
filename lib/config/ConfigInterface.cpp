#include "ConfigInterface.h"
#include <ArduinoJson.h>
#include "FS.h"

ConfigInterface ::ConfigInterface() {}

bool ConfigInterface::init()
{
    Serial.println("Mounting FS...");

    if (!SPIFFS.begin())
    {
        Serial.println("Failed to mount file system");
        return false;
    }
    return true;
}

bool ConfigInterface::loadConfig(struct Configuratrion *conf, struct MeterData (&meterData)[4])
{
    File configFile = SPIFFS.open("/config.json", "r");
    if (!configFile)
    {
        Serial.println("Failed to open config file");
        return false;
    }

    int size = configFile.size();
    if (size > 1024)
    {
        Serial.println("Config file size is too large");
        return false;
    }

    // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);

    StaticJsonDocument<400> doc;
    auto error = deserializeJson(doc, buf.get());
    if (error)
    {
        Serial.println("Failed to parse config file");
        return conf;
    }

    conf->ID = doc["ID"];

    meterData[0].meterID = doc["RJ1_MeterID"];
    meterData[0].RREF_up = doc["RJ1_RREF_T1"];
    meterData[0].RREF_down = doc["RJ1_RREF_T2"];
    meterData[0].mux_up = doc["RJ1_MUX_T1"];
    meterData[0].mux_down = doc["RJ1_MUX_T2"];
    meterData[0].hardware_CounterValue = doc["RJ1_Hardware_CounterValue"];
    meterData[0].water_CounterValue_m3 = doc["RJ1_Water_CounterValue_m3"];

    return false;
}

bool ConfigInterface::saveConfig(Configuratrion config)
{
    StaticJsonDocument<200> doc;
    doc["ID"] = 100;
    doc["serverName"] = "api.example.com";
    doc["accessToken"] = "128du9as8du12eoue8da98h123ueh9h98";

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile)
    {
        Serial.println("Failed to open config file for writing");
        return false;
    }

    serializeJson(doc, configFile);
    return true;
}

void ConfigInterface::serialPrintConfig(Configuratrion conf)
{
    Serial.print("Loaded ID: ");
    Serial.println(conf.ID);
    Serial.print("Loaded RREF_RJ1_T1: ");
    Serial.println(conf.RREF_RJ1_T1);
    Serial.print("Loaded RREF_RJ1_T2: ");
    Serial.println(conf.RREF_RJ1_T2);
    Serial.print("Loaded RREF_RJ2_T1: ");
    Serial.println(conf.RREF_RJ2_T1);
    Serial.print("Loaded RREF_RJ2_T2: ");
    Serial.println(conf.RREF_RJ2_T2);
    Serial.print("Loaded RREF_RJ3_T1: ");
    Serial.println(conf.RREF_RJ3_T1);
    Serial.print("Loaded RREF_RJ3_T2: ");
    Serial.println(conf.RREF_RJ3_T2);
    Serial.print("Loaded RREF_RJ4_T1: ");
    Serial.println(conf.RREF_RJ4_T1);
    Serial.print("Loaded RREF_RJ4_T2: ");
    Serial.println(conf.RREF_RJ4_T2);
}
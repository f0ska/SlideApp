#pragma once

#include <ArduinoOTA.h>

class ArduinoOta
{
private:
    static void otaTask(void* parameter)
    {
        for (;;) {
            ArduinoOTA.handle();
            delay(3500);
        }
    }

    void setupCallbacks()
    {
        ArduinoOTA.onStart([]() {
            String type = ArduinoOTA.getCommand() == U_FLASH ? "sketch" : "filesystem";
            Serial.println("Start updating " + type);
        });

        ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });

        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });

        ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if      (error == OTA_AUTH_ERROR)    Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)     Serial.println("End Failed");
        });
    }

public:
    void setPort(uint16_t port)             { ArduinoOTA.setPort(port); }
    void setPassword(const char* password)  { ArduinoOTA.setPassword(password); }
    void setPasswordHash(const char* hash)  { ArduinoOTA.setPasswordHash(hash); }

    void begin(const char* hostname = nullptr)
    {
        ArduinoOTA.setHostname(hostname != nullptr ? hostname : WiFi.getHostname());
        setupCallbacks();
        ArduinoOTA.begin();

        Serial.println("OTA Initialized");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());

        xTaskCreate(otaTask, "OTA", 10000, NULL, 1, NULL);
    }
};

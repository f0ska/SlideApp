#pragma once

#include <WiFi.h>
#include <WiFiUdp.h>
#include <Timezone32.h>
#include <TimeLib32.h>

class NtpSync
{
private:
    static constexpr int      PACKET_SIZE = 48;
    static constexpr uint16_t LOCAL_PORT  = 8888;

    WiFiUDP     udp;
    byte        packetBuffer[PACKET_SIZE];
    Timezone    localTZ;
    const char* serverName;

    static NtpSync*& instance()
    {
        static NtpSync* inst = nullptr;
        return inst;
    }

    static time32_t syncCallback()
    {
        return instance()->requestTime();
    }

    void sendPacket(IPAddress& address)
    {
        memset(packetBuffer, 0, PACKET_SIZE);
        packetBuffer[0]  = 0b11100011; // LI, Version, Mode
        packetBuffer[1]  = 0;          // Stratum
        packetBuffer[2]  = 6;          // Polling Interval
        packetBuffer[3]  = 0xEC;       // Peer Clock Precision
        packetBuffer[12] = 49;
        packetBuffer[13] = 0x4E;
        packetBuffer[14] = 49;
        packetBuffer[15] = 52;
        udp.beginPacket(address, 123);
        udp.write(packetBuffer, PACKET_SIZE);
        udp.endPacket();
    }

public:
    NtpSync(TimeChangeRule summer, TimeChangeRule winter,
            const char* server = "time.nist.gov")
        : localTZ(summer, winter), serverName(server)
    {
    }

    void begin()
    {
        instance() = this;
        udp.begin(LOCAL_PORT);
        setSyncProvider(syncCallback);
        setSyncInterval(5);
    }

    time32_t requestTime()
    {
        IPAddress serverIP;
        while (udp.parsePacket() > 0);
        WiFi.hostByName(serverName, serverIP);
        sendPacket(serverIP);

        uint32_t beginWait = millis();
        while (millis() - beginWait < 1500) {
            if (udp.parsePacket() >= PACKET_SIZE) {
                udp.read(packetBuffer, PACKET_SIZE);
                unsigned long secsSince1900 =
                    (unsigned long)packetBuffer[40] << 24 |
                    (unsigned long)packetBuffer[41] << 16 |
                    (unsigned long)packetBuffer[42] << 8  |
                    (unsigned long)packetBuffer[43];
                return localTZ.toLocal(secsSince1900 - 2208988800UL);
            }
        }
        return 0;
    }
};

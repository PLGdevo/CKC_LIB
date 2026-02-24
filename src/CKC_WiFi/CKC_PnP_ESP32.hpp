#ifndef CKC_ESP32
#define CKC_ESP32

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <IPAddress.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <MQTT/ESP32_MQTT.hpp>

#define WIFI_AP_IP IPAddress(192, 168, 27, 1)
#define WIFI_AP_Subnet IPAddress(255, 255, 255, 0)

char STA_WIFI_NAME[32];
char STA_WIFI_PASS[32];
#define STA_WIFI_PORT "80"

#define AP_WIFI_NAME "CKC:"
#define AP_WIFI_PASS "CKC@2026"
#define AP_WIFI_IP "192.168.1.4" 
#define AP_WIFI_PORT "80"
enum CKC_WiFI_TASK
{
    MODE_CONFIG_WIFI,
    MODE_STA,
    MODE_AP,
    MODE_AP_STA,
    MODE_CONNECTED,
    WIFI_DISCONNECTED,
};
CKC_WiFI_TASK WiFi_TASK = MODE_STA;
CKC_MQTT MQTT;
class CKC_PnP
{
public:
    CKC_PnP() {};
    void init(const char *sta_ssid, const char *sta_pass);
    void CKC_state_Connect_STA();
    void CKC_state_Connect_AP();
    void CKC_mode_connected();
    bool CkC_Connected();
    void handler_button();
    void run();

private:
    IPAddress _ipAddr;
    // sta
    char _sta_ssid[64];
    char _sta_pass[32];
    char _sta_ip[16];
    char _sta_port[5] = STA_WIFI_PORT;
    int _rssi;
    // ap
    char _ap_ssid[64] = AP_WIFI_NAME;
    char _ap_pass[32] = AP_WIFI_PASS;
    char _ap_ip[16] = AP_WIFI_IP;
    char _ap_port[5] = AP_WIFI_PORT;
    char _mac[12];
    // var local
    unsigned long _SendRssiTime;
    unsigned int count_wifiConnect;
    unsigned long t0, t1, t2, t3, t4;
    String _ping;
#define FLASH_BTN 0 // nút BOOT/FLASH trên ESP32 thường là GPIO0

    unsigned long pressStart = 0;
    bool triggered = false;
};
void CKC_PnP::init(const char *sta_ssid, const char *sta_pass)
{
    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", _ap_ssid, _mac);

    CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _sta_ip);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    // CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _mac);

    // CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
    // CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
    // CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
    // CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);

    delay(100);
    t1 = millis();
    pinMode(FLASH_BTN, INPUT_PULLUP); // nút kéo xuống GND khi nhấn
}
void CKC_PnP::CKC_state_Connect_STA()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(_sta_ssid, _sta_pass);
    while (WiFi.status() != WL_CONNECTED && millis() - t1 <= 20000)
    {
        if (millis() - t0 > 1000)
        {
            CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
            t0 = millis();
        }
        handler_button();
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        CKC_LOG_DEBUG("WIFI", "WIFI_CONNECTED :)) ");
        CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", WiFi.localIP().toString());
        CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
        MQTT.begin();
        WiFi_TASK = MODE_CONNECTED;
    }
    else
    {
        CKC_LOG_DEBUG("WIFI", "WIFI_CONNECT_FALSE !!!!!! ");
        CKC_LOG_DEBUG("WIFI", "RUN_AP");
        CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
        CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
        CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
        CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
        WiFi_TASK = MODE_AP;
        WiFi.mode(WIFI_OFF);
        WiFi.mode(WIFI_AP);
        _ipAddr.fromString(_ap_ip);
        WiFi.softAP(_ap_ssid, _ap_pass);
        t2 = millis();
    }
}
void CKC_PnP::CKC_state_Connect_AP()
{
    if (millis() - t2 > 30000)
    {
        WiFi_TASK = MODE_STA;
        CKC_LOG_DEBUG("WIFI", "RUN_STA");
        CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
        CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
        t1 = millis();
    }
    else
    {
        if (millis() - t3 > 1000)
        {
            CKC_LOG_DEBUG("WIFI", "Waiting ___ %ds\r", (millis() - t2) / 1000);
            t3 = millis();
        }
    }
}
void CKC_PnP::CKC_mode_connected()
{
    MQTT.run();
}
bool CKC_PnP::CkC_Connected()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void CKC_PnP::handler_button()
{
#ifdef BUTTON_MODE
    bool pressed = (digitalRead(FLASH_BTN) == LOW); // nhấn = LOW

    if (pressed)
    {
        if (pressStart == 0)
            pressStart = millis();
        // giữ đủ 5s và chỉ kích 1 lần
        if (!triggered && (millis() - pressStart >= 5000))
        {
            triggered = true;
            CKC_LOG_DEBUG("WIFI", "MODE_AP_run:");
            CKC_LOG_DEBUG("WIFI", "WIFI_CONNECT_FALSE !!!!!! ");
            CKC_LOG_DEBUG("WIFI", "RUN_AP");
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
            CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
            WiFi_TASK = MODE_AP;
            WiFi.mode(WIFI_OFF);
            WiFi.mode(WIFI_AP);
            _ipAddr.fromString(_ap_ip);
            WiFi.softAP(_ap_ssid, _ap_pass);
            t2 = millis();
            // TODO: đặt lệnh bạn muốn ở đây
        }
    }
    else
    {
        // nhả nút thì reset lại
        pressStart = 0;
        triggered = false;
    }

#endif
}
void CKC_PnP::run()
{
    switch (WiFi_TASK)
    {
    case MODE_STA:
        CKC_state_Connect_STA();
        break;
    case MODE_AP:
        CKC_state_Connect_AP();
        break;
    case MODE_CONNECTED:
        CKC_mode_connected();
        break;
    default:
        break;
    }
    handler_button();
    // CkC_Connected();
}

#endif
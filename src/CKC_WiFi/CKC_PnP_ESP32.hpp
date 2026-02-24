#ifndef CKC_ESP32
#define CKC_ESP32

#include <WiFi.h>
#include <HTTPClient.h>
<<<<<<< HEAD
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
=======


class CKC
{
public:
    CKC() {};
    void begin(String sta_ssid, String sta_pass);
    void end();
    void sendDATA(String Data1, String Data2, String Data3, String Data4, String Data5);
    void reconnectWifi(String sta_ssid, String sta_pass);

private:
    String ssidSTA, passSTA;
    String data1, data2, data3, data4, data5;
};
void CKC::begin(String sta_ssid, String sta_pass)
{
    ssidSTA = sta_ssid;
    passSTA = sta_pass;
    WiFi.begin(ssidSTA, passSTA);
    Serial.print("[CKC] Wifi connecting");
    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime <= 10000)
    {
        delay(1000);
        Serial.print(".");
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFi.mode(WIFI_STA);
        Serial.println("[CKC] Connected! ESP32 IP: " + WiFi.localIP().toString());
    }
    else
    {
        WiFi.mode(WIFI_AP);
        WiFi.softAP("ESP-AP MODE", "123456789", 1);
        IPAddress IP = WiFi.softAPIP();
        Serial.println("\n[CKC] CAN'T CONNECT TO WiFI");
        Serial.println("[CKC] SWITCHING TO AP MODE");
        Serial.println("[CKC] ESP32 AP IP address: " + IP.toString());
        Serial.println("[CKC] SSID: ESP-AP MODE");
        Serial.println("[CKC] Pass: 123456789");
    }
}
void CKC::end()
{
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("[CKC] WiFi stopped");
}
void CKC::sendDATA(String Data1, String Data2, String Data3, String Data4, String Data5)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("[CKC] NO WiFi → DATA NOT SENT");
        return;
    }
    HTTPClient http;
    String url = "http://10.240.14.201:3000/data";
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    String json = "{";
    json += "\"data1\":\"" + Data1 + "\",";
    json += "\"data2\":\"" + Data2 + "\",";
    json += "\"data3\":\"" + Data3 + "\",";
    json += "\"data4\":\"" + Data4 + "\",";
    json += "\"data5\":\"" + Data5 + "\"";
    json += "}";
    int httpCode = http.POST(json);
    if (httpCode > 0)
    {
        Serial.println("[CKC] DATA SENT OK");
        Serial.println(http.getString());
    }
    else
    {
        Serial.println("[CKC] SEND FAILED");
    }
    http.end();
}

void CKC::reconnectWifi(String sta_ssid, String sta_pass)
{

    static unsigned long lastTry = 0;

    if (millis() - lastTry < 5000)
        return;

    lastTry = millis();
    if (WiFi.status() == WL_CONNECTED)
        return;
    if (WiFi.getMode() == WIFI_AP)
    {
        Serial.println("[CKC] AP mode: checking WiFi...");
        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
        unsigned long t = millis();
        while (WiFi.status() != WL_CONNECTED &&
               millis() - t < 5000)
        {
            delay(300);
            Serial.print(".");
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\n[CKC] WiFi FOUND → STA CONNECTED");
            Serial.println(WiFi.localIP());
        }
        else
        {
            Serial.println("\n[CKC] WiFi NOT FOUND → back to AP");
            WiFi.disconnect(true);
            WiFi.mode(WIFI_AP);
            WiFi.softAP("ESP-AP MODE", "123456789", 1);
>>>>>>> 51e16ddc631f3760002cbcc7aa6247e66d06a3f3
        }
    }
    else
    {
<<<<<<< HEAD
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

=======
        Serial.println("[CKC] STA lost → reconnecting");
        WiFi.disconnect();
        WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
    }
}
>>>>>>> 51e16ddc631f3760002cbcc7aa6247e66d06a3f3
#endif
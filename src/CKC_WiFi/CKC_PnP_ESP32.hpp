#ifndef CKC_ESP32
#define CKC_ESP32

#include <WiFi.h>
#include <HTTPClient.h>


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
        }
    }
    else
    {
        Serial.println("[CKC] STA lost → reconnecting");
        WiFi.disconnect();
        WiFi.begin(ssidSTA.c_str(), passSTA.c_str());
    }
}
#endif
#ifndef CKC_H
#define CKC_H
#include <WiFi.h>
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
#endif

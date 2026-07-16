#ifndef CKC_OTA_H
#define CKC_OTA_H

#include <Arduino.h>
#include <WebServer.h>
#include <Update.h>
#include <ESPmDNS.h>
#include<OTA/ota_UI.h>

class CKC_OTA
{
public:

    void begin(WebServer &server);
    void stop();

private:

    WebServer *_server;

    void setupRoute();

    void handleRoot();

    void handleUpdate();

    void handleInfo();
};

#endif
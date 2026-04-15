#ifndef INC_CKC_PROTOCAL_HPP_
#define INC_CKC_PROTOCAL_HPP_

#include <CKC/CKC_debug.hpp>
#include <CKC_WiFi/CKC_PnP_ESP32.hpp>

class CKC_Protocall
{
private:
    CKC_PnP<CKC_MQTT<PubSubClient>> CKC_PNP;

public:
    CKC_Protocall(/* args */);
    ~CKC_Protocall();
    void begin(const char *sta_ssid, const char *sta_pass);
    void run();
    bool connected();
    void WriteControl(uint8_t pinV, float param);
    void WriteTelemetry(uint8_t pinV, float param);
};

CKC_Protocall::CKC_Protocall(/* args */)
{
    
}

CKC_Protocall::~CKC_Protocall()
{
}

void CKC_Protocall::begin(const char *sta_ssid, const char *sta_pass)
{
    this->CKC_PNP.init(sta_ssid, sta_pass);
}

void CKC_Protocall::run()
{
    this->CKC_PNP.run();
}

bool CKC_Protocall::connected()
{
    return this->CKC_PNP.CkC_Connected();
}

void CKC_Protocall::WriteControl(uint8_t pinV, float param)
{
    if (this->CKC_PNP.CkC_Connected())
    {

        CKC_LOG_DEBUG("WRITE", "Pin V%d   value: %f  ", pinV, param);
        String MAC = WiFi.macAddress();        
        String PLG = "{\"mac_address\":\"" + String(MAC) + "\",\"data\":{\"value\":" + String(param, 2) + "}}";
        serverMQTT.CKC_publishData(PLG);
    }

    // API_MESS.WRITE(pinV,param);
}
void CKC_Protocall::WriteTelemetry(uint8_t pinV, float param)
{
    if (this->CKC_PNP.CkC_Connected())
    {
        CKC_LOG_DEBUG("WRITE", "Pin V%d   value: %f  ", pinV, param);
        String MAC = WiFi.macAddress();        
        String PLG = "{\"mac_address\":\"" + String(MAC) + "\",\"data\":{\"value\":" + String(param, 2) + "}}";
        serverMQTT.CKC_publishData(PLG);
    }    
}

CKC_Protocall CKC;

#endif
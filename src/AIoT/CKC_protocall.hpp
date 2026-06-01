#ifndef INC_CKC_PROTOCAL_HPP_
#define INC_CKC_PROTOCAL_HPP_

#include <AIoT/CKC_debug.hpp>
#include <CKC_WiFi/CKC_PnP_ESP32.hpp>
#include <Modbus/modbus.h>
// #include <Modbus/CKC_modbus.h>

class CKC_Protocall
{
private:
    CKC_PnP<CKC_MQTT<PubSubClient>> CKC_PNP;
    CKC_MQTT<PubSubClient> serverMQTT;
    CkC_APi API_MESS;
    cJSON *tele_root = NULL;
    cJSON *dataObj_tele = NULL;
    cJSON *control_root = NULL;
    cJSON *dataObj_control = NULL;
    unsigned long ait_time, ait_set_time;

public:
    CKC_Protocall();
    ~CKC_Protocall();
    void begin(const char *sta_ssid, const char *sta_pass);
    void begin(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass);
    void run();
    bool connected();
    template <typename... Args>
    void setTelemetry(Args... args);
    template <typename... Args>
    void setControl(Args... args);
    void writeControl(const char *key, const CKCParam value);
    void writeTelemetry(const char *key, const CKCParam value);
    int addTimeEvent(unsigned long time, void (*callback)());
    void timeEvented();
    void (*_timerCallback)() = NULL;
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
    this->setTelemetry();
}

void CKC_Protocall::begin(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass)
{
    this->CKC_PNP.init(sta_ssid, sta_pass, mqtt_userName, mqtt_pass);
    this->setControl();
    this->setTelemetry();
}

void CKC_Protocall::run()
{
    this->CKC_PNP.run();
    this->timeEvented();
}

void CKC_Protocall::timeEvented()
{
    unsigned long now = millis();

    if (now - ait_time >= ait_set_time)
    {
        ait_time = now;

        if (_timerCallback != NULL)
        {
            _timerCallback();
        }
    }
}

int CKC_Protocall::addTimeEvent(unsigned long time, void (*callback)())
{
    ait_set_time = time;
    ait_time = millis();
    _timerCallback = callback;
    return 1;
}

bool CKC_Protocall::connected()
{
    if (this->CKC_PNP.CkC_Connected() && this->serverMQTT._connect())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void CKC_Protocall::writeControl(const char *key, const CKCParam value)
{
    if (this->CKC_PNP.CkC_Connected() && this->serverMQTT._connect())
    {
        const char *data_control = this->API_MESS.WriteControl(key, value);
        serverMQTT.CKC_publishData_control(data_control);
    }
}
template <typename... Args>
void CKC_Protocall::setControl(Args... args)
{
    if (control_root == NULL)
    {
        control_root = cJSON_CreateObject();
        dataObj_control = cJSON_CreateObject();

        char macStr[18];
        WiFi.macAddress().toCharArray(macStr, sizeof(macStr));
        cJSON_AddStringToObject(control_root, "mac_address", macStr);
        cJSON_AddItemToObject(control_root, "data", dataObj_control);
    }
    else
    {
        cJSON_DeleteItemFromObject(control_root, "data");
        dataObj_control = cJSON_CreateObject();
        cJSON_AddItemToObject(control_root, "data", dataObj_control);
    }

    const char *keys[] = {args...};

    constexpr size_t count = sizeof...(args);

    for (size_t i = 0; i < count; i++)
    {
        if (keys[i] == nullptr)
            continue;

        cJSON_AddNumberToObject(
            dataObj_control,
            keys[i],
            0);
    }

    char buffer[256];

    if (cJSON_PrintPreallocated(control_root, buffer, sizeof(buffer), 0))
    {
        CKC_LOG_DEBUG("SET_TELE", "%s", buffer);

        API_MESS.Set_control(buffer);
    }
    else
    {
        CKC_LOG_ERROR("SET_TELE", "Buffer too small!");
    }
    
}
void CKC_Protocall::writeTelemetry(const char *key, const CKCParam value)
{
    if (this->CKC_PNP.CkC_Connected() && this->serverMQTT._connect())
    {
        const char *data = this->API_MESS.WriteTelemetry(key, value);
        serverMQTT.CKC_publishData_tele(data);
    }
}
template <typename... Args>
void CKC_Protocall::setTelemetry(Args... args)
{
    if (tele_root == NULL)
    {
        tele_root = cJSON_CreateObject();
        dataObj_tele = cJSON_CreateObject();

        char macStr[18];
        WiFi.macAddress().toCharArray(macStr, sizeof(macStr));
        cJSON_AddStringToObject(tele_root, "mac_address", macStr);
        cJSON_AddItemToObject(tele_root, "data", dataObj_tele);
    }
    else
    {
        cJSON_DeleteItemFromObject(tele_root, "data");
        dataObj_tele = cJSON_CreateObject();
        cJSON_AddItemToObject(tele_root, "data", dataObj_tele);
    }

    const char *keys[] = {args...};

    constexpr size_t count = sizeof...(args);

    for (size_t i = 0; i < count; i++)
    {
        if (keys[i] == nullptr)
            continue;

        cJSON_AddNumberToObject(dataObj_tele, keys[i], 0);
    }

    char buffer[256];

    if (cJSON_PrintPreallocated(tele_root, buffer, sizeof(buffer), 0))
    {
        CKC_LOG_DEBUG("SET_TELE", "%s", buffer);

        API_MESS.Set_telemetry(buffer);
    }
    else
    {
        CKC_LOG_ERROR("SET_TELE", "Buffer too small!");
    }
}

CKC_Protocall CKC;

#endif
#ifndef INC_CKC_PROTOCAL_HPP_
#define INC_CKC_PROTOCAL_HPP_

#include "AIoT/CKC_Defined.hpp"
#include "AIoT/CKC_debug.hpp"
#include "CKC_WiFi/CKC_PnP_ESP32.hpp"
#include "Modbus/modbus.h"
// #include <Modbus/CKC_modbus.h>

#define MAX_TIMER_EVENT 10

struct TimerEvent
{
    unsigned long interval;
    unsigned long previousMillis;
    void (*callback)();
    bool enable;
};

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
    cJSON *status_root =NULL;
    cJSON *dataObj_status=NULL;
    // unsigned long ait_time, ait_set_time;
    TimerEvent timerList[MAX_TIMER_EVENT];
    unsigned long ait_time1, ait_set_time1=3000;

public:
    CKC_Protocall();
    ~CKC_Protocall();
    void begin(const char *sta_ssid, const char *sta_pass);
    void begin(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass);
    void run();
    bool connected();
    bool config();
    template <typename... Args>
    void setTelemetry(Args... args);
    template <typename... Args>
    void setControl(Args... args);
    template <typename... Args>
    void setStatus(Args... args);
    void writeControl(const char *key, const CKCParam value);
    void writeTelemetry(const char *key, const CKCParam value);
    void writeStatus(const char *key, const CKCParam value);
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
    this->setStatus();
    this->setControl();
}

void CKC_Protocall::begin(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass)
{
    this->CKC_PNP.init(sta_ssid, sta_pass, mqtt_userName, mqtt_pass);
    this->setControl();
    this->setTelemetry();
    this->setStatus();
}

void CKC_Protocall::run()
{
    this->CKC_PNP.run();
    this->timeEvented();

    unsigned long now1 = millis();
    if (now1 - ait_time1 >= ait_set_time1)
    {
        ait_time1 = now1;
        this->writeStatus("status", "online");
    }
}

void CKC_Protocall::timeEvented()
{
    unsigned long now = millis();

    for (int i = 0; i < MAX_TIMER_EVENT; i++)
    {
        if (timerList[i].enable)
        {
            if (now - timerList[i].previousMillis >= timerList[i].interval)
            {
                timerList[i].previousMillis = now;

                if (timerList[i].callback != NULL)
                {
                    timerList[i].callback();
                }
            }
        }
    }
}

int CKC_Protocall::addTimeEvent(unsigned long time, void (*callback)())
{
    for (int i = 0; i < MAX_TIMER_EVENT; i++)
    {
        if (!timerList[i].enable)
        {
            timerList[i].interval = time;
            timerList[i].previousMillis = millis();
            timerList[i].callback = callback;
            timerList[i].enable = true;

            return i; // trả về ID
        }
    }

    return -1; // đầy
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

bool CKC_Protocall::config()
{
    if (this->CKC_PNP.CKC_Config_mode())
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
        // CKC_LOG_DEBUG("SET_CONTROL", "%s", buffer);
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
        // CKC_LOG_DEBUG("SET_TELE", "%s", buffer);

        API_MESS.Set_telemetry(buffer);
    }
    else
    {
        CKC_LOG_ERROR("SET_TELE", "Buffer too small!");
    }
}

template <typename... Args>
void CKC_Protocall::setStatus(Args... args)
{
    if (status_root == NULL)
    {
        status_root = cJSON_CreateObject();
        dataObj_status = cJSON_CreateObject();

        char macStr[18];
        WiFi.macAddress().toCharArray(macStr, sizeof(macStr));
        cJSON_AddStringToObject(status_root, "mac_address", macStr);
        cJSON_AddItemToObject(status_root, "data", dataObj_status);
    }
    else
    {
        cJSON_DeleteItemFromObject(status_root, "data");
        dataObj_status = cJSON_CreateObject();
        cJSON_AddItemToObject(status_root, "data", dataObj_status);
    }

    const char *keys[] = {args...};

    constexpr size_t count = sizeof...(args);

    for (size_t i = 0; i < count; i++)
    {
        if (keys[i] == nullptr)
            continue;

        cJSON_AddNumberToObject(dataObj_status, keys[i], 0);
    }

    char buffer[256];

    if (cJSON_PrintPreallocated(status_root, buffer, sizeof(buffer), 0))
    {
        CKC_LOG_DEBUG("SET_STATUS", "%s", buffer);

        API_MESS.Set_status(buffer);
    }
    else
    {
        CKC_LOG_ERROR("SET_STATUS", "Buffer too small!");
    }
}

void CKC_Protocall::writeStatus(const char *key, const CKCParam value)
{
    if (this->CKC_PNP.CkC_Connected() && this->serverMQTT._connect())
    {
        const char *dataObj_status = this->API_MESS.WriteStatus(key, value);
        serverMQTT.CKC_publishData_status(dataObj_status);
    }
}

CKC_Protocall CKC;

#endif
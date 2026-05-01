#ifndef INC_CKC_PROTOCAL_HPP_
#define INC_CKC_PROTOCAL_HPP_

#include <CKC/CKC_debug.hpp>
#include <CKC_WiFi/CKC_PnP_ESP32.hpp>

class CKC_Protocall
{
private:
    CKC_PnP<CKC_MQTT<PubSubClient>> CKC_PNP;
    CkC_APi API_MESS;
    cJSON *tele_root = NULL;
    cJSON *dataObj = NULL;

public:
    CKC_Protocall(/* args */);
    ~CKC_Protocall();
    void begin(const char *sta_ssid, const char *sta_pass);
    void run();
    bool connected();
    void set_Telemetry(const char *first, ...);
    void WriteControl(const char *key, const CKCParam value);
    void WriteTelemetry(const char *key, const CKCParam value);
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

void CKC_Protocall::WriteControl(const char *key, const CKCParam value)
{
    if (this->CKC_PNP.CkC_Connected())
    {
        const char *data = this->API_MESS.WriteTelemetry(key, value);
        serverMQTT.CKC_publishData(data);
    }
}
void CKC_Protocall::WriteTelemetry(const char *key, const CKCParam value)
{
    if (this->CKC_PNP.CkC_Connected())
    {
        const char *data = this->API_MESS.WriteTelemetry(key, value);
        serverMQTT.CKC_publishData(data);
    }
}
void CKC_Protocall::set_Telemetry(const char *first, ...)
{
    //  CHỈ tạo 1 lần (tránh fragment heap)
    if (tele_root == NULL)
    {
        tele_root = cJSON_CreateObject();
        dataObj = cJSON_CreateObject();

        // mac không đổi → set 1 lần
        char macStr[18];
        WiFi.macAddress().toCharArray(macStr, sizeof(macStr));

        cJSON_AddStringToObject(tele_root, "mac_address", macStr);
        cJSON_AddItemToObject(tele_root, "data", dataObj);
    }
    else
    {
        //  nếu đã có thì chỉ clear data (không delete root)
        cJSON_DeleteItemFromObject(tele_root, "data");
        dataObj = cJSON_CreateObject();
        cJSON_AddItemToObject(tele_root, "data", dataObj);
    }

    //  thêm key
    va_list args;
    va_start(args, first);

    const char *key = first;

    while (key != NULL)
    {
        cJSON_AddNumberToObject(dataObj, key, 0);
        key = va_arg(args, const char *);
    }

    va_end(args);

    // 👉 dùng buffer tĩnh (KHÔNG malloc)
    char buffer[256];
    if (cJSON_PrintPreallocated(tele_root, buffer, sizeof(buffer), 0))
    {
        CKC_LOG_DEBUG("SET_TELE", "%s", buffer);
        API_MESS.Set_telemetry(buffer);
    }
    else
    {
        CKC_LOG_DEBUG("SET_TELE", "Buffer too small!");
    }
}

CKC_Protocall CKC;

#endif
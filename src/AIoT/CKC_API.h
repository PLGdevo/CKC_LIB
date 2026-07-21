#ifndef INC_CKC_API_H
#define INC_CKC_API_H

#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <stdarg.h>
#include <AIoT/CKC_topic.h>
#include "UIlty/cJSON.hpp"
#include <AIoT/CKC_Param.hpp>
#include <AIoT/CKC_handler.hpp>
#include <MQTT/NPT_Client/NTPClient.h>
#include <MQTT/PubSubClient/PubSubClient.h>

WiFiClientSecure server;
PubSubClient mqttClient(server);

CKC_DataHandler PLGtech;

typedef enum
{
    PIN_DO,
    PIN_DI,
    PIN_AI,
    PIN_AO,
    PIN_UNKNOWN
} PinType_t;

PinType_t parsePinType(const char *type)
{
    if (strcmp(type, "DO") == 0)
        return PIN_DO;
    if (strcmp(type, "DI") == 0)
        return PIN_DI;
    if (strcmp(type, "AI") == 0)
        return PIN_AI;
    if (strcmp(type, "AO") == 0)
        return PIN_AO;
    return PIN_UNKNOWN;
}

class CkC_APi
{

public:
    void begin();
    void handleMessage(const char *topic, const char *payload);
    void dowm(const char *payload);
    void handlerVirtual_Pin(const char *payload);
    void handler_control(const char *payload);
    void handlerArduino_Pin(const char *payload);
    void handler_data(const char *payload);
    void virtualWrite(uint16_t pin, const CKCParam &param);    
    void Set_telemetry(const char *telemetry);
    void Set_control(const char *control);
    void Set_status(const char *Status);
    void feedback_control(const char *payload);
    const char *WriteControl(const char *key, const CKCParam value);
    const char *WriteTelemetry(const char *key, const CKCParam value);
    const char *WriteStatus(const char *key, const CKCParam value);

private:   

    int Pin;
    const char *type;
    const char *type_value;
    uint8_t V_pin;
    cJSON *telemetry_root = NULL;
    cJSON *control_root = NULL;
    cJSON *status_root = NULL;

#define CKC_API_SUB_PREFIX_CONTROL_TOPIC "control"
#ifdef CKC_100_PINS
    String CKC_uuid_list[100];
    int16_t CKC_ID_control[];
#else
    String CKC_uuid_list[5];
    int16_t CKC_ID_control[5];
    int16_t CKC_ID_type[5];
#endif
};

#endif // INC_CKC_API_H
/* How to read Payload Json
                payload
                    |
                    |
                Array length
                    |
        --------------------------------------------------------------
        |                           |                                 |
    virtual pin                 pin_config                           modbus
        |                           |                                  |
    {"id":V?,                   {"GPIO":value,
    "value":value,               "pinType":DO/DI/AI/AO,
    }                            "value":?
                                }


{
    "control_id": 35,
    "uuid": "5f841df7-ca4d-4c31-aebb-b99548c45d5c",
    "data": {
        "pin": "V1",
    "value": 1
    }
}

*/
#ifndef INC_CKC_API_HPP_
#define INC_CKC_API_HPP_

#include <HTTPClient.h>
#include <CKC/CKC_topic.h>
#include "UIlty/cJSON.hpp"
#include <CKC/CKC_Param.hpp>
#include <CKC/CKC_handler.hpp>

typedef enum
{
    PIN_DO,
    PIN_DI,
    PIN_AI,
    PIN_AO,
    PIN_UNKNOWN
    ////
    /////////////////////////////////
    ////////////////
    /////////////
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
    void begin() {}
    void handleMessage(const char *topic, const char *payload);
    void dowm(const char *payload);
    void handlerVirtual_Pin(const char *payload);
    void handlerVirtual_telemetry(const char *payload);
    void handlerArduino_Pin(const char *payload);
    void virtualWrite(uint16_t pin, const CKCParam &param);
    void WRITE(uint8_t pinV,const CKCParam& param);
    void READ(uint8_t pinV,const CKCParam& param);

private:
    int Pin;
    const char *type;
    const char *type_value;
    uint8_t V_pin;
#define CKC_API_SUB_PREFIX_DOWN_TOPIC "down"
#define CKC_API_SUB_PREFIX_ARDUINO_TOPIC "arduino_pin"
#define CKC_API_SUB_PREFIX_VIRTUAL_TOPIC "virtual_pin"
#define CKC_API_SUB_PREFIX_CHANGE_WIFI_TOPIC "/wifi/change"
#ifdef CKC_100_PINS
    String CKC_uuid_list[100];
    int16_t CKC_ID_control[];
#else
    String CKC_uuid_list[50];
    int16_t CKC_ID_control[];
#endif
};

void CkC_APi::handleMessage(const char *topic, const char *payload)
{
    CKC_LOG_DEBUG("MQTT->API", "Topic %s | %s", topic, payload);
    if (strncmp(topic, CKC_BASE_TOPIC, strlen(CKC_BASE_TOPIC)) != 0)
        return;
    String topicStr = topic;
    String sub_Prefix;

    int p1 = topicStr.indexOf('/');
    int p2 = topicStr.indexOf('/', p1 + 1);
    int p3 = topicStr.indexOf('/', p2 + 1);
    int p4 = topicStr.indexOf('/', p3 + 1);

    // base_Topic = topicStr.substring(0, p2);
    // token = topicStr.substring(p2 + 1, p3);
    sub_Prefix = topicStr.substring(p3 + 1, p4);

    Serial.print("SUB_PREFIX: ");
    Serial.println(sub_Prefix);

    if (sub_Prefix == CKC_API_SUB_PREFIX_ARDUINO_TOPIC)
    {
        this->handlerArduino_Pin(payload);
    }
    else if (sub_Prefix == CKC_API_SUB_PREFIX_VIRTUAL_TOPIC)
    {
        this->handlerVirtual_Pin(payload);
    }
    else if (sub_Prefix == CKC_API_SUB_PREFIX_DOWN_TOPIC)
    {
        this->dowm(payload);
    }
    else
    {
        this->handlerVirtual_telemetry(payload);
    }
}

void CkC_APi::handlerVirtual_telemetry(const char *payload)
{
    /*
    {
    "data": {
        "pin": "V1",
    "value": 1
    },
    "control_id": 35,
    "uuid": "5f841df7-ca4d-4c31-aebb-b99548c45d5c"
}*/
    cJSON *root = cJSON_Parse(payload);
    if (root == NULL)
    {
        CKC_LOG_DEBUG("ERR", "JSON parse failed!\n");
        return;
    }

    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (cJSON_IsObject(data))
    {
        // ===== Type ====
        cJSON *Pin_type = cJSON_GetObjectItem(data, "pin");
        if (cJSON_IsString(Pin_type) && Pin_type->valuestring != NULL)
        {
            CKC_LOG_DEBUG("MESS", "virtual_pin = %s", Pin_type->valuestring);
            type = Pin_type->valuestring;
            if (type[0] == 'V')
            {
                V_pin = atoi(type + 1);
            }
        }
        // ===== value ====
        cJSON *handle_value = cJSON_GetObjectItem(data, "value");
        CKCParam val = parseItem(handle_value);
        // ===== DISPATCH =====
        if (V_pin < CKC_HandlerCount)
        {
            CKC_HandlerVector[V_pin](V_pin, val);
        }
        else
        {
            CKC_LOG_DEBUG("ERR", "Invalid pin V%d", V_pin);
        }
    }
    cJSON *uuid = cJSON_GetObjectItem(root, "uuid");
    if (cJSON_IsString(uuid))
    {
        CKC_uuid_list[V_pin] = uuid->valuestring;
        CKC_LOG_DEBUG("json sub", "uuid : %s", CKC_uuid_list[V_pin].c_str());
    }

    cJSON *control_id = cJSON_GetObjectItem(root, "control_id");
    if (cJSON_IsNumber(control_id))
    {
        CKC_ID_control[V_pin] = control_id->valueint;
        CKC_LOG_DEBUG("json sub", "id_control : %d", CKC_ID_control[V_pin]);
    }
    cJSON_Delete(root);
}

void CkC_APi::handlerArduino_Pin(const char *payload)
{
    cJSON *root = cJSON_Parse(payload);
    if (root == NULL)
    {
        CKC_LOG_DEBUG("ERR", "JSON parse failed!\n");
        return;
    }
    // 🔥 đúng key
    cJSON *arr = cJSON_GetObjectItem(root, "Arduino_pins");

    if (cJSON_IsArray(arr))
    {
        int size = cJSON_GetArraySize(arr);
        Serial.printf("virtual_pins size = %d", size);

        for (int i = 0; i < size; i++)
        {
            cJSON *obj = cJSON_GetArrayItem(arr, i);
            if (!cJSON_IsObject(obj))
                continue;

            // ===== GPIO =====
            cJSON *GPIO = cJSON_GetObjectItem(obj, "GPIO");
            if (cJSON_IsNumber(GPIO))
            {
                CKC_LOG_DEBUG("MESS", "GPIO = %d", GPIO->valueint);
                Pin = GPIO->valueint;
            }
            // ===== Type ====
            cJSON *Pin_type = cJSON_GetObjectItem(obj, "Pintype");
            if (cJSON_IsString(Pin_type))
            {
                CKC_LOG_DEBUG("MESS", "pinType = %s", Pin_type->valuestring);
                type = Pin_type->valuestring;
                //"pinType":DO/DI/AI/AO,
            }
            // ===== value ====
            cJSON *handle_value = cJSON_GetObjectItem(obj, "value");
            CKCParam val = parseItem(handle_value);

            switch (parsePinType(type))
            {
            case PIN_DO:
                pinMode(Pin, OUTPUT);
                digitalWrite(Pin, val.getInt());
                break;
            case PIN_DI:
                pinMode(Pin, INPUT);
                break;
            case PIN_AI:
                pinMode(Pin, INPUT);
                break;
            case PIN_AO:
                pinMode(Pin, OUTPUT);
                digitalWrite(Pin, val.getInt());
                break;
            default:
                break;
            }
        }
    }
    cJSON_Delete(root);
}

void CkC_APi::handlerVirtual_Pin(const char *payload)
{
    cJSON *root = cJSON_Parse(payload);
    if (root == NULL)
    {
        CKC_LOG_DEBUG("ERR", "JSON parse failed!\n");
        return;
    }
    // 🔥 đúng key
    cJSON *arr = cJSON_GetObjectItem(root, "virtual_pins");

    if (cJSON_IsArray(arr))
    {
        int size = cJSON_GetArraySize(arr);
        Serial.printf("virtual_pins size = %d", size);

        for (int i = 0; i < size; i++)
        {
            cJSON *obj = cJSON_GetArrayItem(arr, i);
            if (!cJSON_IsObject(obj))
                continue;

            // ===== Type ====
            cJSON *Pin_type = cJSON_GetObjectItem(obj, "virtual_pin");
            if (cJSON_IsString(Pin_type) && Pin_type->valuestring != NULL)
            {
                CKC_LOG_DEBUG("MESS", "virtual_pin = %s", Pin_type->valuestring);
                type = Pin_type->valuestring;

                if (type[0] == 'V')
                {
                    V_pin = atoi(type + 1);
                }
                else
                {
                    continue;
                }
                CKC_LOG_DEBUG("MESS", "virtual_pin = %d", V_pin);
            }
            // ===== value ====
            cJSON *handle_value = cJSON_GetObjectItem(obj, "value");
            CKCParam val = parseItem(handle_value);
            // ===== DISPATCH =====
            if (V_pin < CKC_HandlerCount)
            {
                CKC_HandlerVector[V_pin](V_pin, val);
            }
            else
            {
                CKC_LOG_DEBUG("ERR", "Invalid pin V%d", V_pin);
            }
        }
    }
    cJSON_Delete(root);
}

void CkC_APi::dowm(const char *payload)
{
    cJSON *root = cJSON_Parse(payload);
    if (root == NULL)
    {
        CKC_LOG_DEBUG("ERR", "JSON parse failed!\n");
        return;
    }

    // 🔥 đúng key
    cJSON *arr = cJSON_GetObjectItem(root, "virtual_pins");

    if (cJSON_IsArray(arr))
    {
        int size = cJSON_GetArraySize(arr);
        Serial.printf("virtual_pins size = %d\n", size);

        for (int i = 0; i < size; i++)
        {
            cJSON *obj = cJSON_GetArrayItem(arr, i);
            if (!cJSON_IsObject(obj))
                continue;

            // ===== id =====
            cJSON *id = cJSON_GetObjectItem(obj, "id");
            if (cJSON_IsNumber(id))
            {
                CKC_LOG_DEBUG("MESS", "id = %d\n", id->valueint);
            }

            // ===== config =====
            cJSON *config = cJSON_GetObjectItem(obj, "config");
            if (cJSON_IsNumber(config))
            {
                CKC_LOG_DEBUG("MESS", "config = %d\n", config->valueint);
            }

            // ===== pinType =====
            cJSON *type = cJSON_GetObjectItem(obj, "pinType");
            if (cJSON_IsString(type))
            {
                CKC_LOG_DEBUG("MESS", "pinType = %s\n", type->valuestring);
            }
        }
    }
    else
    {
        Serial.println("virtual_pins is not array");
    }

    cJSON_Delete(root);
}
void CkC_APi::WRITE(uint8_t pinV,const CKCParam& param)
{
   
}
CkC_APi API_MESS;

#endif // INC_CKC_API_HPP_

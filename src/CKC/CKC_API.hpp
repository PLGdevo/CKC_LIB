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

---------------------------------------------------------------------------------------------------------------------------------------------------------
                            virtualpin
{
    "control_id": 35,
    "uuid": "5f841df7-ca4d-4c31-aebb-b99548c45d5c",
    "data": {
        "pin": "V1",
    "value": 1
    }
}
---------------------------------------------------------------------------------------------------------------------------------------------------------
                            control GPIO
    {
   "data":{"mcu_pin":{"GPIO": "number_int","pinType":"DO/DI/AO/AI","value": value }},
   "control_id": number,
   "uuid": "5f841df7-ca4d-4c31-aebb-b99548c45d5c"
    }
    {"mcu_pin":
            {   
                "GPIO": "26",
                "pinType":"DO",
                "value": value 
            }
    }
-------------------------------------------------------------------------------------------------------------------------------------------------------
   {
   "data":{"mcu_pin":[{"GPIO": "number_int","pinType":"DO/DI/AO/AI","value": value },{"GPIO": "number_int","pinType":"DO/DI/AO/AI","value": value }]},
   "control_id": number,
   "uuid": "5f841df7-ca4d-4c31-aebb-b99548c45d5c"
    }


*/
#ifndef INC_CKC_API_HPP_
#define INC_CKC_API_HPP_

#include <HTTPClient.h>
#include <CKC/CKC_topic.h>
#include "UIlty/cJSON.hpp"
#include <CKC/CKC_Param.hpp>
#include <CKC/CKC_handler.hpp>
#include <stdarg.h>

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
    void begin() {}
    void handleMessage(const char *topic, const char *payload);
    void dowm(const char *payload);
    void handlerVirtual_Pin(const char *payload);
    void handler_control(const char *payload);
    void handlerArduino_Pin(const char *payload);
    void virtualWrite(uint16_t pin, const CKCParam &param);
    void Set_telemetry(const char *telemetry);
    void WriteControl(const char *key, const CKCParam value);
    const char *WriteTelemetry(const char *key, const CKCParam value);   

private:
    

    int Pin;
    const char *type;
    const char *type_value;
    uint8_t V_pin;
    cJSON *telemetry_root = NULL;

#define CKC_API_SUB_PREFIX_CONTROL_TOPIC "control"
#ifdef CKC_100_PINS
    String CKC_uuid_list[100];
    int16_t CKC_ID_control[];
#else
    String CKC_uuid_list[50];
    int16_t CKC_ID_control[50];
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
    sub_Prefix = topicStr.substring(p2 + 1, p3);

    if (sub_Prefix == CKC_API_SUB_PREFIX_CONTROL_TOPIC)
    {
        CKC_LOG_DEBUG("Topic", "%s", sub_Prefix);
        this->handler_control(payload);
    }
}

void CkC_APi::handler_control(const char *payload)
{
    cJSON *root = cJSON_Parse(payload);
    if (root == NULL)
    {
        CKC_LOG_DEBUG("ERR", "JSON parse failed!\n");
        return;
    }

    cJSON *data = cJSON_GetObjectItem(root, "data");
    if (cJSON_IsObject(data))
    {
        // Object Type
        cJSON *Object = cJSON_GetObjectItem(data, "V_pin");
        if (cJSON_IsObject(Object))
        {
            char *Object_tem = cJSON_Print(Object);
            // CKC_LOG_DEBUG("API_mode", "%s", Object_tem);
            this->handlerVirtual_Pin(Object_tem);
        }
        cJSON *Object1 = cJSON_GetObjectItem(data, "mcu_pin");
        if (cJSON_IsObject(Object1))
        {
            char *Object_tem1 = cJSON_Print(data);
            // CKC_LOG_DEBUG("API_mode", "%s", Object_tem1);
            this->handlerArduino_Pin(Object_tem1);
        }
        else if (cJSON_IsArray(Object1))
        {
            char *Object_tem1 = cJSON_Print(data);
            // CKC_LOG_DEBUG("API_mode", "%s", Object_tem1);
            this->handlerArduino_Pin(Object_tem1);
        }
    }
    cJSON *uuid = cJSON_GetObjectItem(root, "uuid");
    if (cJSON_IsString(uuid))
    {
        CKC_uuid_list[V_pin] = uuid->valuestring;
        // CKC_LOG_DEBUG("json sub", "uuid : %s", CKC_uuid_list[V_pin].c_str());
    }

    cJSON *control_id = cJSON_GetObjectItem(root, "control_id");
    if (cJSON_IsNumber(control_id))
    {
        CKC_ID_control[V_pin] = control_id->valueint;
        // CKC_LOG_DEBUG("json sub", "id_control : %d", CKC_ID_control[V_pin]);
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
    cJSON *arr = cJSON_GetObjectItem(root, "mcu_pin");

    if (cJSON_IsArray(arr))
    {
        int size = cJSON_GetArraySize(arr);
        // Serial.printf("virtual_pins size = %d", size);

        for (int i = 0; i < size; i++)
        {
            cJSON *obj = cJSON_GetArrayItem(arr, i);
            if (!cJSON_IsObject(obj))
                continue;

            // ===== GPIO =====
            cJSON *GPIO = cJSON_GetObjectItem(obj, "GPIO");
            if (cJSON_IsNumber(GPIO))
            {
                // CKC_LOG_DEBUG("MESS", "GPIO = %d", GPIO->valueint);
                Pin = GPIO->valueint;
            }
            // ===== Type ====
            cJSON *Pin_type = cJSON_GetObjectItem(obj, "Pintype");
            if (cJSON_IsString(Pin_type))
            {
                // CKC_LOG_DEBUG("MESS", "pinType = %s", Pin_type->valuestring);
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
    else
    {
        // ===== GPIO =====
        cJSON *GPIO = cJSON_GetObjectItem(arr, "GPIO");
        if (cJSON_IsNumber(GPIO))
        {
            // CKC_LOG_DEBUG("MESS", "GPIO = %d", GPIO->valueint);
            Pin = GPIO->valueint;
        }
        // ===== Type ====
        cJSON *Pin_type = cJSON_GetObjectItem(arr, "Pintype");
        if (cJSON_IsString(Pin_type))
        {
            // CKC_LOG_DEBUG("MESS", "pinType = %s", Pin_type->valuestring);
            type = Pin_type->valuestring;
            //"pinType":DO/DI/AI/AO,
        }
        // ===== value ====
        cJSON *handle_value = cJSON_GetObjectItem(arr, "value");
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

    // ===== Type ====
    cJSON *Pin_type = cJSON_GetObjectItem(root, "pin");
    if (cJSON_IsString(Pin_type) && Pin_type->valuestring != NULL)
    {
        // CKC_LOG_DEBUG("MESS", "virtual_pin = %s", Pin_type->valuestring);
        type = Pin_type->valuestring;
        if (type[0] == 'V')
        {
            V_pin = atoi(type + 1);
        }
    }

    // ===== value ====
    cJSON *handle_value = cJSON_GetObjectItem(root, "value");
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
void CkC_APi::Set_telemetry(const char *telemetry)
{
    if (telemetry_root != NULL)
    {
        cJSON_Delete(telemetry_root);
        telemetry_root = NULL;
    }

    telemetry_root = cJSON_Parse(telemetry);
    char *jsonStr = cJSON_PrintUnformatted(telemetry_root);
    CKC_LOG_DEBUG("SET_TELEMETRY", "%s", jsonStr);
    if (telemetry_root == NULL)
    {
        CKC_LOG_DEBUG("API", "Parse failed");
        return;
    }
}

void CkC_APi::WriteControl(const char *key, const CKCParam value)
{
}

const char *CkC_APi::WriteTelemetry(const char *key, const CKCParam value)
{
    if (!telemetry_root) return nullptr;

    cJSON *ObjectData = cJSON_GetObjectItem(telemetry_root, "data");    
    if (!ObjectData) return nullptr;

    cJSON *newItem = NULL;

    switch (value.getType())
    {
    case CKCParam::Type::INT:
        newItem = cJSON_CreateNumber(value.getInt());
        break;

    case CKCParam::Type::FLOAT:
        newItem = cJSON_CreateNumber(value.getFloat());
        break;

    case CKCParam::Type::DOUBLE:
        newItem = cJSON_CreateNumber(value.getDouble());
        break;

    case CKCParam::Type::BOOL:
        newItem = cJSON_CreateBool(value.getBool());
        break;

    case CKCParam::Type::STRING:
        newItem = cJSON_CreateString(value.getString().c_str());
        break;

    default:
        return nullptr;
    }

    if (!newItem) return nullptr;

    cJSON *existing = cJSON_GetObjectItem(ObjectData, key);

    if (existing)
        cJSON_ReplaceItemInObject(ObjectData, key, newItem);
    else
        cJSON_AddItemToObject(ObjectData, key, newItem);

    // ✅ STATIC BUFFER
    static char buffer[256];

    if (cJSON_PrintPreallocated(telemetry_root, buffer, sizeof(buffer), 0))
    {
        CKC_LOG_DEBUG("TELEMETRY", "%s", buffer);
        return buffer;
    }
    else
    {
        CKC_LOG_DEBUG("TELEMETRY", "Buffer too small!");
        return nullptr;
    }
}

CkC_APi API_MESS;

#endif // INC_CKC_API_HPP_

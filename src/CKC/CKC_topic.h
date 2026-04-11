#ifndef INC_CKC_TOPIC
#define INC_CKC_TOPIC

/*
Topic format:
    eoh/chip/<token>/<prefix topic>
*/
#define CKC_BASE_TOPIC "device/"

/* Subscribe topic */
#define CKC_SUB_PREFIX_TELEMETRY_TOPIC               "/telemetry"
#define CKC_SUB_PREFIX_ARDUINO_TOPIC                 "/arduino_pin/"
#define CKC_SUB_PREFIX_VIRTUAL_TOPIC                 "/virtual_pin/"
#define CKC_SUB_PREFIX_ASK_WIFI_TOPIC                "/wifi/ask"
#define CKC_SUB_PREFIX_CHANGE_WIFI_TOPIC             "/wifi/change"

/* Publish topic */
#define CKC_PUB_PREFIX_INFO_TOPIC               "telemetry"
#define CKC_PUB_PREFIX_MODBUS_DATA_TOPIC        "/data/"



#endif // INC_CKC_TOPIC
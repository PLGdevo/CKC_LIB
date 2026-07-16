#include "AIoT/CKC_handler.h"
#include <cstring>
#include <Arduino.h>
// ===== Default fallback =====
void CKC_WidgetWrite_Default(uint8_t pin, const CKCParam &param)
{
    // Bạn có thể log ở đây
    // printf("No handler for V%d\n", pin);
}

// ======================================================
// ===== TỰ SINH WEAK HANDLER =====
// ======================================================
#define X(n)                                                                          \
    void __attribute__((weak)) CKC_WidgetWrite##n(uint8_t pin, const CKCParam &param) \
    {                                                                                 \
        CKC_WidgetWrite_Default(pin, param);                                          \
    }

CKC_VPIN_LIST
#undef X

// ======================================================
// ===== TẠO VECTOR HANDLER =====
// ======================================================
#define X(n) CKC_WidgetWrite##n,

const handlerWidget_pin CKC_HandlerVector[] = {
    CKC_VPIN_LIST};

#undef X

// ===== COUNT =====
const uint8_t CKC_HandlerCount =
    sizeof(CKC_HandlerVector) / sizeof(CKC_HandlerVector[0]);

//==========================================================//
CKC_HandlerEntry CKC_DataHandler::handlers[50];
uint8_t CKC_DataHandler::count = 0;

void CKC_DataHandler::add(
    const char *key,
    handlerWidget_key cb)
{
    if (count >= 50)
        return;

    handlers[count].key = key;
    handlers[count].handler = cb;
    count++;
}

void CKC_DataHandler::dispatch(
    const char *key,
    const CKCParam &param)
{
    for (uint8_t i = 0; i < count; i++)
    {
        if (strcmp(
                handlers[i].key,
                key) == 0)
        {
            handlers[i].handler(param);
            return;
        }
    }
    // Serial.printf("[CKC] No handler for %s\n", key);    
    CKC_LOG_HANDLER_FAIL("WRITE_PIN","CKC_WRITE(...?) NO KEY : %s",key);   
    // Serial.printf("dispatch=%s count=%d heap=%u\n", key, count, ESP.getFreeHeap());
}

#include "CkC/CKC_handler.hpp"

// ===== Default fallback =====
void CKC_WidgetWrite_Default(uint8_t pin, const CKCParam& param)
{
    // Bạn có thể log ở đây
    // printf("No handler for V%d\n", pin);
}

// ======================================================
// ===== TỰ SINH WEAK HANDLER =====
// ======================================================
#define X(n) \
void __attribute__((weak)) CKC_WidgetWrite##n(uint8_t pin, const CKCParam& param) { \
    CKC_WidgetWrite_Default(pin, param); \
}

CKC_VPIN_LIST
#undef X

// ======================================================
// ===== TẠO VECTOR HANDLER =====
// ======================================================
#define X(n) CKC_WidgetWrite##n,

const handlerWidget_pin CKC_HandlerVector[] = {
    CKC_VPIN_LIST
};

#undef X

// ===== COUNT =====
const uint8_t CKC_HandlerCount =
    sizeof(CKC_HandlerVector) / sizeof(CKC_HandlerVector[0]);


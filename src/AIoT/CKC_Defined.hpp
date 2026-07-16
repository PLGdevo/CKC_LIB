#ifndef INC_CKC_DEFINE_DEBUG
#define INC_CKC_DEFINE_DEBUG

#if defined(CKC_DEBUG)
#define DEBUG
#define DEBUG_HANDLER
#elif defined (CKC_DEBUG_COLOR)
#define DEBUG_COLOR
#define DEBUG_HANDLER_COLOR
#endif

#if defined(CKC_UPDATE_OTA)
#define OTA
#endif

#endif
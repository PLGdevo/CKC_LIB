<<<<<<< HEAD

#if defined(ARDUINO) && defined(ESP32)||defined(ESP8266)
    #if defined(CKC_DEBUG)
=======
#if defined(CKC_DEBUG)
    #if defined(ARDUINO) && defined(ESP32)||defined(ESP8266)
>>>>>>> 51e16ddc631f3760002cbcc7aa6247e66d06a3f3
        #define LOG_BASE(level, tag, format, ...)                 \
            Serial.printf("[%s][%s:%d] %s()[%s]: " format "\n",   \
                        level, CKCFileName(__FILE__), __LINE__, \
                        __FUNCTION__, tag, ##__VA_ARGS__)

        #define CKC_LOG_ERROR(tag, format, ...) LOG_BASE("ERROR", tag, format, ##__VA_ARGS__)
        #define CKC_LOG_INFO(tag, format, ...)  LOG_BASE("INFO", tag, format, ##__VA_ARGS__)
        #define CKC_LOG_DEBUG(tag, format, ...) LOG_BASE("DEBUG", tag, format, ##__VA_ARGS__)
        #define CKC_LOG_WARN(tag, format, ...)  LOG_BASE("WARNING", tag, format, ##__VA_ARGS__)

    #else

        #define CKC_LOG_ERROR(tag, format, ...)
        #define CKC_LOG_INFO(tag, format, ...)
        #define CKC_LOG_DEBUG(tag, format, ...)
        #define CKC_LOG_WARN(tag, format, ...)
    #endif    

#endif

static inline const char *CKCFileName(const char *path)
{
    const char *file = path;
    while (*path)
    {
        if (*path == '/' || *path == '\\')
        {
            file = path + 1;
        }
        path++;
    }
    return file;
}

//     #if defined(CKC_DEBUG_COLOR)
//         #define CKC_LOG_RED                             "\033[0;31m"
//         #define CKC_LOG_GREEN                           "\033[0;32m"
//         #define CKC_LOG_YELLOW                          "\033[0;33m"
//         #define CKC_LOG_BLUE                            "\033[0;34m"
//         #define CKC_LOG_RESET                           "\033[0m"
//         #pragma message "Debugging with color support is only available in VSCode!!!"
//     #else
//         #define CKC_LOG_RED
//         #define CKC_LOG_GREEN
//         #define CKC_LOG_YELLOW
//         #define CKC_LOG_BLUE
//         #define CKC_LOG_RESET
//     #endif

//     #if defined(CKC_DEBUG_SECRET)
//         #define CKC_LOG_SECRET_BEGIN                    "\033[5m"
//         #define CKC_LOG_SECRET_END                      "\033[6m"
//         #pragma message "Debugging secret support is only available in VSCode!!!"
//     #else
//         #define CKC_LOG_SECRET_BEGIN
//         #define CKC_LOG_SECRET_END
//     #endif

//     #define CKC_LOG_SECRET(s)                           CKC_LOG_SECRET_BEGIN s CKC_LOG_SECRET_END

//     // #include <ERa/ERaDefine.hpp>

//     #if defined(ARDUINO) && defined(ESP32) &&   \
//         (CORE_DEBUG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO)
//         #include <esp32-hal-log.h>

//         #define CKC_LOG_COLOR(COLOR, tag, format, ...)  ESP_LOGI(tag, CKC_LOG_ ##COLOR format CKC_LOG_RESET, ##__VA_ARGS__)
//         #define CKC_LOG(tag, format, ...)               CKC_LOG_COLOR(GREEN, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_ERROR(tag, format, ...)         CKC_LOG_COLOR(RED, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_WARNING(tag, format, ...)       CKC_LOG_COLOR(YELLOW, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_DEBUG(tag, format, ...)         CKC_LOG_COLOR(BLUE, tag, format, ##__VA_ARGS__)
//     #elif defined(ARDUINO) &&                   \
//         !defined(__MBED__) &&                   \
//         (defined(ESP32) || defined(ESP8266) ||  \
//         defined(ARDUINO_ARCH_STM32) ||          \
//         defined(ARDUINO_ARCH_RP2040))
//         #ifndef CKC_SERIAL
//             #define CKC_SERIAL                          Serial
//         #endif

//         #define CKC_LOG_TIME()                          CKCPrintf(CKC_PSTR("[%6u]"), ERaMillis())
//         #define CKC_LOG_TAG(tag)                        CKCPrintf(CKC_PSTR("[%s] "), tag)
//         #define CKC_LOG_FN()                            CKCPrintf(CKC_PSTR("[%s:%d] %s(): "), ERaFileName(__FILE__), __LINE__, __FUNCTION__)
//         #define CKC_LOG_COLOR(COLOR, tag, format, ...)  { CKC_LOG_TIME(); CKC_LOG_FN(); CKC_LOG_TAG(tag); CKCPrintf(CKC_LOG_ ##COLOR format CKC_LOG_RESET, ##__VA_ARGS__); CKC_SERIAL.println(); }
//         #define CKC_LOG(tag, format, ...)               CKC_LOG_COLOR(GREEN, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_ERROR(tag, format, ...)         CKC_LOG_COLOR(RED, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_WARNING(tag, format, ...)       CKC_LOG_COLOR(YELLOW, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_DEBUG(tag, format, ...)         CKC_LOG_COLOR(BLUE, tag, format, ##__VA_ARGS__)

//         static inline
//         CKC_UNUSED const char* CKCFileName(const char* path) {
//             size_t i = 0;
//             size_t pos = 0;
//             char* p = (char*)path;
//             while (*p) {
//                 i++;
//                 if(*p == '/' || *p == '\\'){
//                     pos = i;
//                 }
//                 p++;
//             }
//             return path + pos;
//         }
//     #elif defined(ARDUINO) &&                   \
//         (defined(__AVR__) ||                    \
//         defined(__MBED__) ||                    \
//         defined(RTL8722DM) ||                   \
//         defined(ARDUINO_AMEBA) ||               \
//         defined(ARDUINO_ARCH_AVR) ||            \
//         defined(ARDUINO_ARCH_SAM) ||            \
//         defined(ARDUINO_ARCH_SAMD) ||           \
//         defined(ARDUINO_ARCH_RENESAS) ||        \
//         defined(ARDUINO_ARCH_ARM) ||            \
//         defined(ARDUINO_ARCH_OPENCPU) ||        \
//         defined(ARDUINO_ARCH_ARC32))
//         #ifndef CKC_SERIAL
//             #define CKC_SERIAL                          Serial
//         #endif

//         #define CKC_LOG_TIME()                          CKCPrintf(CKC_PSTR("[%6u]"), ERaMillis())
//         #define CKC_LOG_TAG(tag)                        CKCPrintf(CKC_PSTR("[%s] "), tag)
//         #define CKC_LOG_FN()                            CKCPrintf(CKC_PSTR("[%s:%d] %s(): "), ERaFileName(__FILE__), __LINE__, __FUNCTION__)
//         #define CKC_LOG_COLOR(COLOR, tag, format, ...)  { CKC_LOG_TIME(); CKC_LOG_FN(); CKC_LOG_TAG(tag); CKCPrintf(CKC_LOG_ ##COLOR format CKC_LOG_RESET, ##__VA_ARGS__); CKC_SERIAL.println(); }
//         #define CKC_LOG(tag, format, ...)               CKC_LOG_COLOR(GREEN, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_ERROR(tag, format, ...)         CKC_LOG_COLOR(RED, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_WARNING(tag, format, ...)       CKC_LOG_COLOR(YELLOW, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_DEBUG(tag, format, ...)         CKC_LOG_COLOR(BLUE, tag, format, ##__VA_ARGS__)

//         static inline
//         CKC_UNUSED const char* CKCFileName(const char* path) {
//             size_t i = 0;
//             size_t pos = 0;
//             char* p = (char*)path;
//             while (*p) {
//                 i++;
//                 if(*p == '/' || *p == '\\'){
//                     pos = i;
//                 }
//                 p++;
//             }
//             return path + pos;
//         }

//         #include <stdio.h>
//         #include <stdarg.h>

//         static inline
//         CKC_UNUSED size_t CKCPrintf(const char* CKC_PROGMEM format, ...) {
//             va_list arg;
//             va_list copy;
//             va_start(arg, format);
//             va_copy(copy, arg);
//             char locBuf[128] {0};
//             char* buf = locBuf;
//             int len = vsnprintfp(buf, sizeof(locBuf), format, copy);
//             va_end(copy);
//             if (len < 0) {
//                 va_end(arg);
//                 return 0;
//             }
//             if (len >= (int)sizeof(locBuf)) {
//                 buf = (char*)malloc(len + 1);
//                 if (buf == nullptr) {
//                     va_end(arg);
//                     return 0;
//                 }
//                 len = vsnprintfp(buf, len + 1, format, arg);
//             }
//             va_end(arg);
//             CKC_SERIAL.print(buf);
//             if (buf != locBuf) {
//                 free(buf);
//             }
//             buf = nullptr;
//             return len;
//         }
//     #elif defined(LINUX)
//         #ifndef CKC_SERIAL
//             #define CKC_SERIAL                          stdout
//         #endif

//         #if defined(WIRING_PI)
//             #include <wiringPi.h>
//         #endif

//         #include <iostream>

//         using namespace std;

//         #define CKC_LOG_TIME()                          cout << '[' << CKCMillis() << ']'
//         #define CKC_LOG_TAG(tag)                        cout << '[' << tag << "] "
//         #define CKC_LOG_FN()                            cout << '[' << CKCFileName(__FILE__) << ':' << __LINE__ << "] " << __FUNCTION__ << "(): "
//         #define CKC_LOG_COLOR(COLOR, tag, format, ...)  { CKC_LOG_TIME(); CKC_LOG_FN(); CKC_LOG_TAG(tag); fprintf(CKC_SERIAL, CKC_LOG_ ##COLOR format CKC_LOG_RESET ERA_NEWLINE, ##__VA_ARGS__); }
//         #define CKC_LOG(tag, format, ...)               CKC_LOG_COLOR(GREEN, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_ERROR(tag, format, ...)         CKC_LOG_COLOR(RED, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_WARNING(tag, format, ...)       CKC_LOG_COLOR(YELLOW, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_DEBUG(tag, format, ...)         CKC_LOG_COLOR(BLUE, tag, format, ##__VA_ARGS__)

//         static inline
//         CKC_UNUSED const char* CKCFileName(const char* path) {
//             size_t i = 0;
//             size_t pos = 0;
//             char* p = (char*)path;
//             while (*p) {
//                 i++;
//                 if(*p == '/' || *p == '\\'){
//                     pos = i;
//                 }
//                 p++;
//             }
//             return path + pos;
//         }
//     #else
//         #ifndef CKC_SERIAL
//             #define CKC_SERIAL                          Serial
//         #endif

//         #define CKC_LOG_TIME()                          CKC_SERIAL.printfp(CKC_PSTR("[%6u]"), CKCMillis())
//         #define CKC_LOG_TAG(tag)                        CKC_SERIAL.printfp(CKC_PSTR("[%s] "), tag)
//         #define CKC_LOG_FN()                            CKC_SERIAL.printfp(CKC_PSTR("[%s:%d] %s(): "), CKCFileName(__FILE__), __LINE__, __FUNCTION__)
//         #define CKC_LOG_COLOR(COLOR, tag, format, ...)  { CKC_LOG_TIME(); CKC_LOG_FN(); CKC_LOG_TAG(tag); CKC_SERIAL.printfp(CKC_LOG_ ##COLOR format CKC_LOG_RESET, ##__VA_ARGS__); ERA_SERIAL.println(); }
//         #define CKC_LOG(tag, format, ...)               CKC_LOG_COLOR(GREEN, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_ERROR(tag, format, ...)         CKC_LOG_COLOR(RED, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_WARNING(tag, format, ...)       CKC_LOG_COLOR(YELLOW, tag, format, ##__VA_ARGS__)
//         #define CKC_LOG_DEBUG(tag, format, ...)         CKC_LOG_COLOR(BLUE, tag, format, ##__VA_ARGS__)

//         static inline
//         CKC_UNUSED const char* CKCFileName(const char* path) {
//             size_t i = 0;
//             size_t pos = 0;
//             char* p = (char*)path;
//             while (*p) {
//                 i++;
//                 if(*p == '/' || *p == '\\'){
//                     pos = i;
//                 }
//                 p++;
//             }
//             return path + pos;
//         }
//     #endif

//     #if defined(CKC_SERIAL) &&              \
//         (defined(ARDUINO) || defined(LINUX))
//         // #include <ERa/ERaDebugHelper.hpp>
//         #define CKC_PRINT(...)                          LogHelper::instance().print(__VA_ARGS__)
//     #else
//         #define CKC_PRINT(...)                          do {} while(0)
//     #endif

//     #pragma message "Debug enabled"
//     #if defined(ARDUINO) && defined(ESP32)
//         #if (CORE_DEBUG_LEVEL < ARDUHAL_LOG_LEVEL_INFO)
//             #pragma message "Recommend: Setting the Core Debug Level to Info (3)"
//         #endif
//         #if defined(BOARD_HAS_PSRAM)
//             #pragma message "PSRAM enabled"
//         #endif
//     #endif
// #else
//     #undef CKC_LOG
//     #undef CKC_LOG_ERROR
//     #undef CKC_LOG_WARNING
//     #undef CKC_LOG_DEBUG
//     #undef CKC_PRINT
//     #undef CKC_DEBUG_DUMP
//     #define CKC_LOG(...)            do {} while(0)
//     #define CKC_LOG_ERROR(...)      do {} while(0)
//     #define CKC_LOG_WARNING(...)    do {} while(0)
//     #define CKC_LOG_DEBUG(...)      do {} while(0)
//     #define CKC_PRINT(...)          do {} while(0)

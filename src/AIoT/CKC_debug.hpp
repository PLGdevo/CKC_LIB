#ifndef INC_CKC_DEBUG
#define INC_CKC_DEBUG

#if defined(ARDUINO) && defined(ESP32) || defined(ESP8266)

    #if defined(DEBUG_COLOR)    
    // ======================================================
    //                        COLOR
    // ======================================================

    #define LOG_BLACK "\033[30m"
    #define LOG_RED "\033[31m"
    #define LOG_GREEN "\033[32m"
    #define LOG_YELLOW "\033[33m"
    #define LOG_BLUE "\033[34m"
    #define LOG_MAGENTA "\033[35m"
    #define LOG_CYAN "\033[36m"
    #define LOG_WHITE "\033[37m"

    #define LOG_BOLD "\033[1m"
    #define LOG_RESET "\033[0m"

    // ======================================================
    //                  FILE NAME EXTRACT
    // ======================================================

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

    // ======================================================
    //                    CORE ID ESP32
    // ======================================================

    #if defined(ESP32)
    #define CKC_CORE_ID xPortGetCoreID()
    #else
    #define CKC_CORE_ID 0
    #endif

    // ======================================================
    //                    LOG SYSTEM
    // ======================================================

    #define LOG_BASE(headColor, msgColor, level, tag, format, ...) \
        do                                                         \
        {                                                          \
            Serial.printf(                                         \
                headColor LOG_BOLD                                 \
                "[%10lu ms]"                                       \
                "[CORE %d]"                                        \
                "[%s]"                                             \
                "[%s:%d]"                                          \
                " %s()"                                            \
                "[%s]: " msgColor                                  \
                    format LOG_RESET "\n",                         \
                millis(),                                          \
                CKC_CORE_ID,                                       \
                level,                                             \
                CKCFileName(__FILE__),                             \
                __LINE__,                                          \
                __FUNCTION__,                                      \
                tag,                                               \
                ##__VA_ARGS__);                                    \
        } while (0)

    // ======================================================
    //                    NORMAL LOG
    // ======================================================

    #define CKC_LOG_ERROR(tag, format, ...) \
        LOG_BASE(LOG_RED, LOG_WHITE, "ERROR", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_INFO(tag, format, ...) \
        LOG_BASE(LOG_GREEN, LOG_WHITE, "INFO", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_WARN(tag, format, ...) \
        LOG_BASE(LOG_YELLOW, LOG_WHITE, "WARN", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_DEBUG(tag, format, ...) \
        LOG_BASE(LOG_CYAN, LOG_WHITE, "DEBUG", tag, format, ##__VA_ARGS__)

    // ======================================================
    //                    SPECIAL LOG
    // ======================================================

    #define CKC_LOG_WIFI(tag, format, ...) \
        LOG_BASE(LOG_BLUE, LOG_WHITE, "📶 WIFI", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_MQTT(tag, format, ...) \
        LOG_BASE(LOG_MAGENTA, LOG_WHITE, "📡 MQTT", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_UART(tag, format, ...) \
        LOG_BASE(LOG_CYAN, LOG_WHITE, "🛰 UART", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_OK(tag, format, ...) \
        LOG_BASE(LOG_GREEN, LOG_GREEN, "✅ OK", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_FAIL(tag, format, ...) \
        LOG_BASE(LOG_RED, LOG_RED, "❌ FAIL", tag, format, ##__VA_ARGS__)

    

    #elif defined(DEBUG)
    
    #define LOG_BASE(level, tag, format, ...)                 \
        Serial.printf("[%s][%s:%d] %s()[%s]: " format "\n",   \
                    level, CKCFileName(__FILE__), __LINE__, \
                    __FUNCTION__, tag, ##__VA_ARGS__)

    #define CKC_LOG_ERROR(tag, format, ...) LOG_BASE("ERROR", tag, format, ##__VA_ARGS__)
    #define CKC_LOG_INFO(tag, format, ...) LOG_BASE("INFO", tag, format, ##__VA_ARGS__)
    #define CKC_LOG_DEBUG(tag, format, ...) LOG_BASE("DEBUG", tag, format, ##__VA_ARGS__)
    #define CKC_LOG_WARN(tag, format, ...) LOG_BASE("WARNING", tag, format, ##__VA_ARGS__)

    // ======================================================
    //                    SPECIAL LOG
    // ======================================================

    #define CKC_LOG_WIFI(tag, format, ...) \
        LOG_BASE("📶 WIFI", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_MQTT(tag, format, ...) \
        LOG_BASE("📡 MQTT", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_UART(tag, format, ...) \
        LOG_BASE("🛰 UART", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_OK(tag, format, ...) \
        LOG_BASE("✅ OK", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_FAIL(tag, format, ...) \
        LOG_BASE("❌ FAIL", tag, format, ##__VA_ARGS__)
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
    #else

    #define CKC_LOG_ERROR(tag, format, ...)
    #define CKC_LOG_INFO(tag, format, ...)
    #define CKC_LOG_DEBUG(tag, format, ...)
    #define CKC_LOG_WARN(tag, format, ...)

    #define CKC_LOG_WIFI(tag, format, ...)
    #define CKC_LOG_MQTT(tag, format, ...)
    #define CKC_LOG_UART(tag, format, ...)

    #define CKC_LOG_OK(tag, format, ...)
    #define CKC_LOG_FAIL(tag, format, ...)   

    #endif
#endif

#if defined(ARDUINO) && defined(ESP32) || defined(ESP8266)
    #if defined(DEBUG_HANDLER_COLOR)    
    // ======================================================
    //                        COLOR
    // ======================================================

    #define LOG_BLACK "\033[30m"
    #define LOG_RED "\033[31m"
    #define LOG_GREEN "\033[32m"
    #define LOG_YELLOW "\033[33m"
    #define LOG_BLUE "\033[34m"
    #define LOG_MAGENTA "\033[35m"
    #define LOG_CYAN "\033[36m"
    #define LOG_WHITE "\033[37m"

    #define LOG_BOLD "\033[1m"
    #define LOG_RESET "\033[0m"  

    // ======================================================
    //                  FILE NAME EXTRACT
    // ======================================================

    static inline const char *CKCFileName_hanler(const char *path)
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
    // ======================================================
    //                    CORE ID ESP32
    // ======================================================

    #if defined(ESP32)
    #define CKC_CORE_ID xPortGetCoreID()
    #else
    #define CKC_CORE_ID 0
    #endif

    // ======================================================
    //                    LOG SYSTEM
    // ======================================================

    #define LOG_BASE_HANDLER(headColor, msgColor, level, tag, format, ...) \
        do                                                         \
        {                                                          \
            Serial.printf(                                         \
                headColor LOG_BOLD                                 \
                "[%10lu ms]"                                       \
                "[CORE %d]"                                        \
                "[%s]"                                             \
                "[%s:%d]"                                          \
                " %s()"                                            \
                "[%s]: " msgColor                                  \
                    format LOG_RESET "\n",                         \
                millis(),                                          \
                CKC_CORE_ID,                                       \
                level,                                             \
                CKCFileName_hanler(__FILE__),                      \
                __LINE__,                                          \
                __FUNCTION__,                                      \
                tag,                                               \
                ##__VA_ARGS__);                                    \
        } while (0)

    #define CKC_LOG_HANDLER_OK(tag, format, ...) \
        LOG_BASE_HANDLER(LOG_GREEN, LOG_GREEN, "✅ OK", tag, format, ##__VA_ARGS__)

    #undef CKC_LOG_HANDLER_FAIL    
    #define CKC_LOG_HANDLER_FAIL(tag, format, ...) \
        LOG_BASE_HANDLER(LOG_RED, LOG_RED, "❌ FAIL", tag, format, ##__VA_ARGS__)

    
    #elif defined(DEBUG_HANDLER)    

    #define LOG_BASE_HANDLER(level, tag, format, ...)                 \
        Serial.printf("[%s][%s:%d] %s()[%s]: " format "\n",   \
                    level, CKC_HANDLER_DEBUG(__FILE__), __LINE__, \
                    __FUNCTION__, tag, ##__VA_ARGS__)

    // ======================================================
    //                    SPECIAL LOG
    // ======================================================

    #undef CKC_LOG_HANDLER_FAIL
    #define CKC_LOG_HANDLER_OK(tag, format, ...) \
        LOG_BASE_HANDLER("✅ OK", tag, format, ##__VA_ARGS__)

    #define CKC_LOG_HANDLER_FAIL(tag, format, ...) \
        LOG_BASE_HANDLER("❌ FAIL", tag, format, ##__VA_ARGS__)
    static inline const char *CKC_HANDLER_DEBUG(const char *path)
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
    #else

    #define CKC_LOG_HANDLER_OK(tag, format, ...)
    #define CKC_LOG_HANDLER_FAIL(tag, format, ...)

    #endif
    
#endif

#define MODBUS_DEBUG
#if defined(ARDUINO) && defined(ESP32) || defined(ESP8266)
#if defined(MODBUS_DEBUG)
#define LOG_BASE0(level, tag, format, ...)              \
    Serial.printf("[%s][%s:%d] %s()[%s]: " format "\n", \
                  level, MODBUS(__FILE__), __LINE__,    \
                  __FUNCTION__, tag, ##__VA_ARGS__)
#define LOG_BASE1(level, tag, format, ...)            \
    Serial.printf("[%s][%s:%d] %s()[%s]: " format "", \
                  level, MODBUS(__FILE__), __LINE__,  \
                  __FUNCTION__, tag, ##__VA_ARGS__)

#define MODBUS_LOG_ERROR(tag, format, ...) LOG_BASE0("ERROR", tag, format, ##__VA_ARGS__)
#define MODBUS_LOG_INFO(tag, format, ...) LOG_BASE0("INFO", tag, format, ##__VA_ARGS__)
#define MODBUS_LOG_DEBUG(tag, format, ...) LOG_BASE0("DEBUG", tag, format, ##__VA_ARGS__)
#define MODBUS_LOG_WARN(tag, format, ...) LOG_BASE0("WARNING", tag, format, ##__VA_ARGS__)

#define MODBUS_CHECK_ERROR(tag, format, ...) LOG_BASE1("ERROR", tag, format, ##__VA_ARGS__)
#define MODBUS_CHECK_INFO(tag, format, ...) LOG_BASE1("INFO", tag, format, ##__VA_ARGS__)
#define MODBUS_CHECK_DEBUG(tag, format, ...) LOG_BASE1("DEBUG", tag, format, ##__VA_ARGS__)
#define MODBUS_CHECK_WARN(tag, format, ...) LOG_BASE1("WARNING", tag, format, ##__VA_ARGS__)

#else

#define MODBUS_LOG_ERROR(tag, format, ...)
#define MODBUS_LOG_INFO(tag, format, ...)
#define MODBUS_LOG_DEBUG(tag, format, ...)
#define MODBUS_LOG_WARN(tag, format, ...)

#define MODBUS_CHECK_ERROR(tag, format, ...)
#define MODBUS_CHECK_INFO(tag, format, ...)
#define MODBUS_CHECK_DEBUG(tag, format, ...)
#define MODBUS_CHECK_WARN(tag, format, ...)

#endif

#endif

static inline const char *MODBUS(const char *path)
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

#endif // INC_CKC_DEBUG
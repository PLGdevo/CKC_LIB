#ifndef INC_CKC_DEBUG
#define INC_CKC_DEBUG

#if defined(ARDUINO) && defined(ESP32) || defined(ESP8266)
#if defined(CKC_DEBUG)
#define LOG_BASE(level, tag, format, ...)                 \
    Serial.printf("[%s][%s:%d] %s()[%s]: " format "\n",   \
                  level, CKCFileName(__FILE__), __LINE__, \
                  __FUNCTION__, tag, ##__VA_ARGS__)

#define CKC_LOG_ERROR(tag, format, ...) LOG_BASE("ERROR", tag, format, ##__VA_ARGS__)
#define CKC_LOG_INFO(tag, format, ...) LOG_BASE("INFO", tag, format, ##__VA_ARGS__)
#define CKC_LOG_DEBUG(tag, format, ...) LOG_BASE("DEBUG", tag, format, ##__VA_ARGS__)
#define CKC_LOG_WARN(tag, format, ...) LOG_BASE("WARNING", tag, format, ##__VA_ARGS__)

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
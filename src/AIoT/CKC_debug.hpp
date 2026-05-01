#if defined(ARDUINO) && defined(ESP32)||defined(ESP8266)
    #if defined(CKC_DEBUG)
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

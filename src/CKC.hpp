#ifndef INC_CKC_HPP
#define INC_CKC_HPP


#if defined(ARDUINO) && defined(ESP32)
    #if defined(ERA_USE_SSL)
        // #include <ERaSimpleMBEsp32SSL.hpp>
    #else
        #include <ESP32_simple.hpp>
    #endif
#elif defined(ARDUINO) && defined(ESP8266)
    #if defined(ERA_USE_SSL)
        // #include <ERaEsp8266SSL.hpp>
    #else
        // #include <ERaEsp8266.hpp>
    #endif
#elif defined(ARDUINO)&& defined()


#endif 
#endif/*INC_CKC_HPP*/
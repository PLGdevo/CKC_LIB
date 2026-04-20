#ifndef CKC_ESP32_MQTT_HPP
#define CKC_ESP32_MQTT_HPP

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <MQTT/NPT_Client/NTPClient.h>
#include <MQTT/PubSubClient/PubSubClient.h>
#include <stdint.h>
#include <CkC/CKC_Param.hpp>
#include "CkC/CKC_API.hpp"

// #define CKC_MQTT_BASE_TOPIC CKC_BASE_TOPIC

template <class MQTT>
class CKC_MQTT
{
public:
    void begin();
    void run();
    void sendData(String Topic_s, String Data);
    void receiveData(String Topic_r);
    bool _connect();
    void CKC_subscribeTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_unsubscribeTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_publishTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_unpublishTopic(const char *baseTopic, const char *Topic_ne);
    void CKC_publishData(String data);
    bool check_mode_sub(char *topic, char *mess);

private:
    // const char *MQTT_Server = "172.16.1.252";
    // const int16_t MQTT_PORT = 8883;
    // const char *MQTT_ID = "emqx_MDI3NY";
    // const char *MQTT_USERNAME = "";
    // const char *MQTT_PASS = "";

    const char *MQTT_Server = "190d175e0d5b47c9bb68f15b675c839e.s1.eu.hivemq.cloud";
    const int16_t MQTT_PORT = 8883;
    const char *MQTT_ID = "190d175e0d5b47c9bb68f15b675c839e";
    const char *MQTT_USERNAME = "hivemq.webclient.1775916076788";
    const char *MQTT_PASS = "<>Kmju@2thY!13IAw8TB";

    char CKC_MQTT_BASE_TOPIC[30] = CKC_BASE_TOPIC;
    char _mac[12];

    
};

WiFiClientSecure server;
PubSubClient mqttClient(server);
CKC_MQTT<PubSubClient> mqtt;

void CKC_Callback(char *topic, byte *payload, unsigned int length)
{
    char *msg = (char *)malloc(length + 1);
    if (!msg)
        return;
    memcpy(msg, payload, length);
    msg[length] = '\0';
    API_MESS.handleMessage(topic, msg);
    free(msg);
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_subscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.subscribe(NameTopic);
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_unsubscribeTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
    mqttClient.unsubscribe(NameTopic);
}

/* Publish Topic */
template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_publishTopic(const char *baseTopic, const char *Topic_ne)
{
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", baseTopic, Topic_ne);
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_unpublishTopic(const char *baseTopic, const char *Topic_ne)
{
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::CKC_publishData(String data)
{
    char NameTopic[100];    
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", CKC_BASE_TOPIC, CKC_PUB_PREFIX_INFO_TOPIC); 
    mqttClient.publish(NameTopic, data.c_str());
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::begin()
{
    server.setInsecure();
    mqttClient.setServer(MQTT_Server, MQTT_PORT);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    CKC_LOG_DEBUG("MQTT", "Connecting_________");
    if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        if (mqttClient.connect(MQTT_ID))
        {
            CKC_LOG_DEBUG("MQTT", "OK");
            mqttClient.setCallback(CKC_Callback);            
            snprintf(CKC_MQTT_BASE_TOPIC, sizeof(CKC_MQTT_BASE_TOPIC), "%s%s", CKC_MQTT_BASE_TOPIC, _mac);
            this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_TELEMETRY_TOPIC);
            this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_ARDUINO_TOPIC);
            this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_VIRTUAL_TOPIC);
            this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_ASK_WIFI_TOPIC);
            this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_CHANGE_WIFI_TOPIC);
        }
        else
        {
            CKC_LOG_DEBUG("MQTT", "FAILED, rc=", mqttClient.state());
        }
}

template <class MQTT>
inline bool CKC_MQTT<MQTT>::_connect()
{
    if (mqttClient.connected())
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::run()
{
    if (this->_connect())
    {
        mqttClient.loop();
        if (!mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        {
            Serial.println("FAILED");
            this->begin();
            return;
        }
    }
    static unsigned long PLGTimer = 0;
    if (millis() - PLGTimer > 2000)
    {
        PLGTimer = millis();
             
    }
}

template <class MQTT>
inline void CKC_MQTT<MQTT>::sendData(String Topic_s, String Data)
{
    if (this->_connect())
    {
        Serial.print("[CKC] MQTT reconnect...");

        // if (!mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        if (!mqttClient.connect(MQTT_ID))
        {
            Serial.println("FAILED");
            this->begin();
            return;
        }
        Serial.println("OK");
    }
    mqttClient.publish(Topic_s.c_str(), Data.c_str());
    Serial.println("[CKC] Sent: " + Topic_s + " -> " + Data);
}

CKC_MQTT<PubSubClient> serverMQTT;

#endif // END CKC_ESP32_MQTT_HPP
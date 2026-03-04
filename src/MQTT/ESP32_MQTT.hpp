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

const char *MQTT_Server = "6937adf7e70d48879245c5a2b4299e63.s1.eu.hivemq.cloud";
const int16_t MQTT_PORT = 8883;
const char *MQTT_ID = "6937adf7e70d48879245c5a2b4299e63";
const char *MQTT_USERNAME = "hivemq.webclient.1772521107497";
const char *MQTT_PASS = "0eQLc1F,<bG>SX%ns34r";

#define CKC_MQTT_BASE_TOPIC CKC_BASE_TOPIC PLGtoken
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
    void CKC_publishData(const char *data);
    bool check_mode_sub(char *topic, char *mess);

private:
};
// template <class MQTT>
// inline
// bool CKC_MQTT<MQTT>::check_mode_sub(char *topic,char *mess)
// {

// }
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (int i = 0; i < length; i++)
        msg += (char)payload[i];
    CKC_LOG_DEBUG("SUB_MQTT", "Topic %s  mess: %s", topic, msg);
}

WiFiClientSecure server;
PubSubClient mqttClient(server);
/* subscribeTopic */
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
inline void CKC_MQTT<MQTT>::CKC_publishData(const char *data)
{
    // this ->CKC_publishTopic(CKC_MQTT_BASE_TOPIC,CKC_PUB_PREFIX_INFO_TOPIC);
    char NameTopic[100];
    snprintf(NameTopic, sizeof(NameTopic), "%s%s", CKC_MQTT_BASE_TOPIC, CKC_PUB_PREFIX_INFO_TOPIC);
    mqttClient.publish(NameTopic, data);
}
template <class MQTT>
inline void CKC_MQTT<MQTT>::begin()
{
    server.setInsecure();
    mqttClient.setServer(MQTT_Server, MQTT_PORT);
    Serial.print("[CKC] MQTT connecting...");
    if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
    {
        Serial.println("OK");
        mqttClient.setCallback(mqttCallback);
        this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_DOWN_TOPIC);
        this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_ARDUINO_TOPIC);
        this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_VIRTUAL_TOPIC);
        this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_ASK_WIFI_TOPIC);
        this->CKC_subscribeTopic(CKC_MQTT_BASE_TOPIC, CKC_SUB_PREFIX_CHANGE_WIFI_TOPIC);
    }
    else
    {
        Serial.print("FAILED, rc=");
        Serial.println(mqttClient.state());
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
    }
    static unsigned long PLGTimer = 0;
    if (millis() - PLGTimer > 2000)
    {
        PLGTimer = millis();
        this->CKC_publishData("dasdasd");
    }
}
template <class MQTT>
inline void CKC_MQTT<MQTT>::sendData(String Topic_s, String Data)
{
    // if (this->_connect())
    // {
    //     Serial.print("[CKC] MQTT reconnect...");

    //     if (!mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
    //     {
    //         Serial.println("FAILED");
    //         this->begin();
    //         return;
    //     }
    //     Serial.println("OK");
    // }
    // mqttClient.publish(Topic_s.c_str(), Data.c_str());
    // Serial.println("[CKC] Sent: " + Topic_s + " -> " + Data);
}

CKC_MQTT<PubSubClient> serverMQTT;
#endif // END CKC_ESP32_MQTT_HPP
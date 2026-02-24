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

const char *MQTT_Server = "5a2457df7f6441bb855f4fa5876508a3.s1.eu.hivemq.cloud";
const int16_t MQTT_PORT = 8883;

const char *MQTT_ID = "5a2457df7f6441bb855f4fa5876508a3";
const char *MQTT_USERNAME = "hivemq.webclient.1771340173854";
const char *MQTT_PASS = "RSuYakI,H$nj1c9.C!08";

class CKC_MQTT
{
public:
    void begin();
    void run();
    void sendData(String Topic_s, String Data);
    void receiveData(String Topic_r);

private:    
    String Data_receive;
};
void mqttCallback(char *topic, byte *payload, unsigned int length)
{
    String msg;
    for (int i = 0; i < length; i++)
        msg += (char)payload[i];
    Serial.print("[MQTT] ");
    Serial.print(topic);
    Serial.print(" => ");
    Serial.println(msg);

    if (String(topic) == "led/2")
    {
        int value = msg.toInt(); // "1" / "0"
    }
}

WiFiClientSecure server;
PubSubClient mqttClient(server);

void CKC_MQTT::begin() // Hàm cấu hình Wifi và set up MQTT Server
{
    server.setInsecure();                         // hàm cài đặt bỏ qua bước xác thực
    mqttClient.setServer(MQTT_Server, MQTT_PORT); // set up MQTT Server với link Server và cổng PORT
    Serial.print("[CKC] MQTT connecting...");
    if (mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
    {
        Serial.println("OK");
        mqttClient.setCallback(mqttCallback);
    }
    else
    {
        Serial.print("FAILED, rc=");
        Serial.println(mqttClient.state());
    }
}

void CKC_MQTT::run() // hàm hoạt động của MQTT
{
    if (mqttClient.connected())
        mqttClient.loop();        
}

void CKC_MQTT::sendData(String Topic_s, String Data) // gửi dữ liệu kèm theo TOPIC đến MQTT Server, đồng thời kiểm tra kết nỗi trước khi gửi
{
    if (!mqttClient.connected())
    {
        Serial.print("[CKC] MQTT reconnect...");

        if (!mqttClient.connect(MQTT_ID, MQTT_USERNAME, MQTT_PASS))
        {
            Serial.println("FAILED");
            CKC_MQTT:begin();
            return;
        }
        Serial.println("OK");
    }
    mqttClient.publish(Topic_s.c_str(), Data.c_str());
    Serial.println("[CKC] Sent: " + Topic_s + " -> " + Data);
}

#endif // END CKC_ESP32_MQTT_HPP
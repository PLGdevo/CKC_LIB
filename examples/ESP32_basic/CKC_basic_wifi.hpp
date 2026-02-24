#include <Arduino.h>

// #define CKC_DEBUG
#define BUTTON_MODE
const char *SSID = "Phuoc Le 2.4G";
const char *PASS = "0909028573";
#include <CKC.h>


void setup()
{
  Serial.begin(115200);
  pinMode(26, OUTPUT);
  CKC.init(SSID, PASS);
}
void loop()
{
  CKC.run();
  if (CKC.CkC_Connected())
  {
    digitalWrite(26, 1);
  }
  else
  {
    digitalWrite(26, 0);
  }
  mqttClient.subscribe("PLG");
  static unsigned long sendTimer = 0;
  if (millis() - sendTimer > 3000)
  {
    sendTimer = millis();
    MQTT.sendData("NHIET_DO", "PLG");
    MQTT.sendData("DO_AM", "UPDATE");
  }
}

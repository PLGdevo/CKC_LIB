#include <CKC_ESP32_WiFi.hpp>
CKC CKC_IoT;
void setup()
{
  Serial.begin(115200);
  CKC_IoT.begin("PLG", "admin123");
  delay(20000);
}
void loop()
{
  static unsigned long sendTimer = 0;
  if (millis() - sendTimer > 3000)
  {
    sendTimer = millis();
    CKC_IoT.sendDATA("1", "2", "3", "4", "5");
  }
}
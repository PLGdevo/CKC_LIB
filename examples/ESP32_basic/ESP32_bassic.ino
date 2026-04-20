#include <Arduino.h>
#define CKC_DEBUG
#define BUTTON_MODE

const char *SSID = "CKC";
const char *PASS = "CKC2026";
#include <CKC.h>
CKC_WRITE(V3)
{
  int a = param.getInt();
  CKC_LOG_DEBUG("CKC", "value%d", a);
  CKC.WriteControl(V3, a);
}
CKC_WRITE(V2)
{
  int a = param.getFloat();
  CKC_LOG_DEBUG("CKC1", "value%f", a);
  CKC.WriteControl(V2, a);
}
void setup()
{
  Serial.begin(115200);
  pinMode(26, OUTPUT);
  CKC.begin(SSID, PASS);
}
void loop()
{
  CKC.run();
  if (CKC.connected())
  {
    digitalWrite(26, 1);
  }
  else
  {
    digitalWrite(26, 0);
  }
}

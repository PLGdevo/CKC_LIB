#include <Arduino.h>
#define CKC_DEBUG
#define BUTTON_MODE

const char *SSID = "CKC";
const char *PASS = "CKC2026";
#include <CKC.h>
CKC_WRITE(V3)
{
   int a = param.getInt();
   CKC_LOG_DEBUG("CKC","value%d",a);
}
CKC_WRITE(V2)
{
   int a = param.getFloat();
   CKC_LOG_DEBUG("CKC1","value%f",a);
}
void setup()
{
  Serial.begin(115200);
  pinMode(26, OUTPUT);
  CKC.init(SSID, PASS);  
}
void loop()
{
  CKC.run();  
  if (CKC.CkC_Connected())  {digitalWrite(26, 1);}
  else                       {digitalWrite(26, 0);}
  
}


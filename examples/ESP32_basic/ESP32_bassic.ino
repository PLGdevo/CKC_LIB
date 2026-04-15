
/*************************************************************
  Download latest CKC_lib here:
      https://github.com/PLGdevo/CKC_LIB.git

    CKC Website:                https://ckc.kthd.vn/login
    Documentation:              https://your-docs.com
    Community/Forum:            https://your-forum.com
    Follow us:                  https://www.fb.com/your-page

 *************************************************************/

/*
 * CKC_lib - IoT Communication & Control Library
 * 
 * Copyright (c) 2026 CKC
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies.
 *
 * Conditions:
 *  - The above copyright notice must be included in all copies.
 *  - Do not misrepresent the origin of this software.
 *
 * Disclaimer:
 *  This software is provided "as is", without warranty of any kind.
 *  CKC shall not be liable for any damages arising from its use.
 */


// Enable debug console
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
  if (millis() - time_P > 1000)
  {
    time_P = millis();
    float value = (sin(t) + 1) * 10 + 50;
    t += 0.1;
    CKC.WriteTelemetry(V10, temp);
  }
  
}


#ifndef CKC_OTA_H
#define CKC_OTA_H

/*************************************************************
  Download latest CKC_lib here:
      https://github.com/PLGdevo/CKC_LIB.git

    CKC Website:                https://ait.caothang.edu.vn/login
    Documentation:              https://your-docs.com
    Community/Forum:            https://your-forum.com
    Follow us:                  https://www.fb.com/your-page

  ===========================================================
  DEFAULT LOGIN CREDENTIALS / TÀI KHOẢN ĐĂNG NHẬP MẶC ĐỊNH
    - http://ckc_ota.local
    - Username: admin
    - Password: 123456
  ===========================================================
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

#include <Arduino.h>
#include <WebServer.h>
#include <Update.h>
#include <ESPmDNS.h>
#include<OTA/ota_UI.h>

class CKC_OTA
{
public:

    void begin(WebServer &server);
    void stop();

private:

    WebServer *_server;

    void setupRoute();

    void handleRoot();

    void handleUpdate();

    void handleInfo();
};

#endif
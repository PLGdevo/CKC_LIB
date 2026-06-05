
/*************************************************************
  Download latest CKC_lib here:
      https://github.com/PLGdevo/CKC_LIB.git

    CKC Website:                https://ait.caothang.edu.vn/login
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


// Bật debug log ra Serial Monitor
#define CKC_DEBUG

// Bật chế độ nút nhấn (tuỳ thư viện bạn xử lý)
#define BUTTON_MODE

// Thông tin WiFi 
const char *SSID = "CKC";
const char *PASS = "CKC2026";

// thông tin bao mật người dùng từ ait.caothang.edu.vn → Hồ sơ:
const char *USER_SERVER = "xxxxxxxxxx@caothang.edu.vn";
const char *PASS_SERVER = "xxxxxxxxxxxxxx";

// Include thư viện chính
#include <CKC.h>

/*
==========================================================
  SETUP
==========================================================
*/
void setup()
{
  Serial.begin(115200);     // Khởi tạo Serial
  // Kết nối WiFi + Server CKC
  CKC.begin(SSID, PASS,USER_SERVER,PASS_SERVER);  
}
/*
==========================================================
  LOOP
==========================================================
*/
void loop()
{
  // Hàm chạy chính của thư viện (bắt buộc)
  CKC.run(); 
}
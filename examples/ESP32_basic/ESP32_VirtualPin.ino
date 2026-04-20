
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


// Bật debug log ra Serial Monitor
#define CKC_DEBUG

// Bật chế độ nút nhấn (tuỳ thư viện bạn xử lý)
#define BUTTON_MODE

// Thông tin WiFi
const char *SSID = "CKC";
const char *PASS = "CKC2026";

// Include thư viện chính
#include <CKC.h>

// Biến lưu thời gian để gửi dữ liệu định kỳ
int32_t time_P = 0;


/*
==========================================================
  CALLBACK NHẬN DỮ LIỆU TỪ SERVER (Virtual Pin)
==========================================================
*/

// Nhận dữ liệu từ Virtual Pin V3
CKC_WRITE(V3)
{
   int a = param.getInt();  // Lấy dữ liệu kiểu int
   CKC_LOG_DEBUG("CKC","value %d", a);  // In debug
}

// Nhận dữ liệu từ Virtual Pin V2
CKC_WRITE(V2)
{
   float a = param.getFloat();  // Lấy dữ liệu kiểu float
   CKC_LOG_DEBUG("CKC1","value %f", a);
}


/*
==========================================================
  SETUP
==========================================================
*/
void setup()
{
  Serial.begin(115200);     // Khởi tạo Serial
  pinMode(26, OUTPUT);      // LED báo trạng thái

  // Kết nối WiFi + Server CKC
  CKC.begin(SSID, PASS);  

  // Khai báo các key telemetry sẽ gửi lên server
  CKC.set_Telemetry("TEM","HUM");
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

  // Kiểm tra trạng thái kết nối
  if (CKC.connected())
  {
    digitalWrite(26, 1);  // Có kết nối server → bật LED
  }
  else
  {
    digitalWrite(26, 0);  // Mất kết nối server → tắt LED
  }

  // Gửi dữ liệu mỗi 1 giây
  if (millis() - time_P > 1000)
  {
    time_P = millis();

    // Gửi dữ liệu nhiệt độ lên server
    CKC.WriteTelemetry("TEM", 30);
  }
}
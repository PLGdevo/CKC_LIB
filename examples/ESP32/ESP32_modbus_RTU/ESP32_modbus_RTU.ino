
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

/*
    =========================================================
    READ HOLDING REGISTER (FC03)
    =========================================================
    int readHoldingRegisterValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values);
                                (địa chỉ Slave,thanh ghi bắt đầu, số lượng thanh ghi, mảng lưu giá trị)

    =========================================================
    READ INPUT REGISTER (FC04)
    =========================================================
    int readInputRegistersValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values);
                                (địa chỉ Slave,thanh ghi bắt đầu, số lượng thanh ghi, mảng lưu giá trị)

    =========================================================
    WRITE REGISTER (FC06) && (FC16)
    =========================================================
    bool writeSingleRegister(uint8_t slave, uint16_t reg, uint16_t value);
                            (địa chỉ slave,thanh ghi ,giá trị)
    bool writeMultiRegister(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *value);
                            (địa chỉ Slave,thanh ghi bắt đầu, số lượng thanh ghi, giá trị)

    =========================================================
    WRITE COIL (FC05) && (FC15)
    =========================================================
    bool writeSingleCoil(uint8_t slave, uint16_t coil, bool value);
                        (địa chỉ slave,thanh ghi ,giá trị)
    bool writeMultiCoil(uint8_t slave, uint16_t coil, uint16_t num, bool *value);
                        (địa chỉ Slave,thanh ghi bắt đầu, số lượng thanh ghi, giá trị)

    =========================================================
    READ COIL (FC01)
    =========================================================
    int readCoils(uint8_t slave, uint16_t coil, uint16_t num, bool *value);
                  (địa chỉ Slave,thanh ghi bắt đầu, số lượng thanh ghi, mảng lưu giá trị)  

    =========================================================
    TRANSPLATE REGISTER (dữ liệu int_16 không dấu)
    =========================================================
    float RegToFloat(uint16_t reg1, uint16_t reg2);
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

#define RX_ 16 // chân RX cho Module Modbus
#define TX_ 17 // chân TX cho Module Modbus

uint16_t data_sensor[18]; // Tạo bảng dữ liệu data_sensor
// ================= READ TEMPRATURE ==========================
void timeEvent()
{
    if (CKC.connected())
    {
        int doccambien = CKCModbus.readInputRegistersValue(1, 449, 10, data_sensor);
        //(Địa chỉ cảm biến, đọc từ thanh ghi, số thanh ghi, bảng lưu thanh ghi). Hàm đọc giá trị các thanh ghi
        if (doccambien > 0)
        {
            float nhietdo1 = CKCModbus.RegToFloat(data_sensor[0], data_sensor[1]); // Hàm ghép hai thanh ghi dữ liệu để xuất dữ liệu
            CKC.writeTelemetry("TEM1", nhietdo1);                                  //("Key" , giá trị)

            float nhietdo2 = CKCModbus.RegToFloat(data_sensor[2], data_sensor[3]);
            CKC.writeTelemetry("TEM2", nhietdo2); //("Key", giá trị)

            float nhietdo3 = CKCModbus.RegToFloat(data_sensor[4], data_sensor[5]);
            CKC.writeTelemetry("TEM3", nhietdo3); //("Key", giá trị)
        }
    }
}

/*
==========================================================
  SETUP
==========================================================
*/
void setup()
{
    Serial.begin(115200); // Khởi tạo Serial
    // Kết nối WiFi + Server CKC
    CKC.begin(SSID, PASS, USER_SERVER, PASS_SERVER);

    Serial2.begin(9600);
    CKCModbus.beginModbus(Serial2, 9600, RX_, TX_);
    //(cổng giao tiếp UART, BAUDRAY ,RX_pin, TX_pin)
    CKCModbus.setTimeout(1000);

    CKC.addTimeEvent(3000, timeEvent);
    // Cứ mối 3s thì hàm timeEvent sẽ đưuọc thực hiện
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
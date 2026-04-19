<img width="886" height="279" alt="image" src="https://github.com/user-attachments/assets/5ecebe79-e025-4ad8-ac69-b6c86a63d06b" />﻿🚀 CKC IoT Library

CKC IoT Library là thư viện hỗ trợ xây dựng hệ thống IoT nhanh chóng, linh hoạt và dễ mở rộng.
Được phát triển bởi sinh viên Cao Thắng, thư viện hướng đến việc tối ưu giao tiếp giữa thiết bị và server.

✨ Tính năng chính
📡 Gửi dữ liệu (Telemetry) lên server
🎛 Nhận lệnh điều khiển từ app/web
🔄 Đồng bộ trạng thái thiết bị
🧩 Thiết kế dạng module dễ mở rộng
📦 Hỗ trợ nhiều giao thức:
MQTT
Serial
HTTP (có thể mở rộng)
🧰 Nền tảng hỗ trợ
ESP32
STM32
Arduino
Các hệ thống nhúng khác
🌐 Kiến trúc
Device (ESP32/STM32)
        ↓
   CKC Library
        ↓
   MQTT / Protocol
        ↓
     Server
        ↓
   Mobile App / Web
📥 Cài đặt
Arduino IDE
Tải thư viện về

Import vào Arduino IDE:

Sketch → Include Library → Add .ZIP Library
PlatformIO

Thêm vào platformio.ini:

lib_deps =
    https://github.com/your-repo/ckc-library.git
⚡ Quick Start (ESP32)
#include <CKC.h>

void setup() {
    Serial.begin(115200);

    CKC.begin("your_token", "your_wifi", "your_pass");

    CKC.setTelemetry("temperature", 25);
}

void loop() {
    CKC.run();
}
📡 Gửi dữ liệu (Telemetry)
CKC.setTelemetry("temp", 30);
CKC.setTelemetry("humidity", 60.5);
CKC.setTelemetry("status", true);
🎛 Nhận điều khiển
void onControl(const char* key, CKCParam value) {
    if (strcmp(key, "led") == 0) {
        digitalWrite(2, value.getInt());
    }
}

CKC.setControlCallback(onControl);
🧠 Cấu trúc CKCParam
Kiểu dữ liệu	Mô tả
INT	Số nguyên
FLOAT	Số thực
BOOL	True/False
STRING	Chuỗi
📱 Ứng dụng
Smart Home 🏠
Nông nghiệp thông minh 🌱
Giám sát công nghiệp 🏭
IoT học tập & nghiên cứu 🎓
📌 Roadmap
 Web Dashboard
 OTA Update
 Multi-device sync
 AI Integration
🤝 Đóng góp

Mọi đóng góp đều được hoan nghênh!

fork → code → pull request
📄 License

MIT License © 2026 CKC Team

❤️ Credits

Developed by Sinh viên Cao Thắng
Built with passion for IoT 🚀

👨‍💻 Tác giả

PLG Team - Cao Thắng Technical College

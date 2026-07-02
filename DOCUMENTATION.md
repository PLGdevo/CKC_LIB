# Hướng dẫn sử dụng thư viện CKC IoT

## 1. Giới thiệu thư viện

Thư viện CKC IoT là một thư viện hỗ trợ xây dựng hệ thống IoT trên các thiết bị nhúng như ESP32, ESP8266 và các nền tảng Arduino tương thích. Thư viện giúp bạn:

- kết nối thiết bị với Wi-Fi và MQTT;
- gửi dữ liệu telemetry lên server;
- nhận lệnh điều khiển từ server;
- xử lý virtual pin và pin mạch thật;
- dễ mở rộng bằng các callback và timer sự kiện.

Thư viện phù hợp cho các ứng dụng như cảm biến, điều khiển thiết bị, dashboard IoT và hệ thống giám sát từ xa.

---

## 2. Cấu trúc hệ thống

Thư viện được tổ chức theo các module chính sau:

- src/AIoT
  - chứa logic xử lý API, callback, tham số, topic và protocol trung tâm;
  - là phần cốt lõi của thư viện.
- src/CKC_WiFi
  - chứa lớp kết nối Wi-Fi, cấu hình PnP và tích hợp mạng.
- src/MQTT
  - cung cấp giao thức MQTT và các client hỗ trợ.
- src/Modbus
  - hỗ trợ giao tiếp Modbus.
- src/UIlty
  - chứa util như cJSON để xử lý JSON.
- examples
  - cung cấp ví dụ sử dụng cho ESP32 và ESP8266.

### Luồng hoạt động cơ bản

1. Thiết bị kết nối Wi-Fi.
2. Thiết bị kết nối tới server MQTT.
3. Thiết bị gửi telemetry.
4. Server gửi lệnh điều khiển về thiết bị.
5. Thư viện phân tích payload và gọi callback tương ứng.

---

## 3. Cách cài đặt

### Arduino IDE

1. Tải thư viện về.
2. Chọn Sketch → Include Library → Add .ZIP Library.
3. Chọn thư mục thư viện đã tải.

### PlatformIO

Thêm thư viện vào file platformio.ini như sau:

```ini
lib_deps =
    https://github.com/PLGdevo/CKC_LIB
```

---

## 4. Cách dùng cơ bản

### 4.1 Include thư viện

```cpp
#include <CKC.h>
```

### 4.2 Khởi tạo Wi-Fi và MQTT

```cpp
const char *SSID = "TEN_WIFI";
const char *PASS = "MAT_KHAU_WIFI";
const char *USER_SERVER = "user@domain.com";
const char *PASS_SERVER = "password";

void setup() {
  Serial.begin(115200);
  CKC.begin(SSID, PASS, USER_SERVER, PASS_SERVER);
}

void loop() {
  CKC.run();
}
```

### 4.3 Gửi dữ liệu telemetry

```cpp
void setup() {
  CKC.begin(SSID, PASS, USER_SERVER, PASS_SERVER);
}

void loop() {
  CKC.run();
  CKC.writeTelemetry("TEM", 30);
}
```

### 4.4 Gửi trạng thái

```cpp
CKC.writeStatus("status", "online");
```

### 4.5 Gửi lệnh điều khiển

```cpp
CKC.writeControl("LED", 1);
```

---

## 5. Các hàm chính của thư viện

### 5.1 CKC.begin()

Khởi tạo hệ thống kết nối.

Cú pháp:

```cpp
CKC.begin(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass);
```

Mục đích:
- khởi tạo Wi-Fi;
- thiết lập kết nối với server MQTT;
- chuẩn bị các topic telemetry/control/status.

### 5.2 CKC.run()

Hàm chạy chính, phải được gọi liên tục trong loop.

```cpp
void loop() {
  CKC.run();
}
```

### 5.3 CKC.connected()

Kiểm tra trạng thái kết nối.

```cpp
if (CKC.connected()) {
  // đã kết nối
}
```

### 5.4 CKC.addTimeEvent()

Đăng ký một sự kiện chạy theo chu kỳ.

```cpp
void timeEvent() {
  CKC.writeTelemetry("TEM", 30);
}

void setup() {
  CKC.begin(SSID, PASS, USER_SERVER, PASS_SERVER);
  CKC.addTimeEvent(5000L, timeEvent);
}
```

### 5.5 CKC.writeTelemetry()

Gửi dữ liệu telemetry lên server.

```cpp
CKC.writeTelemetry("TEM", 30);
CKC.writeTelemetry("HUM", 60.5f);
```

### 5.6 CKC.writeControl()

Gửi dữ liệu điều khiển hoặc phản hồi điều khiển.

```cpp
CKC.writeControl("LED", 1);
```

### 5.7 setTelemetry(), setControl(), setStatus()

Dùng để khai báo cấu trúc dữ liệu trước khi gửi.

```cpp
CKC.setTelemetry("TEM", "HUM");
CKC.setControl("LED", "RELAY");
```

---

## 6. Xử lý lệnh từ server bằng Virtual Pin

Thư viện hỗ trợ nhận lệnh từ server thông qua virtual pin bằng macro CKC_WRITE.

### Cú pháp

```cpp
CKC_WRITE(TEM)
{
  int value = param.getInt();
  // xử lý giá trị ở đây
}
```

### Ví dụ

```cpp
CKC_WRITE(V2)
{
  float value = param.getFloat();
  Serial.println(value);
}
```

Macro này cho phép bạn viết callback riêng cho từng virtual pin, giúp việc điều khiển thiết bị trở nên trực quan và dễ mở rộng.

---

## 7. Class CKCParam

CKCParam là lớp dùng để lưu trữ và đọc dữ liệu nhận được từ server hoặc gửi đi.

### Các phương thức chính

- getInt()
- getFloat()
- getDouble()
- getBool()
- getString()

### Ví dụ

```cpp
CKC_WRITE(TEM)
{
  int a = param.getInt();
  float b = param.getFloat();
  bool c = param.getBool();
  String d = param.getString();
}
```

---

## 8. Ví dụ đầy đủ

```cpp
#include <CKC.h>

#define CKC_DEBUG

const char *SSID = "TEN_WIFI";
const char *PASS = "MAT_KHAU_WIFI";
const char *USER_SERVER = "user@domain.com";
const char *PASS_SERVER = "password";

CKC_WRITE(TEM)
{
  int value = param.getInt();
  Serial.println(value);
}

void timeEvent()
{
  CKC.writeTelemetry("TEM", 30);
}

void setup() {
  Serial.begin(115200);
  CKC.begin(SSID, PASS, USER_SERVER, PASS_SERVER);
  CKC.addTimeEvent(5000L, timeEvent);
}

void loop() {
  CKC.run();
}
```

---

## 9. Gợi ý khi phát triển với thư viện

- Luôn gọi CKC.run() trong loop.
- Không nên gọi các thao tác mạng quá thường xuyên trong vòng lặp chính.
- Dùng timer event cho các tác vụ gửi dữ liệu định kỳ.
- Dùng CKC_WRITE cho các virtual pin để giữ code rõ ràng và dễ mở rộng.
- Khi làm việc với JSON, đảm bảo dữ liệu đúng cấu trúc mà thư viện mong đợi.

---

## 10. Kết luận

Thư viện CKC IoT cung cấp một cách tiếp cận đơn giản nhưng mạnh mẽ để xây dựng ứng dụng IoT trên thiết bị nhúng. Với khả năng kết nối Wi-Fi, MQTT, xử lý lệnh và quản lý định kỳ, thư viện phù hợp cho các dự án demo, sản phẩm thử nghiệm và hệ thống giám sát điều khiển từ xa.

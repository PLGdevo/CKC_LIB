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

1. Mở Arduino IDE.
2. Chọn Sketch → Include Library → Manage Libraries...
3. Tìm kiếm CKC hoặc AIoT CKC.
4. Chọn thư viện CKC IoT.
5. Nhấn Install để cài đặt.

### PlatformIO

#### Cài đặt bằng giao diện PlatformIO

1. Mở **Visual Studio Code** với **PlatformIO**.
2. Mở **Libraries**.
3. Tìm kiếm **CKC** hoặc **CKC AIoT**.
4. Chọn thư viện **CKC IoT**.
5. Nhấn **Add to Project** và chọn dự án cần sử dụng.

#### Cài đặt bằng `platformio.ini`

Hoặc thêm thư viện vào tệp `platformio.ini`:

```ini
lib_deps =
    PLGdevo/CKC
```

Sau khi lưu tệp, PlatformIO sẽ tự động tải và cài đặt thư viện.

#### Cài đặt từ GitHub (Phiên bản mới nhất)

Nếu muốn sử dụng phiên bản mới nhất hoặc phiên bản đang phát triển, bạn có thể cài đặt trực tiếp từ GitHub:

```ini
lib_deps =
    https://github.com/PLGdevo/CKC_LIB.git
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

### 4.4 Gửi lệnh điều khiển

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

### 5.7 setTelemetry(), setControl()

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

## 8. Button Mode (Chế độ nút nhấn)

Button Mode cho phép thiết bị vào chế độ cấu hình WiFi qua web interface khi nhấn nút trên thiết bị. Tính năng này rất hữu ích để reset cài đặt hoặc cấu hình lại mạng mà không cần nạp lại code.

### Cách bật Button Mode

Thêm dòng sau vào đầu file Arduino:

```cpp
#define BUTTON_MODE
#include <CKC.h>
```

### Cách hoạt động

Khi Button Mode được bật:
- Thiết bị sẽ tạo ra một access point (AP) WiFi khi nhấn nút cấu hình.
- Người dùng có thể kết nối đến AP này qua điện thoại hoặc máy tính.
- Mở trình duyệt và truy cập giao diện web để cài đặt WiFi mới.

### Ví dụ với Button Mode

```cpp
#define BUTTON_MODE
#define CKC_DEBUG
#include <CKC.h>

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

Khi muốn cấu hình WiFi:
1. Nhấn nút trên thiết bị (GPIO tương ứng).
2. Thiết bị tạo ra AP WiFi với tên `CKC_Config_XXXXXX`.
3. Kết nối đến AP này.
4. Mở trình duyệt, truy cập `192.168.4.1`.
5. Nhập SSID và mật khẩu WiFi mới.
6. Thiết bị sẽ kết nối lại với WiFi mới.

---

## 9. Web Configuration cho WiFi

Thư viện cung cấp giao diện web tích hợp để cài đặt WiFi và các thông tin kết nối mà không cần sửa code.

### Tính năng

- Giao diện web dễ sử dụng (UI responsive).
- Tìm kiếm và chọn mạng WiFi có sẵn.
- Cấu hình thông tin đăng nhập server MQTT.
- Lưu cài đặt vào bộ nhớ flash của thiết bị.

### Cách sử dụng Web Config

#### 1. Thông qua Button Mode

Như đã nêu ở mục 8, nhấn nút để kích hoạt chế độ AP.

#### 2. Thông qua Serial Monitor (nếu có)

Một số phiên bản thư viện cho phép gửi lệnh qua Serial để kích hoạt web config:

```
// Gửi lệnh này qua Serial Monitor để kích hoạt AP
CONFIG_MODE
```

#### 3. Giao diện Web Config

Sau khi kết nối đến AP, mở trình duyệt và truy cập: `http://192.168.6.1`

Các trường cài đặt trên giao diện web:
- **SSID**: Tên mạng WiFi
- **Password**: Mật khẩu WiFi
- **MQTT Server**: Địa chỉ server MQTT
- **MQTT Username**: Tên đăng nhập MQTT
- **MQTT Password**: Mật khẩu MQTT

### Ví dụ: Cấu hình WiFi qua Web

```
1. Thiết bị tạo AP: CKC_Config_ABCD1234
2. Bạn kết nối đến AP này
3. Mở browser → 192.168.6.1
4. Thấy danh sách WiFi xung quanh
5. Chọn WiFi của bạn → nhập mật khẩu
6. Nhập thông tin server MQTT
7. Nhấn "SAVE" hoặc "APPLY"
8. Thiết bị khởi động lại → kết nối WiFi mới
```

### Lưu ý khi sử dụng Web Config

- Cài đặt sẽ được lưu vào bộ nhớ EEPROM hoặc Flash của thiết bị.
- Các lần khởi động sau, thiết bị sẽ tự kết nối với WiFi đã cấu hình.
- Để quay lại chế độ AP config, nhấn nút hoặc gửi lệnh reset.
- Đảm bảo rằng thiết bị được cấp điện đủ khi cấu hình.

---

## 10. Gợi ý khi phát triển với thư viện

- Luôn gọi CKC.run() trong loop.
- Không nên gọi các thao tác mạng quá thường xuyên trong vòng lặp chính.
- Dùng timer event cho các tác vụ gửi dữ liệu định kỳ.
- Dùng CKC_WRITE cho các virtual pin để giữ code rõ ràng và dễ mở rộng.
- Khi làm việc với JSON, đảm bảo dữ liệu đúng cấu trúc mà thư viện mong đợi.
- Sử dụng Button Mode để cấu hình WiFi một cách linh hoạt.
- Lưu các tham số nhạy cảm (password) vào EEPROM để tránh lộ lên code.

---

## 11. Ví dụ đầy đủ với Button Mode và Web Config

```cpp
#define BUTTON_MODE
#define CKC_DEBUG
#include <CKC.h>

const char *SSID = "TEN_WIFI";
const char *PASS = "MAT_KHAU_WIFI";
const char *USER_SERVER = "user@domain.com";
const char *PASS_SERVER = "password";

// Virtual Pin nhận dữ liệu từ server
CKC_WRITE(TEM)
{
  int temp = param.getInt();
  Serial.print("Nhiệt độ nhận được: ");
  Serial.println(temp);
}

CKC_WRITE(V2)
{
  float humidity = param.getFloat();
  Serial.print("Độ ẩm: ");
  Serial.println(humidity);
}

// Timer event gửi dữ liệu định kỳ
void sendTelemetry()
{
  CKC.writeTelemetry("TEM", 30);
  CKC.writeTelemetry("HUM", 65.5f);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  Serial.println("CKC IoT System Starting...");
  
  // Khởi tạo kết nối WiFi + MQTT
  CKC.begin(SSID, PASS, USER_SERVER, PASS_SERVER);
  
  // Gửi dữ liệu mỗi 5 giây
  CKC.addTimeEvent(5000L, sendTelemetry);
  
  Serial.println("Setup complete!");
}

void loop() {
  // Chạy hệ thống thư viện
  CKC.run();
  
  // Kiểm tra trạng thái kết nối
  if (CKC.connected()) {
    digitalWrite(LED_PIN, HIGH);  // Bật LED nếu đã kết nối
  } else {
    digitalWrite(LED_PIN, LOW);   // Tắt LED nếu mất kết nối
  }
}
```

---

## 12. Kết luận

Thư viện CKC IoT cung cấp một cách tiếp cận đơn giản nhưng mạnh mẽ để xây dựng ứng dụng IoT trên thiết bị nhúng. Với khả năng:
- kết nối Wi-Fi và MQTT tự động,
- xử lý lệnh điều khiển từ server,
- gửi dữ liệu telemetry định kỳ,
- cấu hình Wi-Fi qua web interface,
- hỗ trợ button mode cho việc reset cài đặt,
- quản lý timer event cho các tác vụ không đồng bộ.

Thư viện phù hợp cho các dự án demo, sản phẩm thử nghiệm, hệ thống giám sát điều khiển từ xa, và các ứng dụng IoT thực tiễn trên nền tảng ESP32 và ESP8266.

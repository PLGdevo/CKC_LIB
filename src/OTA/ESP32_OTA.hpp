#include "OTA/ESP32_OTA.h"
#include <Update.h>
#include <ESPmDNS.h>
#include <WiFi.h>

extern const char WebOTA[] PROGMEM;

// Cấu hình tài khoản bảo mật đồng bộ với Frontend
const char* ota_user = "admin";
const char* ota_pass = "123456";

void CKC_OTA::begin(WebServer &server)
{
    _server = &server;

    this->setupRoute();

    _server->begin();

    if (MDNS.begin("ckc_ota"))
    {
        MDNS.addService("http", "tcp", 80);
    }

    Serial.printf("CKC Panel: http://%s:%d\n",
                  WiFi.localIP().toString().c_str(),
                  80);

    Serial.println("[CKC OTA] Integrated Control Panel initialized");
}

void CKC_OTA::stop()
{
    if (_server)
    {
        _server->stop();
        _server = nullptr;
    }

    MDNS.end();

    Serial.println("[CKC OTA] Service stopped");
}

void CKC_OTA::setupRoute()
{
    // Giao diện chính (Chứa form login, cấu hình wifi và cập nhật ota)
    _server->on("/", HTTP_GET, [this]()
    {
        handleRoot();
    });

    // --- ROUTE LẤY THÔNG TIN HỆ THỐNG (Yêu cầu login) ---
    _server->on("/info", HTTP_GET, [this]()
    {
        if (!_server->authenticate(ota_user, ota_pass)) {
            return _server->requestAuthentication();
        }
        handleInfo();
    });

    // --- ROUTE QUÉT WIFI CHUYỂN JSON VỀ FRONTEND ---
    _server->on("/scan", HTTP_GET, [this]()
    {
        if (!_server->authenticate(ota_user, ota_pass)) {
            return _server->requestAuthentication();
        }
        
        int n = WiFi.scanNetworks();
        String json = "[";
        for (int i = 0; i < n; ++i) {
            json += "{";
            json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
            json += "\"rssi\":" + String(WiFi.RSSI(i));
            json += "}";
            if (i < n - 1) json += ",";
        }
        json += "]";
        WiFi.scanDelete(); // Giải phóng bộ nhớ sau khi quét xong
        
        _server->send(200, "application/json", json);
    });

    // --- ROUTE NHẬN THÔNG TIN KẾT NỐI WIFI VÀ MQTT ---
    _server->on("/connect", HTTP_POST, [this]()
    {
        if (!_server->authenticate(ota_user, ota_pass)) {
            return _server->requestAuthentication();
        }

        String ssid = _server->arg("ssid");
        String pass = _server->arg("pass");
        String mqtt_user = _server->arg("mqtt_user");
        String mqtt_pass = _server->arg("mqtt_pass");

        Serial.println("\n[Config Received]");
        Serial.printf("SSID: %s\n", ssid.c_str());
        Serial.printf("MQTT User: %s\n", mqtt_user.c_str());
        
        // Bạn có thể lưu các thông số này vào Preferences hoặc EEPROM tại đây để khởi động lại tự load
        
        _server->send(200, "text/html", "<h3>Connecting to WiFi... Device may change IP address.</h3>");
        
        // Tiến hành đổi mạng mạng ngầm
        WiFi.begin(ssid.c_str(), pass.c_str());
    });

    // --- ROUTE UPLOAD FIRMWARE OTA ---
    _server->on("/update", HTTP_POST,
        [this]() // Sau khi hoàn thành nhận file
        {
            if (!_server->authenticate(ota_user, ota_pass)) return;
            
            if (Update.hasError())
            {
                _server->send(500, "text/plain", "FAIL");
            }
            else
            {
                _server->send(200, "text/plain", "OK");
                delay(1000);
                ESP.restart();
            }
        },
        [this]() // Xử lý ghi từng gói tin nhị phân phân đoạn
        {
            if (!_server->authenticate(ota_user, ota_pass)) return;
            handleUpdate();
        }
    );
}

void CKC_OTA::handleRoot()
{
    _server->send_P(200, "text/html", WebOTA);
}

void CKC_OTA::handleInfo()
{
    String mac = WiFi.macAddress();
    String chipModel = ESP.getChipModel(); 
    uint32_t flashSize = ESP.getFlashChipSize();
    uint32_t freeOta = ESP.getFreeSketchSpace();
    uint32_t freeHeap = ESP.getFreeHeap();
    uint32_t totalHeap = ESP.getHeapSize();

    String json = "{";
    json += "\"mac\":\"" + mac + "\",";
    json += "\"chip\":\"" + chipModel + "\",";
    json += "\"flash\":\"" + String((float)flashSize / 1024 / 1024, 1) + " MB\",";
    json += "\"free_ota\":\"" + String((float)freeOta / 1024 / 1024, 1) + " MB\",";
    json += "\"ram\":\"" + String((float)freeHeap / 1024, 0) + " KB / " + String((float)totalHeap / 1024, 0) + " KB\"";
    json += "}";

    _server->send(200, "application/json", json);
}

void CKC_OTA::handleUpdate()
{
    HTTPUpload &upload = _server->upload();

    switch (upload.status)
    {
    case UPLOAD_FILE_START:
        Serial.printf("OTA Start: %s\n", upload.filename.c_str());        
        
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) 
        {
            Update.printError(Serial);
        }
        break;

    case UPLOAD_FILE_WRITE:
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
        {
            Update.printError(Serial);
        }
        break;

    case UPLOAD_FILE_END:
        if (Update.end(true))
        {
            Serial.printf("OTA Success: %u bytes\n", upload.totalSize);
        }
        else
        {
            Update.printError(Serial);
        }
        break;

    case UPLOAD_FILE_ABORTED:
        Update.end();
        Serial.println("OTA Aborted");
        break;

    default:
        break;
    }
}
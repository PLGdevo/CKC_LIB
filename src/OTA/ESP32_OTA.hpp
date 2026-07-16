#include "OTA/ESP32_OTA.h"

extern const char WebOTA[] PROGMEM;

void CKC_OTA::begin(WebServer &server)
{
    _server = &server;

    this->setupRoute();

    _server->begin();

    if (MDNS.begin("PLG_ota"))
    {
        MDNS.addService("http", "tcp", 80);
    }

    Serial.printf("CKC OTA: http://%s:%d\n",
                  WiFi.localIP().toString().c_str(),
                  80);

    Serial.println("[CKC OTA] Web OTA initialized");
}

void CKC_OTA::stop()
{
    if (_server)
    {
        _server->stop();
        _server = nullptr;
    }

    MDNS.end();

    Serial.println("[CKC OTA] OTA service stopped");
}

void CKC_OTA::setupRoute()
{
    _server->on("/", HTTP_GET, [this]()
    {
        handleRoot();
    });

    // --- THÊM ROUTE LẤY THÔNG TIN HỆ THỐNG ---
    _server->on("/info", HTTP_GET, [this]()
    {
        handleInfo();
    });

    _server->on(
        "/update",
        HTTP_POST,

        // Kết thúc upload
        [this]()
        {
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

        // Upload từng gói dữ liệu
        [this]()
        {
            handleUpdate();
        });
}

void CKC_OTA::handleRoot()
{
    _server->send_P(
        200,
        "text/html",
        WebOTA
    );
}

// --- HÀM XỬ LÝ LẤY THÔNG TIN THỰC TẾ TỪ CHIP ESP32 ---
void CKC_OTA::handleInfo()
{
    String mac = WiFi.macAddress();
    String chipModel = ESP.getChipModel(); // Tự động nhận diện dòng chip thực tế (ESP32, ESP32-S3,...)
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

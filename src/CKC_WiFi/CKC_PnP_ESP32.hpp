#ifndef CKC_ESP32
#define CKC_ESP32
#include <Arduino.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <AIoT/CKC_API.hpp>
#include <MQTT/ESP32_MQTT.hpp>
#include <WebServer.h>
#include <Preferences.h>
#include <CKC_WiFi/Confg_UI.hpp>

#define WIFI_AP_Subnet IPAddress(255, 255, 255, 0)
char STA_WIFI_NAME[32];
char STA_WIFI_PASS[32];
#define STA_WIFI_PORT "80"

#define AP_WIFI_NAME "CKC:"
#define AP_WIFI_PASS "CKC@2026"
#define AP_WIFI_IP "192.168.6.1"
#define AP_WIFI_PORT "80"

enum CKC_WiFI_TASK
{
    MODE_CONFIG,
    MODE_STA,
    MODE_AP,
    MODE_CONNECTED,
    RUN_AP_WEB,
    WIFI_DISCONNECTED,
};

CKC_WiFI_TASK WiFi_TASK = MODE_STA;
template <class Transport>
class CKC_PnP
{
    WebServer webServer{80};
    Preferences prefs;
    DNSServer dnsServer;

public:
    CKC_PnP() {};
    // START TASK
    void init(const char *sta_ssid, const char *sta_pass);                                                   // đưa cấu hình WiFi vào từ khai báo
    void init(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass); // đưa cấu hình WiFi và Server từ khai báo
    // Handle SYSTEM
    void SaveWiFi(String newSSID, String newPASS);   // Thực hiện Lưu WiFi từ Config_UI mỗi khi có WiFi mới
    void SaveMQTT(String mqttuser, String mqttpass); // Thực hiện lưu tài khoảng từ Config_UI mỗi khi cấu hình
    void loadWiFi();                                 // tải danh sách WiFi từ trong bộ nhớ ESP32
    void loadMQTT();                                 // tải tài khoảng từ trong bộ nhớ ESP32
    void handleScan();                               // thực hiện lệnh Scan WiFi xung quanh trong Config_UI
    void handleSave();                               // thực hiện lệnh nhúng cấu hình từ Config_UI để xử lý
    void CKC_state_Connect_STA();                    // khởi tạo và cấu hình chế độ STA
    void STA();
    void CKC_state_Connect_AP(); // khởi tạo và cấu hình chế độ AP
    void AP();
    void resetAPMODE();        // RESET lại giá trị về ban đầu sau khi đã kết nối
    void Try_Connect();        // Kết nối lại WiFi sau khi bị báo rớt WiFi
    void CKC_mode_connected(); // Hàm hoạt động giao tiếp giữa thiết bị và máy chủ và kiểm tra kết nối
    bool CkC_Connected();      // Hàm kiểm tra kết nối WiFi
    void run();                // Hàm chạy tuần tự chương trình theo STATE
    static const char WebConfigHEAD[] PROGMEM;
    static const char WebConfigFOOT[] PROGMEM;
    void CKC_SendPage();
    void handler_button();

private:
    IPAddress _ipAddr;
    // sta
    char _sta_ssid[64];
    char _sta_pass[32];
    char _sta_ip[16];
    char _sta_port[5] = STA_WIFI_PORT;
    int _rssi;
    // ap
    char _ap_ssid[64] = AP_WIFI_NAME;
    char _ap_pass[32] = AP_WIFI_PASS;
    char _ap_ip[16] = AP_WIFI_IP;
    char _ap_port[5] = AP_WIFI_PORT;
    char _mac[18];

    // ===== MQTT INIT ===== //
    char mqttusername[64];
    char mqttpass[64];
    // ===== MQTT EEPROM ===== //
    char _mqtt_username[64];
    char _mqtt_pass[64];

    unsigned long t0, t1, t2, t3, t4;
    int time_sta = 10000;

    int reconnect_count = 0;
    unsigned long lastAttemp = 0;
    const unsigned long retryDelay = 10000;
    bool isconnecting = false;
    unsigned long connectStart = 0;

    bool setup_device_state = false;

#define FLASH_BTN 0 // nút BOOT/FLASH trên ESP32 thường là GPIO0
    unsigned long pressStart = 0;
    bool triggered = false;
    bool triggered_AP = false;

    String newSSID;
    String newPASS;

#define Saved_WiFi_MAX 2
#define Scan_WiFi_MAX 5
    // ===== WIFI ĐÃ LƯU (AUTO CONNECT) ===== //
    String saved_ssid[Saved_WiFi_MAX];
    String saved_pass[Saved_WiFi_MAX];

    // ===== WIFI SCAN (HIỂN THỊ WEB) ===== //
    String scan_ssid[Scan_WiFi_MAX];
    int scan_rssi[Scan_WiFi_MAX];
};
//========== START SYSTEM ==========//
template <class Transport>
inline void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass)
{
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    delay(500);
    // WiFi.setAutoReconnect(true);
    // WiFi.persistent(true);
    // WiFi.setHostname("CKC_ESP32");
    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", _ap_ssid, _mac);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _sta_ip);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    t1 = millis();
    pinMode(FLASH_BTN, INPUT_PULLUP); // nút kéo xuống GND khi nhấn
}

template <class Transport>
inline void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass)
{
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setTxPower(WIFI_POWER_19_5dBm);
    delay(500);
    // WiFi.setAutoReconnect(true);
    // WiFi.persistent(true);
    // WiFi.setHostname("CKC_ESP32");
    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);
    strcpy(mqttusername, mqtt_userName);
    strcpy(mqttpass, mqtt_pass);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", _ap_ssid, _mac);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _sta_ip);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    t1 = millis();
    pinMode(FLASH_BTN, INPUT_PULLUP); // nút kéo xuống GND khi nhấn
}

//========== HANDLE SYSTEM ==========//
template <class Transport>
inline void CKC_PnP<Transport>::SaveWiFi(String newSSID, String newPASS)
{
    if (newSSID.length() == 0)
        return;

    if (!prefs.begin("wifi", false))
    {
        CKC_LOG_DEBUG("WIFI", "NVS OPEN FAIL");
        return;
    }
    // =========================
    // CHECK EXIST SSID
    // =========================
    for (int i = 0; i < Saved_WiFi_MAX; i++)
    {
        if (saved_ssid[i] == newSSID)
        {
            // PASS GIỐNG -> SKIP
            if (saved_pass[i] == newPASS)
            {
                CKC_LOG_DEBUG("WIFI", "SSID & PASS EXISTS -> SKIP");
                prefs.end();
                return;
            }
            // PASS KHÁC -> UPDATE
            CKC_LOG_DEBUG("WIFI", "SSID EXISTS -> UPDATE PASS");
            // Xóa vị trí cũ
            for (int j = i; j > 0; j--)
            {
                saved_ssid[j] = saved_ssid[j - 1];
                saved_pass[j] = saved_pass[j - 1];
            }
            // Đưa lên đầu
            saved_ssid[0] = newSSID;
            saved_pass[0] = newPASS;
            // SAVE NVS
            for (int k = 0; k < Saved_WiFi_MAX; k++)
            {
                String keyS = "ssid" + String(k);
                String keyP = "pass" + String(k);

                if (saved_ssid[k].length() == 0)
                {
                    prefs.remove(keyS.c_str());
                    prefs.remove(keyP.c_str());
                }
                else
                {
                    prefs.putString(keyS.c_str(), saved_ssid[k]);
                    prefs.putString(keyP.c_str(), saved_pass[k]);
                }
            }
            prefs.end();
            CKC_LOG_DEBUG("WIFI", "UPDATE WIFI DONE: %s", newSSID.c_str());
            return;
        }
    }
    // =========================
    // ADD NEW WIFI
    // =========================
    for (int i = Saved_WiFi_MAX - 1; i > 0; i--)
    {
        saved_ssid[i] = saved_ssid[i - 1];
        saved_pass[i] = saved_pass[i - 1];
    }
    saved_ssid[0] = newSSID;
    saved_pass[0] = newPASS;
    // SAVE NVS
    for (int i = 0; i < Saved_WiFi_MAX; i++)
    {
        String keyS = "ssid" + String(i);
        String keyP = "pass" + String(i);

        if (saved_ssid[i].length() == 0)
        {
            prefs.remove(keyS.c_str());
            prefs.remove(keyP.c_str());
        }
        else
        {
            prefs.putString(keyS.c_str(), saved_ssid[i]);
            prefs.putString(keyP.c_str(), saved_pass[i]);
        }
    }
    prefs.end();
    CKC_LOG_DEBUG("WIFI", "SAVE NEW WIFI DONE: %s", newSSID.c_str());
}

template <class Transport>
inline void CKC_PnP<Transport>::SaveMQTT(String mqttuser, String mqttpass)
{
    // Không lưu nếu user rỗng
    if (mqttuser.length() == 0)
        return;

    // Load dữ liệu hiện tại
    loadMQTT();

    // =========================
    // USER & PASS GIỐNG -> SKIP
    // =========================
    if ((mqttuser == _mqtt_username) && (mqttpass == _mqtt_pass))
    {
        CKC_LOG_DEBUG("MQTT", "MQTT EXISTS -> SKIP");
        return;
    }

    // =========================
    // USER GIỐNG - PASS KHÁC
    // =========================
    if (mqttuser == _mqtt_username &&
        mqttpass != _mqtt_pass)
    {
        CKC_LOG_DEBUG("MQTT", "MQTT USER EXISTS -> UPDATE PASS");
    }
    else
    {
        // =========================
        // MQTT MỚI
        // =========================
        CKC_LOG_DEBUG("MQTT", "SAVE NEW MQTT");
    }

    // SAVE EEPROM
    if (!prefs.begin("mqtt", false))
        return;

    prefs.putString("user", mqttuser);
    prefs.putString("pass", mqttpass);

    prefs.end();

    // UPDATE RAM
    strcpy(_mqtt_username, mqttuser.c_str());
    strcpy(_mqtt_pass, mqttpass.c_str());

    CKC_LOG_DEBUG("MQTT", "SAVE MQTT DONE");
}

template <class Transport>
inline void CKC_PnP<Transport>::loadWiFi()
{
    if (!prefs.begin("wifi", true))
        return;
    for (int i = 0; i < Saved_WiFi_MAX; i++)
    {
        String ssid = prefs.getString(("ssid" + String(i)).c_str(), "");
        String pass = prefs.getString(("pass" + String(i)).c_str(), "");
        if (ssid.length() > 0)
        {
            saved_ssid[i] = ssid;
            saved_pass[i] = pass;
        }
        else
        {
            saved_ssid[i] = "";
            saved_pass[i] = "";
        }
    }
    CKC_LOG_DEBUG("MQTT", "LOAD WiFi DONE");
    prefs.end();
}

template <class Transport>
inline void CKC_PnP<Transport>::loadMQTT()
{
    memset(_mqtt_username, 0, sizeof(_mqtt_username));
    memset(_mqtt_pass, 0, sizeof(_mqtt_pass));
    if (!prefs.begin("mqtt", true))
        return;
    String user = prefs.getString("user", "");
    String pass = prefs.getString("pass", "");
    prefs.end();
    strcpy(_mqtt_username, user.c_str());
    strcpy(_mqtt_pass, pass.c_str());
    CKC_LOG_DEBUG("MQTT", "LOAD MQTT DONE");
}

template <class Transport>
inline void CKC_PnP<Transport>::handleScan()
{
    WiFi.scanDelete();
    delay(100);
    int n = WiFi.scanNetworks();
    if (n <= 0)
        return;
    CKC_LOG_DEBUG("WIFI", "Found %d networks", n);
    int count = min(n, Scan_WiFi_MAX);
    for (int i = 0; i < count; i++)
    {
        scan_ssid[i] = WiFi.SSID(i);
        scan_rssi[i] = WiFi.RSSI(i);
    }
    for (int i = count; i < Scan_WiFi_MAX; i++)
    {
        scan_ssid[i] = "";
        scan_rssi[i] = 0;
    }
    WiFi.scanDelete();
}

template <class Transport>
inline void CKC_PnP<Transport>::handleSave()
{
    newSSID = webServer.arg("ssid");
    newPASS = webServer.arg("pass");
    strcpy(_sta_ssid, newSSID.c_str());
    strcpy(_sta_pass, newPASS.c_str());
    String mqttUser = webServer.arg("mqtt_user");
    String mqttPass = webServer.arg("mqtt_pass");
    SaveMQTT(mqttUser, mqttPass);
    webServer.send(200, "text/html", CKC_WebUI::WebConfigCONNECT);
    WiFi.disconnect();
    isconnecting = false;
    resetAPMODE();
    WiFi_TASK = MODE_STA; // chuyển sang STA để test connect
}

template <class Transport>
inline void CKC_PnP<Transport>::CKC_SendPage()
{
    webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    webServer.send(200, "text/html", "");
    webServer.sendContent_P(CKC_WebUI::WebConfigHEAD);

    for (int i = 0; i < Scan_WiFi_MAX; i++)
    {
        if (scan_ssid[i] != "")
        {
            String opt = CKC_WebUI::buildWiFiOption(
                scan_ssid[i],
                scan_rssi[i]);
            webServer.sendContent(opt);
            yield();
        }
    }
    webServer.sendContent_P(CKC_WebUI::WebConfigFOOT);
}

template <class Transport>
inline void CKC_PnP<Transport>::Try_Connect()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        isconnecting = false;
        reconnect_count = 0;
        return;
    }
    if (isconnecting)
    {
        if (millis() - connectStart > time_sta)
        {
            isconnecting = false;
            reconnect_count++;
        }
        return;
    }
    if (millis() - lastAttemp < retryDelay)
        return;
    lastAttemp = millis();
    if (reconnect_count >= Saved_WiFi_MAX)
        reconnect_count = 0;
    String ssid = saved_ssid[reconnect_count];
    String pass = saved_pass[reconnect_count];
    if (ssid == "")
    {
        reconnect_count++;
        return;
    }
    CKC_LOG_DEBUG("WIFI", "TRY: %s", ssid.c_str());
    WiFi.disconnect();
    delay(100);
    WiFi.begin(ssid.c_str(), pass.c_str());
    isconnecting = true;
    connectStart = millis();
}

//========== STA MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_STA()
{
    webServer.stop();
    loadWiFi();
    loadMQTT();
    // =========================
    // 1. ƯU TIÊN WIFI INIT
    // =========================
    if (strlen(_sta_ssid) > 0)
    {
        t1 = millis();
        WiFi.mode(WIFI_STA);
        WiFi.disconnect();
        delay(200);
        WiFi.begin(_sta_ssid, _sta_pass);
        CKC_LOG_DEBUG("WIFI", "TRY CONNECT INIT WIFI: %s", _sta_ssid);
        while (WiFi.status() != WL_CONNECTED && millis() - t1 <= time_sta)
        {
            if (millis() - t0 >= 1000)
            {
                CKC_LOG_DEBUG("WIFI", "CONNECTING... %ds", (millis() - t1) / 1000);
                t0 = millis();
            }
            if (WiFi_TASK == MODE_AP)
            {
                CKC_LOG_DEBUG("WIFI", "BREAK CONNECT -> AP MODE");
                return;
            }
            delay(10);
            handler_button();
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            SaveWiFi(_sta_ssid, _sta_pass);
            Serial.println(WiFi.RSSI());
            STA();
            return;
        }
    }
    // =========================
    // 2. TRY CONNECT SAVED WIFI
    // =========================
    for (int j = 0; j < Saved_WiFi_MAX; j++)
    {
        if (saved_ssid[j].length() == 0)
            continue;
        CKC_LOG_DEBUG("WIFI", "FOUND MATCH: %s", saved_ssid[j].c_str());
        t1 = millis();
        WiFi.begin(saved_ssid[j].c_str(), saved_pass[j].c_str());
        while (WiFi.status() != WL_CONNECTED && millis() - t1 <= time_sta)
        {
            if (millis() - t0 > 1000)
            {
                CKC_LOG_DEBUG("WIFI", "CONNECTING... %ds", (millis() - t1) / 1000);
                t0 = millis();
            }
            if (WiFi_TASK == MODE_AP)
            {
                CKC_LOG_DEBUG("WIFI", "BREAK CONNECT -> AP MODE");
                return;
            }
            delay(10);
            handler_button();
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            SaveWiFi(saved_ssid[j].c_str(), saved_pass[j].c_str());
            Serial.println(WiFi.RSSI());
            STA();
            return;
        }
        CKC_LOG_DEBUG("WIFI", "FAILED WIFI: %s", saved_ssid[j].c_str());
    }
    // =========================
    // 3. FAIL -> AP MODE
    // =========================
    CKC_LOG_DEBUG("WIFI", "NO WIFI CONNECTED -> AP MODE");
    mqttClient.disconnect();
    WiFi_TASK = MODE_AP;
    delay(100);
}

template <class Transport>
inline void CKC_PnP<Transport>::STA()
{
    resetAPMODE();
    CKC_LOG_DEBUG("WIFI", "WIFI_CONNECTED!!!");
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", WiFi.localIP().toString());
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_MAC: %s", _mac);
    if (strlen(_mqtt_username) > 0)
    {
        CKC_LOG_DEBUG("MQTT", "USE EEPROM MQTT");
        serverMQTT.config(_mqtt_username, _mqtt_pass);
    }
    else
    {
        CKC_LOG_DEBUG("MQTT", "EEPROM EMPTY -> USE INIT MQTT");
        serverMQTT.config(mqttusername, mqttpass);
    }
    serverMQTT.begin();
    if (serverMQTT._connect())
    {
        CKC_LOG_DEBUG("TAG", "\r\n"
                             "  ____  _ __   ____ "
                             "\r\n"
                             " / ___|| |/ / / ___| "
                             "\r\n"
                             "| |    | ' / | | "
                             "\r\n"
                             "| |___ | . \\ | |___ "
                             "\r\n"
                             " \\____||_|\\_\\ \\____| "
                             "\r\n");
        SaveMQTT(mqttusername, mqttpass);
    }
    WiFi_TASK = MODE_CONNECTED;
}

//========== AP MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_AP()
{
    webServer.stop();
    dnsServer.stop();
    WiFi.mode(WIFI_AP_STA);
    delay(100);
    loadWiFi();   // load saved WiFi
    loadMQTT();   // load user&pass MQTT
    handleScan(); // scan WiFi xung quanh
    CKC_LOG_DEBUG("WIFI", "RUN_AP");
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_NAME: %s", _ap_ssid);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_PASS: %s", _ap_pass);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_IP: %s", _ap_ip);
    CKC_LOG_DEBUG("WIFI", "AP_WIFI_PORT: %s", _ap_port);
    IPAddress local_ip;
    local_ip.fromString(_ap_ip);
    WiFi.softAPConfig(local_ip, local_ip, WIFI_AP_Subnet);
    WiFi.softAP(_ap_ssid, _ap_pass);
    dnsServer.start(53, "*", WiFi.softAPIP());

    webServer.onNotFound([this]()
                         { 
    webServer.sendHeader("Location",String("http://") +WiFi.softAPIP().toString(),true);
    webServer.send(302, "text/plain", ""); });

    webServer.on("/", [this]()
                 {
        CKC_LOG_DEBUG("WIFI", "WEBSITE ON");
         setup_device_state = true;
         this -> CKC_SendPage(); });

    webServer.on("/scan", [this]()
                 {
        setup_device_state = true;
        WiFi.disconnect();
        isconnecting = false;
        this->handleScan();
        this -> CKC_SendPage(); });

    webServer.on("/connect", HTTP_POST, [this]()
                 { this->handleSave(); });

    webServer.begin();
    WiFi_TASK = RUN_AP_WEB;
}

template <class Transport>
inline void CKC_PnP<Transport>::resetAPMODE()
{
    triggered_AP = false;
    isconnecting = false;
    setup_device_state = false;
    CKC_LOG_DEBUG("WIFI", "RESET AP MODE!!!");
}

template <class Transport>
inline void CKC_PnP<Transport>::AP()
{
    mqttClient.disconnect();
    dnsServer.processNextRequest();
    this->webServer.handleClient();
    if (!triggered_AP)
    {
        if (!setup_device_state)
        {
            this->Try_Connect();
        }
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        resetAPMODE();
        CKC_LOG_DEBUG("WIFI", "CONNECTED -> EXIT AP");
        WiFi.softAPdisconnect(true);
        dnsServer.stop();
        STA();
    }
}

//========== CONNECTED MODE ==========//
template <class Transport>
inline bool CKC_PnP<Transport>::CkC_Connected()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <class Transport>
inline void CKC_PnP<Transport>::CKC_mode_connected()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        CKC_LOG_DEBUG("WIFI", "LOST WIFI !!!!!");
        mqttClient.disconnect();
        WiFi.disconnect(true, true);
        delay(100);
        WiFi_TASK = MODE_STA;
        return;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        if (serverMQTT._connect())
        {
            serverMQTT.run();
        }
        else
        {
            static unsigned long lastMQTTReconnect = 0;
            if (millis() - lastMQTTReconnect > 5000)
            {
                CKC_LOG_DEBUG("MQTT", "RECONNECTING__________");
                lastMQTTReconnect = millis();
                serverMQTT.begin();
            }
        }
    }
}

//================ BUTTON ================//
template <class Transport>
inline void CKC_PnP<Transport>::handler_button()
{
#ifdef BUTTON_MODE
    bool pressed = (digitalRead(FLASH_BTN) == LOW);
    if (pressed)
    {
        if (pressStart == 0)
            pressStart = millis();
        if (!triggered && (millis() - pressStart >= 5000))
        {
            mqttClient.disconnect();
            WiFi.disconnect(true, true);
            WiFi.mode(WIFI_OFF);
            delay(300);
            triggered = true;
            triggered_AP = true;
            isconnecting = false;
            WiFi_TASK = MODE_AP;
            CKC_LOG_DEBUG("WIFI", "BUTTON TRIGGER");
        }
    }
    else
    {
        pressStart = 0;
        triggered = false;
    }
#endif
}

//================ RUN ================//
template <class Transport>
inline void CKC_PnP<Transport>::run()
{
    switch (WiFi_TASK)
    {
    case MODE_STA:
        this->CKC_state_Connect_STA();
        break;
    case MODE_AP:
        this->CKC_state_Connect_AP();
        break;
    case MODE_CONNECTED:
        this->CKC_mode_connected();
        break;
    case RUN_AP_WEB:
        this->AP();
        break;
    default:
        break;
    }
    this->handler_button();
}
#endif
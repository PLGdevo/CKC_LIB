#ifndef CKC_ESP32
#define CKC_ESP32
#include <Arduino.h>
#include <WiFiClient.h>
#include <AIoT/CKC_API.hpp>
#include <MQTT/ESP32_MQTT.hpp>
#include <WebServer.h>
#include <Preferences.h>

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
    MODE_CONFIG_WIFI,
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

public:
    CKC_PnP() {};
    void init(const char *sta_ssid, const char *sta_pass);
    void init(const char *sta_ssid, const char *sta_pass, const char *mqtt_userName, const char *mqtt_pass);
    void SaveWiFi(String newSSID, String newPASS);
    void SaveMQTT(String mqttuser, String mqttpass);
    void handleSave();
    void loadWiFi();
    void loadMQTT();
    void handleScan();
    void CKC_state_Connect_STA();
    void STA();
    void CKC_state_Connect_AP();
    void AP();
    void Try_Connect();
    void CKC_mode_connected();
    bool CkC_Connected();
    void run();
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

    char _mqtt_username[64];
    char _mqtt_pass[64];

    unsigned long t0, t1, t2, t3, t4;
    int time_sta = 10000;

    int reconnect_count = 0;
    unsigned long lastAttemp = 0;
    const unsigned long retryDelay = 10000;
    bool isconnecting = false;
    unsigned long connectStart = 0;

    bool manual_config = false;
    unsigned long lastUserconfig = 0;
    const unsigned long userTimeout = 120000;

#define FLASH_BTN 0 // nút BOOT/FLASH trên ESP32 thường là GPIO0
    unsigned long pressStart = 0;
    bool triggered = false;

    String newSSID;
    String newPASS;

#define WiFi_MAX 5
    // ===== WIFI ĐÃ LƯU (AUTO CONNECT) =====
    String saved_ssid[WiFi_MAX];
    String saved_pass[WiFi_MAX];

    // ===== WIFI SCAN (HIỂN THỊ WEB) =====
    String scan_ssid[WiFi_MAX];
    int scan_rssi[WiFi_MAX];
};

template <class Transport>
inline void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass)
{
    WiFi.mode(WIFI_STA);
    delay(500);
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
    delay(500);
    strcpy(_sta_ssid, sta_ssid);
    strcpy(_sta_pass, sta_pass);
    String MAC = WiFi.macAddress();
    strcpy(_mac, MAC.c_str());
    snprintf(_ap_ssid, sizeof(_ap_ssid), "%s%s", _ap_ssid, _mac);
    serverMQTT.config(mqtt_userName, mqtt_pass);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_NAME: %s", _sta_ssid);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PASS: %s", _sta_pass);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", _sta_ip);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    t1 = millis();
    pinMode(FLASH_BTN, INPUT_PULLUP); // nút kéo xuống GND khi nhấn
}

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
    for (int i = 0; i < WiFi_MAX; i++)
    {
        if (saved_ssid[i] == newSSID)
        {
            CKC_LOG_DEBUG("WIFI", "SSID EXISTS -> SKIP SAVE");
            prefs.end();
            return;
        }
    }
    for (int i = WiFi_MAX - 1; i > 0; i--)
    {
        saved_ssid[i] = saved_ssid[i - 1];
        saved_pass[i] = saved_pass[i - 1];
    }
    saved_ssid[0] = newSSID;
    saved_pass[0] = newPASS;
    for (int i = 0; i < WiFi_MAX; i++)
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
    CKC_LOG_DEBUG("WIFI", "SAVE WIFI DONE: %s", newSSID.c_str());
}

template <class Transport>
inline void CKC_PnP<Transport>::SaveMQTT(String mqttuser, String mqttpass)
{
    if (!prefs.begin("mqtt", false))
        return;
    prefs.putString("user", mqttuser);
    prefs.putString("pass", mqttpass);
    prefs.end();
    strcpy(_mqtt_username, mqttuser.c_str());
    strcpy(_mqtt_pass, mqttpass.c_str());
    CKC_LOG_DEBUG("MQTT", "SAVE MQTT DONE");
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
    webServer.send(200, "text/html", "<h3>WIFI CONNECTING...!!!</h3>");
    manual_config = false;
    WiFi_TASK = MODE_STA; // chuyển sang STA để test connect
}

template <class Transport>
inline void CKC_PnP<Transport>::loadWiFi()
{
    if (!prefs.begin("wifi", true))
        return;
    for (int i = 0; i < WiFi_MAX; i++)
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
    prefs.end();
}

template <class Transport>
inline void CKC_PnP<Transport>::loadMQTT()
{
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
    int n = WiFi.scanNetworks();
    if (n <= 0)
        return;
    CKC_LOG_DEBUG("WIFI", "Found %d networks", n);
    int count = min(n, WiFi_MAX);
    for (int i = 0; i < count; i++)
    {
        scan_ssid[i] = WiFi.SSID(i);
        scan_rssi[i] = WiFi.RSSI(i);
    }
    for (int i = count; i < WiFi_MAX; i++)
    {
        scan_ssid[i] = "";
        scan_rssi[i] = 0;
    }
    WiFi.scanDelete();
}

template <class Transport>
inline const char CKC_PnP<Transport>::WebConfigHEAD[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">
<head>
<meta charset="UTF-8">
<title>WiFi CONFIG</title>

<meta name="viewport"
      content="width=device-width, initial-scale=1.0">

<style>
  :root {
            --primary: #4CAF93;
            --bg: #f5f7f9;
            --card: #ffffff;
            --text: #2c3e50;
            --border: #e0e6ed;
        }

        body {
            font-family: "Segoe UI", sans-serif;
            background: var(--bg);
            margin: 0;
            padding: 20px;
            color: var(--text);
        }

        .container {
            max-width: 400px;
            margin: auto;
        }

        .card {
            background: var(--card);
            padding: 25px;
            border-radius: 14px;
            box-shadow: 0 6px 20px rgba(0,0,0,0.05);
        }

        h2 {
            margin-bottom: 20px;
            text-align: center;
        }

        label {
            display: block;
            margin-top: 12px;
            font-weight: 500;
        }

        select,
        input {
            width: 100%;
            padding: 12px;
            border-radius: 10px;
            border: 1px solid var(--border);
            margin-top: 8px;
            box-sizing: border-box;
        }

        button,
        input[type="submit"] {
            width: 100%;
            padding: 12px;
            border-radius: 10px;
            border: none;
            background: var(--primary);
            color: white;
            margin-top: 15px;
            cursor: pointer;
            font-size: 15px;
        }

        hr {
            border: none;
            border-top: 1px solid var(--border);
            margin: 25px 0;
        }

        .select-wrap {
            position: relative;
        }

        #signalView {
            position: absolute;
            right: 35px;
            top: 50%;
            transform: translateY(-50%);
            pointer-events: none;
        }

        .signal {
            display: flex;
            gap: 3px;
            align-items: flex-end;
        }

        .bar {
            width: 5px;
            border-radius: 3px;
            background: #d0d7de;
        }

        .bar.active {
            background: var(--primary);
        }

        .bar:nth-child(1) { height: 6px; }
        .bar:nth-child(2) { height: 10px; }
        .bar:nth-child(3) { height: 14px; }
        .bar:nth-child(4) { height: 18px; }

        .level-1 .bar:nth-child(n+2),
        .level-2 .bar:nth-child(n+3),
        .level-3 .bar:nth-child(4) {
            background: #d0d7de !important;
        }
       .password-box {
    position: relative;
    width: 100%;
}

.password-box input {
    width: 100%;
    padding-right: 45px;
}

.toggle-eye {
    position: absolute;
    right: 14px;
    top: 50%;
    transform: translateY(-50%);
    cursor: pointer;
    user-select: none;
    font-size: 16px;
    opacity: 0.7;
}

.toggle-eye:hover {
    opacity: 1;     
</style>
</head>

<body>

<div class="container">

<div class="card">

<h2>WIFI & MQTT CONFIG</h2>

<form action="/connect" method="POST">

<label>SSID</label>

<div class="select-wrap">

<select id="wifiSelect" name="ssid">

)rawliteral";

template <class Transport>
inline const char CKC_PnP<Transport>::WebConfigFOOT[] PROGMEM = R"rawliteral(
</select>

<div id="signalView"></div>

</div>

<label>WiFi Password</label>

<div class="password-box">

    <input type="password"
           id="wifiPass"
           name="pass"
           placeholder="Enter WiFi password">

    <span class="toggle-eye"
          onclick="togglePassword('wifiPass', this)">
        👁
    </span>

</div>

<label>MQTT Username</label>

<input type="text"
       name="mqtt_user"
       placeholder="Enter MQTT username">

<label>MQTT Password</label>

<div class="password-box">

    <input type="password"
           id="mqttPass"
           name="mqtt_pass"
           placeholder="Enter MQTT password">

    <span class="toggle-eye"
          onclick="togglePassword('mqttPass', this)">
        👁
    </span>

</div>

<input type="submit"
       value="CONNECT">

</form>

<hr>

<form action="/scan" method="GET">

<button type="submit">
    RELOAD WIFI
</button>

</form>

</div>

</div>

<script>

const select = document.getElementById("wifiSelect");
const signalView = document.getElementById("signalView");

function renderSignal(level)
{
    let html =
        '<div class="signal level-' + level + '">';

    for (let i = 0; i < 4; i++)
    {
        html += '<span class="bar active"></span>';
    }

    html += '</div>';

    signalView.innerHTML = html;
}

select.addEventListener("change", function ()
{
    let level =
        this.options[this.selectedIndex]
        .getAttribute("data-level");

    renderSignal(level);
});

window.onload = function ()
{
    if (select.options.length > 0)
    {
        let level =
            select.options[select.selectedIndex]
            .getAttribute("data-level");

        renderSignal(level);
    }
};

function togglePassword(inputId, eye)
{
    const input = document.getElementById(inputId);

    if (input.type === "password")
    {
        input.type = "text";
        eye.innerHTML = "👁";
    }
    else
    {
        input.type = "password";
        eye.innerHTML = "👁";
    }
}


</script>

</body>
</html>

)rawliteral";

template <class Transport>
inline void CKC_PnP<Transport>::CKC_SendPage()
{
    webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
    webServer.send(200, "text/html", "");
    webServer.sendContent_P(WebConfigHEAD);
    for (int i = 0; i < WiFi_MAX; i++)
    {
        if (scan_ssid[i] != "")
        {
            char option[256];
            int rssi = scan_rssi[i];
            int level = 1;
            if (rssi > -60)
                level = 4;
            else if (rssi > -70)
                level = 3;
            else if (rssi > -80)
                level = 2;
            snprintf(option,
                     sizeof(option),
                     "<option value='%s' data-level='%d'>%s (%d dBm)</option>",
                     scan_ssid[i].c_str(),
                     level,
                     scan_ssid[i].c_str(),
                     rssi);
            webServer.sendContent(option);
            yield();
        }
    }
    webServer.sendContent_P(WebConfigFOOT);
}

template <class Transport>
inline void CKC_PnP<Transport>::Try_Connect()
{
    if (WiFi.status() == WL_CONNECTED)
        return;
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
    if (reconnect_count >= WiFi_MAX)
        reconnect_count = 0;
    String ssid = saved_ssid[reconnect_count];
    String pass = saved_pass[reconnect_count];
    if (ssid == "")
    {
        reconnect_count++;
        return;
    }
    CKC_LOG_DEBUG("WIFI", "TRY: %s", ssid.c_str());
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
        WiFi.disconnect(true);
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
            STA();
            return;
        }
    }
    // =========================
    // 2. SCAN WIFI XUNG QUANH
    // =========================
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    delay(200);
    CKC_LOG_DEBUG("WIFI", "SCANNING WIFI...");
    int n = WiFi.scanNetworks();
    if (n <= 0)
    {
        CKC_LOG_DEBUG("WIFI", "NO WIFI FOUND -> SWITCH AP");
        mqttClient.disconnect();
        WiFi_TASK = MODE_AP;
        return;
    }
    // =========================
    // 3. TRY CONNECT SAVED WIFI
    // =========================
    for (int j = 0; j < WiFi_MAX; j++)
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
            STA();
            return;
        }
        CKC_LOG_DEBUG("WIFI", "FAILED WIFI: %s", saved_ssid[j].c_str());
    }
    // =========================
    // 4. FAIL -> AP MODE
    // =========================
    CKC_LOG_DEBUG("WIFI", "NO WIFI CONNECTED -> AP MODE");
    mqttClient.disconnect();
    manual_config = false;
    WiFi_TASK = MODE_AP;
    delay(100);
}

template <class Transport>
inline void CKC_PnP<Transport>::STA()
{
    CKC_LOG_DEBUG("WIFI", "WIFI_CONNECTED!!!");
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", WiFi.localIP().toString());
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_MAC: %s", _mac);
    serverMQTT.config(_mqtt_username, _mqtt_pass);
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
    }
    WiFi_TASK = MODE_CONNECTED;
}

//========== AP MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_AP()
{
    WiFi.mode(WIFI_AP_STA);
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
    webServer.on("/", [this]()
                 {
                    CKC_LOG_DEBUG("WIFI", "WEBSITE ON");
        lastUserconfig = millis();
         this -> CKC_SendPage(); });
    webServer.on("/connect", HTTP_POST, [this]()
                 { this->handleSave(); });
    webServer.on("/scan", [this]()
                 {
        lastUserconfig = millis();
        this->handleScan();
        this -> CKC_SendPage(); });
    // this->webServer.send(200, "text/html", this->htmlPage()); });
    webServer.begin();

    WiFi_TASK = RUN_AP_WEB;
}

template <class Transport>
inline void CKC_PnP<Transport>::AP()
{
    mqttClient.disconnect();
    this->webServer.handleClient();

    if (manual_config)
    {
        return; // không auto connect
    }

    if (millis() - lastUserconfig < userTimeout)
    {
        return; // thời gian để User có thể vào Web
    }
    this->Try_Connect();

    if (WiFi.status() == WL_CONNECTED)
    {
        CKC_LOG_DEBUG("WIFI", "CONNECTED -> EXIT AP");
        WiFi.softAPdisconnect(true);
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
    if (this -> !CkC_Connected())
    {
        CKC_LOG_DEBUG("WIFI", "LOST WIFI !!!!!");
        mqttClient.disconnect();
        WiFi_TASK = MODE_STA;
        return;
    }
    if (this -> CkC_Connected())
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
                CKC_LOG_DEBUG("MQTT", "RECONNECTING --> SERVER");
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
            triggered = true;
            mqttClient.disconnect();
            manual_config = true;
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
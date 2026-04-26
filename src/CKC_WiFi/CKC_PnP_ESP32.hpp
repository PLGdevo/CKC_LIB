#ifndef CKC_ESP32
#define CKC_ESP32
#include <Arduino.h>
#include <WiFiClient.h>
#include <CKC/CKC_API.hpp>
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
    void SaveWiFi(String newSSID, String newPASS);
    void handleSave();
    void loadWiFi();
    void handleScan();
    void CKC_state_Connect_STA();
    void STA();
    void CKC_state_Connect_AP();
    void CKC_mode_connected();
    bool CkC_Connected();
    void run();
    String htmlPage();
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

    unsigned long t0, t1, t2, t3, t4;
    int time_sta = 10000;

#define FLASH_BTN 0 // nút BOOT/FLASH trên ESP32 thường là GPIO0
    unsigned long pressStart = 0;
    bool triggered = false;

    String newSSID;
    String newPASS;
#define WiFi_MAX 5
    String ssid_list[WiFi_MAX];
    String pass_list[WiFi_MAX];
    int RSSI_list[WiFi_MAX];
};
template <class Transport>
inline void CKC_PnP<Transport>::init(const char *sta_ssid, const char *sta_pass)
{
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
inline void CKC_PnP<Transport>::SaveWiFi(String newSSID, String newPASS)
{
    prefs.begin("wifi", false);
    for (int i = WiFi_MAX - 1; i > 0; i--)
    {
        prefs.putString(("ssid" + String(i)).c_str(), ssid_list[i - 1]);
        prefs.putString(("pass" + String(i)).c_str(), pass_list[i - 1]);
    }
    prefs.putString("ssid0", newSSID);
    prefs.putString("pass0", newPASS);
    prefs.end();
}

template <class Transport>
inline void CKC_PnP<Transport>::handleSave()
{
    newSSID = webServer.arg("ssid");
    newPASS = webServer.arg("pass");
    strcpy(_sta_ssid, newSSID.c_str());
    strcpy(_sta_pass, newPASS.c_str());
    webServer.send(200, "text/html", "<h3>WIFI CONNECTING...!!!</h3>");
    WiFi_TASK = MODE_STA; // chuyển sang STA để test connect
}

template <class Transport>
inline void CKC_PnP<Transport>::loadWiFi()
{
    prefs.begin("wifi", true);
    for (int i = 0; i < WiFi_MAX; i++)
    {
        ssid_list[i] = prefs.getString(("ssid" + String(i)).c_str(), "");
        pass_list[i] = prefs.getString(("pass" + String(i)).c_str(), "");
    }
    prefs.end();
}

template <class Transport>
inline void CKC_PnP<Transport>::handleScan()
{
    // CKC_LOG_DEBUG("WIFI", "WiFi SCANNING !!!!!!");
    int n = WiFi.scanNetworks();
    if (n <= 0)
    {
        // CKC_LOG_DEBUG("WIFI", "NO WIFI FOUND");
        return;
    }
    CKC_LOG_DEBUG("WIFI", "Found %d networks", n);
    int count = min(n, WiFi_MAX);
    for (int i = 0; i < count; i++)
    {
        ssid_list[i] = WiFi.SSID(i);
        RSSI_list[i] = WiFi.RSSI(i);
        // CKC_LOG_DEBUG("WIFI", "WiFi %d", i);
        // CKC_LOG_DEBUG("WIFI", "SSID: %s", ssid_list[i].c_str());
        // CKC_LOG_DEBUG("WIFI", "RSSI: %d dBm", WiFi.RSSI(i));
    }
    for (int i = count; i < WiFi_MAX; i++)
    {
        ssid_list[i] = "";
    }
    WiFi.scanDelete();
}
template <class Transport>
inline String CKC_PnP<Transport>::htmlPage()
{
    String options = "";
    for (int i = 0; i < WiFi_MAX; i++)
    {
        if (ssid_list[i] != "")
        {
            int rssi = RSSI_list[i];
            int level = 1;
            if (rssi > -60)
                level = 4;
            else if (rssi > -70)
                level = 3;
            else if (rssi > -80)
                level = 2;
            else
                level = 1;
            options += "<option value='" + ssid_list[i] + "' data-level='" + String(level) + "'>";
            options += ssid_list[i] + " (" + String(rssi) + " dBm)";
            options += "</option>";
        }
    }
    String page = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">

<head>
    <meta charset="UTF-8">
    <title>WiFi CONFIG</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
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
            box-shadow: 0 6px 20px rgba(0, 0, 0, 0.05);
        }

        h2 {
            margin-bottom: 20px;
            text-align: center
        }

        select,
        input {
            width: 100%;
            padding: 12px;
            border-radius: 10px;
            border: 1px solid var(--border);
            margin-top: 8px;
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
        }

        hr {
            border: none;
            border-top: 1px solid var(--border);
            margin: 25px 0;
        }

        /* SIGNAL */
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

        .bar:nth-child(1) {
            height: 6px;
        }

        .bar:nth-child(2) {
            height: 10px;
        }

        .bar:nth-child(3) {
            height: 14px;
        }

        .bar:nth-child(4) {
            height: 18px;
        }

        .level-1 .bar:nth-child(n+2),
        .level-2 .bar:nth-child(n+3),
        .level-3 .bar:nth-child(4) {
            background: #d0d7de !important;
        }
    </style>
</head>

<body>
    <div class="container">
        <div class="card">
            <h2>WIFI CONNECTING SITE</h2>
            <form action="/connect" method="POST"> <label>SSID</label>
                <div class="select-wrap"> <select id="wifiSelect" name="ssid"> )rawliteral";
    page += options;
    page += R"rawliteral( </select>
                    <div id="signalView"></div>
                </div> <label>Password</label> <input type="password" name="pass"> <input type="submit" value="CONNECT">
            </form>
            <hr>
            <form action="/scan" method="GET"> <button type="submit">RELOAD WIFI</button> </form>
        </div>
    </div>
    <script> const select = document.getElementById("wifiSelect"); const signalView = document.getElementById("signalView"); function renderSignal(level) { let html = '<div class="signal level-' + level + '">'; for (let i = 0; i < 4; i++) { html += '<span class="bar active"></span>'; } html += '</div>'; signalView.innerHTML = html; } select.addEventListener("change", function () { let level = this.options[this.selectedIndex].getAttribute("data-level"); renderSignal(level); }); window.onload = function () { let level = select.options[select.selectedIndex].getAttribute("data-level"); renderSignal(level); }; </script>
</body>

</html>
)rawliteral";
    return page;
}

//========== STA MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_STA()
{
    webServer.stop();
    this->loadWiFi();
    // ==============================
    // ƯU TIÊN WIFI TỪ INIT()
    // ==============================
    if (strlen(_sta_ssid) > 0)
    {
        t1 = millis();
        WiFi.mode(WIFI_STA);
        WiFi.disconnect(true);
        delay(200);
        WiFi.begin(_sta_ssid, _sta_pass);
        CKC_LOG_DEBUG("WIFI", "TRY CONNECTING TO: %s", _sta_ssid);
        while (WiFi.status() != WL_CONNECTED && millis() - t1 <= this->time_sta)
        {
            if (millis() - t0 > 1000)
            {
                CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
                t0 = millis();
            }
            if (WiFi_TASK == MODE_AP)
            {
                CKC_LOG_DEBUG("WIFI", "BREAK WIFI CONNECT !!!!!! ");
                return;
            }
            delay(10);
            handler_button();
        }
        if (WiFi.status() == WL_CONNECTED)
        {
            this->STA();
            return;
        }
    }
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    delay(200);
    CKC_LOG_DEBUG("WIFI", "LOAD WIFI FORM ESP32 !!!!!! ");
    int n = WiFi.scanNetworks();
    if (n <= 0)
    {
        CKC_LOG_DEBUG("WIFI", "NO_WIFI_CONNECT !!!!!! ");
        mqttClient.disconnect();
        WiFi_TASK = MODE_AP;
        return;
    }
    for (int j = 0; j < WiFi_MAX; j++)
    {
        if (ssid_list[j] == "")
            continue;
        for (int i = 0; i < n; i++)
        {
            if (WiFi.SSID(i) == ssid_list[j])
            {
                CKC_LOG_DEBUG("WIFI", "Connecting to: %s", ssid_list[j].c_str());
                t1 = millis();
                WiFi.begin(ssid_list[j].c_str(), pass_list[j].c_str());
                while (WiFi.status() != WL_CONNECTED && millis() - t1 <= this->time_sta)
                {
                    if (millis() - t0 > 1000)
                    {
                        CKC_LOG_DEBUG("WIFI", "CONNECTING ___ %ds\r", (millis() - t1) / 1000);
                        t0 = millis();
                    }
                    if (WiFi_TASK == MODE_AP)
                    {
                        CKC_LOG_DEBUG("WIFI", "BREAK WIFI CONNECT !!!!!! ");
                        return;
                    }
                    delay(10);
                    handler_button();
                }
                if (WiFi.status() == WL_CONNECTED)
                {
                    this->STA();
                    return;
                }
                else
                {
                    CKC_LOG_DEBUG("WIFI", "WIFI_CONNECT_FALSE !!!!!! ");
                    CKC_LOG_DEBUG("WIFI", "TRY_ANOTHER_WIFI!!!");
                }
            }
        }
    }
    CKC_LOG_DEBUG("WIFI", "NO_WIFI_CONNECT !!!!!! ");
    mqttClient.disconnect();
    WiFi_TASK = MODE_AP;
    delay(100);
}

template <class Transport>
inline void CKC_PnP<Transport>::STA()
{
    SaveWiFi(String(_sta_ssid), String(_sta_pass));
    CKC_LOG_DEBUG("WIFI", "WIFI_CONNECTED!!!");
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_IP: %s", WiFi.localIP().toString());
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_PORT: %s", _sta_port);
    CKC_LOG_DEBUG("WIFI", "STA_WIFI_MAC: %s", _mac);
    serverMQTT.begin();
    CKC_LOG_DEBUG("TAG", "\r\n"
                         " ____ _ __ ____ "
                         "\r\n"
                         " / ___|| |/ / / ___| "
                         "\r\n"
                         "| | | ' / | | "
                         "\r\n"
                         "| |___ | . \\ | |___ "
                         "\r\n"
                         " \\____||_|\\_\\ \\____| "
                         "\r\n");
    WiFi_TASK = MODE_CONNECTED;
}

//========== AP MODE ==========//
template <class Transport>
inline void CKC_PnP<Transport>::CKC_state_Connect_AP()
{
    WiFi.mode(WIFI_AP_STA);
    this->handleScan();
    delay(100);
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
                 { this->webServer.send(200, "text/html", this->htmlPage()); });
    webServer.on("/connect", HTTP_POST, [this]()
                 { this->handleSave(); });
    webServer.on("/scan", [this]()
                 { this->handleScan(); this->webServer.send(200, "text/html", this->htmlPage()); });
    webServer.begin();
    mqttClient.disconnect();
    delay(1000);
    WiFi_TASK = RUN_AP_WEB;
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
    if (!CkC_Connected())
    {
        CKC_LOG_DEBUG("WIFI", "LOST WIFI !!!!!");
        mqttClient.disconnect();
        WiFi_TASK = MODE_STA;
        return;
    }
    if (CkC_Connected())
    {
        serverMQTT.run();
        if (!serverMQTT._connect())
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
            triggered = true;
            mqttClient.disconnect();
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
        mqttClient.disconnect();
        this->webServer.handleClient();
        break;
    default:
        break;
    }
    this->handler_button();
}
#endif
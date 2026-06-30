#ifndef CKC_WEBUI_HPP
#define CKC_WEBUI_HPP

#include <Arduino.h>

class CKC_WebUI
{
public:
    static const char WebConfigHEAD[];
    static const char WebConfigFOOT[];
    static const char WebConfigCONNECT[];

    static String buildWiFiOption(const String &ssid, int rssi);
    static int getSignalLevel(int rssi);
};

// =====================================================
// HTML HEAD
// =====================================================
const char CKC_WebUI::WebConfigHEAD[] PROGMEM = R"rawliteral(
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
            margin: 10px 0;
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

        .dropdown{
            margin-top:15px;
            max-height:180px;
            overflow-y:auto;
            border:1px solid #e0e6ed;
            border-radius:10px;
            background:#fff;
        }

        .dropdown .title{
            padding:14px 16px;
            font-size:18px;
            font-weight:bold;
            border-bottom:1px solid #eee;
        }

        .option{
            display:flex;
            justify-content:space-between;
            align-items:center;
            padding:14px 16px;
            cursor:pointer;
            border-bottom:1px solid #eee;
            transition:.2s;
        }

        .option:last-child{
            border-bottom:none;
        }

        .option:hover{
            background:#f5f7f9;
        }

        .signal{
            display:flex;
            gap:2px;
            align-items:flex-end;
        }

        .bar{
            width:4px;
            background:#d0d7de;
            border-radius:2px;
        }

        .bar:nth-child(1){height:6px;}
        .bar:nth-child(2){height:9px;}
        .bar:nth-child(3){height:12px;}
        .bar:nth-child(4){height:15px;}

        .bar.active{
            background:#4CAF93;
        }

        .toggle-eye:hover {
            opacity: 1;}
    </style>
</head>

<body>

    <div class="container">

        <div class="card">

            <h2>SET UP DEVICE</h2>

            <form action="/connect" method="POST">

                <label>SSID</label>

                <div class="select-wrap">
                <input
                type="text"
                id="wifiSelect"
                name="ssid"
                placeholder="Select WiFi"
                />                

                </div>

                <label>WiFi Password</label>

                <div class="password-box">

                    <input type="password" id="wifiPass" name="pass" placeholder="Enter WiFi password">

                    <span class="toggle-eye" onclick="togglePassword('wifiPass', this)">
                        👁
                    </span>

                </div>

                <label>MQTT Username</label>

                <input
                    type="text"
                    id="mqttUser"
                    name="mqtt_user"
                    placeholder="Enter MQTT username">

                <label>MQTT Password</label>

                <div class="password-box">
                    <input
                            type="password"
                            id="mqttPass"
                            name="mqtt_pass"
                            placeholder="Enter MQTT password">

                    <span class="toggle-eye" onclick="togglePassword('mqttPass', this)">
                    👁
                    </span>
                </div>

                <input type="submit" value="CONNECT" />

            </form>

        </div>

            <hr> 
                <div class="card">

                    <form action="/scan" method="GET">
                        <button type="submit">
                            RELOAD WIFI
                        </button>
                    </form>

                    <div class="dropdown" id="wifiList">

                        <div class="title">
                            Scan Wi-Fi
                        </div>
)rawliteral";
// =====================================================
// HTML FOOT
// =====================================================
const char CKC_WebUI::WebConfigFOOT[] PROGMEM = R"rawliteral(
                       </div>      <!-- dropdown -->
                </div>      <!-- card -->
    </div>      <!-- container --> 
    <script>

        const CONFIG = {
                            mqttUser: "%MQTT_USER%",
                            mqttPass: "%MQTT_PASS%"
                        };

        document.getElementById("mqttUser").value = CONFIG.mqttUser;
        document.getElementById("mqttPass").value = CONFIG.mqttPass;


        const select = document.getElementById("wifiSelect");
        

        function selectWifi(name)
        {
            document.getElementById("wifiSelect").value = name;
        }        

        window.onload = function () {
            if (select.options.length > 0) {
                let level =
                    select.options[select.selectedIndex]
                        .getAttribute("data-level");

                renderSignal(level);
            }
        };

        function togglePassword(inputId, eye) {
            const input = document.getElementById(inputId);

            if (input.type === "password") {
                input.type = "text";
                eye.innerHTML = "👁";
            }
            else {
                input.type = "password";
                eye.innerHTML = "👁";
            }
        }

    </script>
</body>

</html>
)rawliteral";

const char CKC_WebUI::WebConfigCONNECT[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">

<head>
    <title>Cấu hình WiFi thành công | WiFi Config Success</title>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600;700;800&display=swap" rel="stylesheet">
    <style>
        :root {
--bg-color: #f5f7fb;
    --surface-color: #ffffff;

    --accent-color: #32d6a0;
    --accent-glow: rgba(50, 255, 255, 0.3);

    --text-primary: #1f2937;
    --text-secondary: #6b7280;

    --border-color: rgba(0,0,0,0.15);
        }

        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }

        body {
            font-family: 'Inter', -apple-system, sans-serif;
            background-color: var(--bg-color);
            color: var(--text-primary);
            height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            overflow: hidden;
            position: relative;
            letter-spacing: -0.01em;
        }

        /* Subtle background glow */
        .bg-glow {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: 
                radial-gradient(circle at 10% 20%, rgba(210, 255, 50, 0.05) 0%, transparent 40%),
                radial-gradient(circle at 90% 80%, rgba(210, 255, 50, 0.03) 0%, transparent 40%);
            pointer-events: none;
            z-index: -1;
        }

        .container {
            position: relative;
            width: 100%;
            max-width: 480px;
            padding: 24px;
            z-index: 10;
        }

        .card {
            background: var(--surface-color);
            border: 1px solid var(--border-color);
            border-radius: 32px;
            padding: 60px 40px;
            text-align: center;
            box-shadow: 0 10px 25px rgba(15,23,42,0.08);
            animation: cardIn 0.8s cubic-bezier(0.16, 1, 0.3, 1) forwards;
        }

        @keyframes cardIn {
            from { opacity: 0; transform: translateY(30px) scale(0.98); }
            to { opacity: 1; transform: translateY(0) scale(1); }
        }

        /* Success Icon */
        .icon-container {
            margin-bottom: 32px;
            display: inline-flex;
            justify-content: center;
            align-items: center;
            position: relative;
        }

        .icon-bg {
            position: absolute;
            width: 110px;
            height: 110px;
            background: var(--accent-glow);
            border-radius: 50%;
            filter: blur(25px);
            animation: pulse 2.5s infinite;
        }

        @keyframes pulse {
            0%, 100% { transform: scale(1); opacity: 0.4; }
            50% { transform: scale(1.15); opacity: 0.7; }
        }

        .checkmark-svg {
            width: 90px;
            height: 90px;
            position: relative;
            z-index: 2;
        }

        .checkmark-circle {
            fill: none;
            stroke: var(--accent-color);
            stroke-width: 3;
            stroke-dasharray: 166;
            stroke-dashoffset: 166;
            stroke-linecap: round;
            animation: drawCircle 0.8s cubic-bezier(0.65, 0, 0.45, 1) 0.2s forwards;
        }

        .checkmark-check {
            fill: none;
            stroke: #000;
            stroke-width: 4;
            stroke-dasharray: 48;
            stroke-dashoffset: 48;
            stroke-linecap: round;
            stroke-linejoin: round;
            animation: drawCheck 0.5s cubic-bezier(0.65, 0, 0.45, 1) 0.9s forwards;
        }

        @keyframes drawCircle { to { stroke-dashoffset: 0; } }
        @keyframes drawCheck { to { stroke-dashoffset: 0; } }

        h1 {
            font-size: 28px;
            font-weight: 800;
            margin-bottom: 12px;
            letter-spacing: -0.03em;
            opacity: 0;
            animation: fadeIn 0.8s ease-out 1s forwards;
        }

        h2 {
            font-size: 18px;
            font-weight: 400;
            color: var(--text-secondary);
            margin-bottom: 32px;
            opacity: 0;
            animation: fadeIn 0.8s ease-out 1.2s forwards;
        }

        .status-box {
            background: rgba(255, 255, 255, 0.03);
            border: 1px solid var(--border-color);
            border-radius: 20px;
            padding: 20px;
            opacity: 0;
            animation: fadeIn 0.8s ease-out 1.4s forwards;
        }

        .status-text {
            color: var(--text-secondary);
            font-size: 14px;
            line-height: 1.8;
            display: flex;
            flex-direction: column;
            gap: 6px;
        }

        .bold-text {
            color: var(--text-primary);
            font-weight: 600;
        }

        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(10px); }
            to { opacity: 1; transform: translateY(0); }
        }

        /* Loading Dots */
        .dots {
            display: inline-flex;
            gap: 4px;
            margin-left: 6px;
        }

        .dot {
            width: 4px;
            height: 4px;
            background: var(--accent-color);
            border-radius: 50%;
            animation: bounce 1.4s infinite ease-in-out both;
        }

        .dot:nth-child(1) { animation-delay: -0.32s; }
        .dot:nth-child(2) { animation-delay: -0.16s; }

        @keyframes bounce {
            0%, 80%, 100% { transform: scale(0); }
            40% { transform: scale(1); }
        }

        footer {
            position: absolute;
            bottom: 30px;
            width: 100%;
            text-align: center;
            color: var(--text-secondary);
            font-size: 13px;
            opacity: 0;
            animation: fadeIn 1s ease-out 1.8s forwards;
        }

        footer b { color: var(--accent-color); }

        @media (max-width: 480px) {
            .card { padding: 40px 24px; }
            h1 { font-size: 24px; }
        }
    </style>
</head>

<body>
    <div class="bg-glow"></div>

    <div class="container">
        <div class="card">
            <div class="icon-container">
                <div class="icon-bg"></div>
                <svg class="checkmark-svg" viewBox="0 0 52 52">
                    <circle class="checkmark-circle" cx="26" cy="26" r="25" />
                    <path class="checkmark-check" d="M14.1 27.2l7.1 7.2 16.7-16.8" />
                </svg>
            </div>

            <h1>Cấu hình thành công!</h1>
            <h2>Configuration Successful!</h2>

            <div class="status-box">
                <div class="status-text">
                    <span>
                        <span class="bold-text">Thiết bị đang khởi động lại</span>
                        <span class="dots">
                            <span class="dot"></span><span class="dot"></span><span class="dot"></span>
                        </span>
                    </span>
                    <span>
                        <span class="bold-text">Device is restarting</span>
                        <span class="dots">
                            <span class="dot"></span><span class="dot"></span><span class="dot"></span>
                        </span>
                    </span>
                </div>
            </div>
        </div>
    </div>

    <footer>
        Contact: <b>CKC</b>
    </footer>

    <script>
        window.addEventListener('load', function () {
            setTimeout(function () {
                fetch('/exit', { method: 'POST' })
                .then(response => {
                    if (response.ok) console.log('Exit config mode request sent');
                })
                .catch(error => console.error('Error:', error));
            }, 1000);
        });
    </script>
</body>

</html>
)rawliteral";

// =====================================================
// SIGNAL LEVEL
// =====================================================
int CKC_WebUI::getSignalLevel(int rssi)
{
    if (rssi > -60)
        return 4;
    if (rssi > -70)
        return 3;
    if (rssi > -80)
        return 2;
    return 1;
}

// =====================================================
// BUILD OPTION HTML
// =====================================================
String CKC_WebUI::buildWiFiOption(const String &ssid, int rssi)
{
    int level = getSignalLevel(rssi);

    String color;

    switch(level)
    {
        case 4: color = "#4CAF50"; break;   // Xanh lá
        case 3: color = "#FFC107"; break;   // Vàng
        case 2: color = "#FF9800"; break;   // Cam
        default: color = "#F44336"; break;  // Đỏ
    }

    String html;

    html += "<div class='option' onclick=\"selectWifi('";
    html += ssid;
    html += "')\">";

    html += "<span>";
    html += ssid;
    html += "</span>";

    html += "<div class='signal'>";

    for (int i = 1; i <= 4; i++)
    {
        if (i <= level)
        {
            html += "<span class='bar active' style='background:";
            html += color;
            html += "'></span>";
        }
        else
        {
            html += "<span class='bar'></span>";
        }
    }

    html += "</div>";
    html += "</div>";

    return html;
}

#endif
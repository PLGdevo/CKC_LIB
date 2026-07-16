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
            /* CHẾ ĐỘ SÁNG (MẶC ĐỊNH) */
            --bg: #f8fafc;
            --card: #ffffff;
            --text: #0f172a;
            --sub: #64748b;
            --border: #e2e8f0;
            --pri: #0ea5e9;
            --hover: #0284c7;
            --btn-text: #ffffff;
            --btn-bg: rgba(14, 165, 233, 0.1);
            --input-bg: #f1f5f9;
            --shadow: 0 10px 30px rgba(0, 0, 0, 0.05), 0 1px 3px rgba(0, 0, 0, 0.02);
            --glow-1: rgba(14, 165, 233, 0.03);
            --glow-2: rgba(14, 165, 233, 0.01);
            --switch-glow: rgba(14, 165, 233, 0.3);
            --moon-glow: none;
            --text-shadow: none;
        }

        body.dark {
            /* CHẾ ĐỘ TỐI */
            --bg: #090d16;
            --card: #121824;
            --text: #f8fafc;
            --sub: #64748b;
            --border: #1e293b;
            --pri: #00d2ff;
            --hover: #00b9e6;
            --btn-text: #090d16;
            --btn-bg: rgba(0, 210, 255, 0.1);
            --input-bg: #161f30;
            --shadow:
              0 0 25px rgba(0, 210, 255, 0.25),
              inset 0 0 15px rgba(0, 210, 255, 0.08);
            --glow-1: rgba(0, 210, 255, 0.05);
            --glow-2: rgba(0, 210, 255, 0.02);
            --switch-glow: rgba(0, 210, 255, 0.8);
            --moon-glow: 0 0 8px var(--pri);
            --text-shadow: 0 0 10px rgba(0, 210, 255, 0.3);
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
        }

        body {
            background: var(--bg);
            background-image:
              radial-gradient(circle at top right, var(--glow-1), transparent 40%),
              radial-gradient(circle at bottom left, var(--glow-2), transparent 50%);
            padding: 20px;
            display: flex;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            transition: background 0.3s, color 0.3s;
        }

        .container {
            width: 100%;
            max-width: 400px;
        }

        .card {
            background: var(--card);
            padding: 32px;
            border-radius: 16px;
            box-shadow: var(--shadow);
            border: 1px solid var(--border);
            transition: background 0.3s, border-color 0.3s, box-shadow 0.3s;
            backdrop-filter: blur(10px);
            margin-bottom: 20px;
        }

        h2 {
            color: var(--text);
            font-size: 1.4rem;
            font-weight: 600;
            letter-spacing: 0.5px;
            text-shadow: var(--text-shadow);
        }

        /* ĐỒNG BỘ KHU VỰC SWITCH THEME */
        .theme-switch {
            display: inline-flex;
            align-items: center;
            justify-content: center;
            gap: 8px;
        }

        .switch {
            position: relative;
            width: 40px;
            height: 22px;
            display: inline-block;
        }

        .switch input {
            opacity: 0;
            width: 0;
            height: 0;
        }

        .slider {
            position: absolute;
            inset: 0;
            background: #cbd5e1;
            border-radius: 20px;
            cursor: pointer;
            transition: 0.3s;
            border: 1px solid var(--border);
        }

        body.dark .slider {
            background: #1e293b;
        }

        .slider:before {
            content: "";
            position: absolute;
            width: 14px;
            height: 14px;
            left: 3px;
            bottom: 3px;
            background: #fff;
            border-radius: 50%;
            transition: 0.3s;
            box-shadow: 0 0 8px var(--switch-glow);
        }

        input:checked + .slider {
            background: rgba(0, 210, 255, 0.2);
            border-color: var(--pri);
        }

        input:checked + .slider:before {
            transform: translateX(18px);
            background: var(--pri);
        }

        /* ẨN HIỆN ICON TRÁI SWITCH THEO THEME */
        .sun-icon, .moon-icon {
            font-size: 15px;
            line-height: 1;
            display: inline-block;
            user-select: none;
        }

        .sun-icon { display: block; }
        .moon-icon { display: none; }

        body.dark .sun-icon { display: none; }
        body.dark .moon-icon { 
            display: block; 
            text-shadow: var(--moon-glow);
        }

        label {
            display: block;
            color: var(--sub);
            font-size: 0.8rem;
            font-weight: 600;
            margin-top: 16px;
            margin-bottom: 8px;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        .input-control {
            width: 100%;
            height: 46px;
            border: 1px solid var(--border);
            border-radius: 10px;
            background: var(--input-bg);
            padding: 0 16px;
            color: var(--text);
            font-size: 0.95rem;
            outline: none;
            transition: all 0.2s;
        }

        .input-control:focus {
            border-color: var(--pri);
            box-shadow: 0 0 10px rgba(0, 210, 255, 0.3);
        }

        .password-box {
            position: relative;
            width: 100%;
        }

        .password-box input {
            padding-right: 48px;
        }

        .toggle-eye {
            position: absolute;
            right: 16px;
            top: 50%;
            transform: translateY(-50%);
            cursor: pointer;
            user-select: none;
            font-size: 16px;
            opacity: 0.6;
            color: var(--text);
            transition: opacity 0.2s;
        }

        .toggle-eye:hover {
            opacity: 1;
        }

        /* NÚT SUBMIT & BUTTON */
        button,
        input[type="submit"] {
            width: 100%;
            height: 48px;
            border: 1px solid var(--pri);
            border-radius: 10px;
            background: var(--btn-bg);
            color: var(--pri);
            font-weight: 600;
            font-size: 0.95rem;
            cursor: pointer;
            transition: all 0.2s, transform 0.1s;
            letter-spacing: 0.5px;
            box-shadow: 0 0 12px rgba(0, 210, 255, 0.1);
            margin-top: 20px;
        }

        button:hover,
        input[type="submit"]:hover {
            background: var(--pri);
            color: var(--btn-text);
            box-shadow: 0 0 20px rgba(0, 210, 255, 0.4);
        }

        button:active,
        input[type="submit"]:active {
            transform: scale(0.98);
        }

        .btn-reload {
            margin-top: 0;
            height: 44px;
        }

        hr {
            border: none;
            border-top: 1px solid var(--border);
            margin: 20px 0;
        }

        /* DROPDOWN DANH SÁCH WIFI QUÉT ĐƯỢC */
        .dropdown {
            margin-top: 16px;
            max-height: 220px;
            overflow-y: auto;
            border: 1px solid var(--border);
            border-radius: 10px;
            background: var(--input-bg);
        }

        .dropdown .title {
            padding: 14px 16px;
            font-size: 0.9rem;
            font-weight: 600;
            color: var(--sub);
            border-bottom: 1px solid var(--border);
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        .option {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 14px 16px;
            cursor: pointer;
            border-bottom: 1px solid var(--border);
            transition: background 0.2s;
            color: var(--text);
            font-size: 0.95rem;
        }

        .option:last-child {
            border-bottom: none;
        }

        .option:hover {
            background: var(--border);
        }

        /* BIỂU TƯỢNG VẠCH SÓNG */
        .signal {
            display: flex;
            gap: 2.5px;
            align-items: flex-end;
        }

        .bar {
            width: 4px;
            background: #cbd5e1;
            border-radius: 2px;
            transition: background 0.2s;
        }

        body.dark .bar {
            background: #1e293b;
        }

        .bar:nth-child(1) { height: 6px; }
        .bar:nth-child(2) { height: 9px; }
        .bar:nth-child(3) { height: 12px; }
        .bar:nth-child(4) { height: 15px; }

        .bar.active {
            background: #10b981 !important; /* Đổi thành màu xanh lá hiện đại cho vạch sóng active */
        }
    </style>
</head>

<body>

    <div class="container">

        <!-- CARD THIẾT LẬP THIẾT BỊ -->
        <div class="card">

            <!-- HEADER CARD: TIÊU ĐỀ BÊN TRÁI, SWITCH BÊN PHẢI -->
            <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px;">
                <h2>SET UP DEVICE</h2>
                
                <div class="theme-switch">
                    <span class="sun-icon">☀️</span>
                    <span class="moon-icon">🌙</span>
                    <label class="switch">
                        <input id="themeSwitch" type="checkbox" />
                        <span class="slider"></span>
                    </label>
                </div>
            </div>

            <form action="/connect" method="POST">

                <label>SSID</label>
                <div class="select-wrap">
                    <input type="text" id="wifiSelect" name="ssid" class="input-control" placeholder="Select WiFi" />                
                </div>

                <label>WiFi Password</label>
                <div class="password-box">
                    <input type="password" id="wifiPass" name="pass" class="input-control" placeholder="Enter WiFi password">
                    <span class="toggle-eye" onclick="togglePassword('wifiPass', this)">👁</span>
                </div>

                <label>MQTT Username</label>
                <input type="text" id="mqttUser" name="mqtt_user" class="input-control" placeholder="Enter MQTT username">

                <label>MQTT Password</label>
                <div class="password-box">
                    <input type="password" id="mqttPass" name="mqtt_pass" class="input-control" placeholder="Enter MQTT password">
                    <span class="toggle-eye" onclick="togglePassword('mqttPass', this)">👁</span>
                </div>

                <input type="submit" value="CONNECT" />

            </form>

        </div>

        <hr> 
        
        <!-- CARD SCAN WIFI -->
        <div class="card">

            <form action="/scan" method="GET">
                <button type="submit" class="btn-reload">RELOAD WIFI</button>
            </form>

            <div class="dropdown" id="wifiList">
                <div class="title">Scan Wi-Fi</div>
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
        
        function selectWifi(name) {
            document.getElementById("wifiSelect").value = name;
        }        

        window.onload = function () {
            if (select && select.options && select.options.length > 0) {
                let level = select.options[select.selectedIndex].getAttribute("data-level");
                renderSignal(level);
            }
        };

        function togglePassword(inputId, eye) {
            const input = document.getElementById(inputId);
            if (input.type === "password") {
                input.type = "text";
                eye.innerHTML = "👁"; // Có thể đổi thành biểu tượng khác nếu muốn
            } else {
                input.type = "password";
                eye.innerHTML = "👁";
            }
        }

        // --- XỬ LÝ SWITCH THEME (MẶC ĐỊNH LÀ SÁNG) ---
        const sw = document.getElementById("themeSwitch");

        function setTheme(isDark) {
            document.body.classList.toggle("dark", isDark);
            sw.checked = isDark;
            localStorage.setItem("theme", isDark ? "dark" : "light");
        }

        // Kiểm tra cấu hình cũ đã lưu, mặc định ban đầu là light (false) nếu chưa cài đặt
        if (localStorage.getItem("theme") === "dark") {
            setTheme(true);
        } else {
            setTheme(false);
        }

        sw.onchange = () => setTheme(sw.checked);
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
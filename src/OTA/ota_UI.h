
#include <Arduino.h>

const char WebOTA[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="vi">

<head>
    <meta charset="UTF-8">
    <title>CKC Control Panel</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;600;700;800&display=swap" rel="stylesheet">

    <style>
        :root {
            /* CHẾ ĐỘ SÁNG */
            --bg: #f8fafc;
            --card: #ffffff;
            --text: #0f172a;
            --sub: #64748b;
            --border: #e2e8f0;
            --prog-bg: #e2e8f0;
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

            /* BIẾN CHO MÀN HÌNH THÀNH CÔNG */
            --accent-color: #32d6a0;
            --accent-glow: rgba(50, 255, 255, 0.3);
        }

        body.dark {
            /* CHẾ ĐỘ TỐI */
            --bg: #090d16;
            --card: #121824;
            --text: #f8fafc;
            --sub: #64748b;
            --border: #1e293b;
            --prog-bg: #1e293b;
            --pri: #00d2ff;
            --hover: #00b9e6;
            --btn-text: #090d16;
            --btn-bg: rgba(0, 210, 255, 0.1);
            --input-bg: #161f30;
            --shadow: 0 0 25px rgba(0, 210, 255, 0.25), inset 0 0 15px rgba(0, 210, 255, 0.08);
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
            font-family: 'Inter', -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
        }

        body {
            background: var(--bg);
            background-image:
              radial-gradient(circle at top right, var(--glow-1), transparent 40%),
              radial-gradient(circle at bottom left, var(--glow-2), transparent 50%),
              radial-gradient(circle at 10% 20%, rgba(210, 255, 50, 0.02) 0%, transparent 40%);
            padding: 20px;
            display: flex;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            transition: background 0.3s, color 0.3s;
        }

        .container {
            width: 100%;
            max-width: 420px;
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

        .card-success-mode {
            padding: 60px 40px;
            text-align: center;
            border-radius: 32px;
        }

        h2 {
            color: var(--text);
            font-size: 1.4rem;
            font-weight: 600;
            letter-spacing: 0.5px;
            text-shadow: var(--text-shadow);
        }

        .tabs {
            display: flex;
            gap: 10px;
            margin-bottom: 20px;
            border-bottom: 1px solid var(--border);
            padding-bottom: 10px;
        }

        .tab-btn {
            flex: 1;
            padding: 10px;
            background: none;
            border: none;
            color: var(--sub);
            font-weight: 600;
            font-size: 0.9rem;
            cursor: pointer;
            transition: all 0.2s;
            border-radius: 6px;
            margin-top: 0;
            height: auto;
            box-shadow: none;
        }

        .tab-btn.active {
            color: var(--pri);
            background: var(--btn-bg);
        }

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

        .switch input { opacity: 0; width: 0; height: 0; }

        .slider {
            position: absolute;
            inset: 0;
            background: #cbd5e1;
            border-radius: 20px;
            cursor: pointer;
            transition: 0.3s;
            border: 1px solid var(--border);
        }

        body.dark .slider { background: #1e293b; }

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

        .sun-icon, .moon-icon {
            font-size: 15px;
            line-height: 1;
            display: inline-block;
            user-select: none;
        }

        .sun-icon { display: block; }
        .moon-icon { display: none; }

        body.dark .sun-icon { display: none; }
        body.dark .moon-icon { display: block; text-shadow: var(--moon-glow); }

        label {
            display: block;
            color: var(--sub);
            font-size: 0.75rem;
            font-weight: 600;
            margin-top: 14px;
            margin-bottom: 6px;
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

        .password-box { position: relative; width: 100%; }
        .password-box input { padding-right: 48px; }

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
        }

        button, input[type="submit"] {
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

        button:hover, input[type="submit"]:hover {
            background: var(--pri);
            color: var(--btn-text);
            box-shadow: 0 0 20px rgba(0, 210, 255, 0.4);
        }

        button:active, input[type="submit"]:active { transform: scale(0.98); }

        .btn-reload { margin-top: 0; height: 44px; }
        .btn-secondary { border: 1px solid #64748b; background: rgba(100, 116, 139, 0.1); color: #cbd5e1; box-shadow: none; margin-top: 12px; }
        .btn-secondary:hover { background: #64748b; color: #090d16; }

        hr { border: none; border-top: 1px solid var(--border); margin: 20px 0; }

        /* DROPDOWN WIFI */
        .dropdown { margin-top: 16px; max-height: 180px; overflow-y: auto; border: 1px solid var(--border); border-radius: 10px; background: var(--input-bg); }
        .dropdown .title { padding: 12px 16px; font-size: 0.85rem; font-weight: 600; color: var(--sub); border-bottom: 1px solid var(--border); text-transform: uppercase; }
        .option { display: flex; justify-content: space-between; align-items: center; padding: 12px 16px; cursor: pointer; border-bottom: 1px solid var(--border); color: var(--text); font-size: 0.95rem; }
        .option:last-child { border-bottom: none; }
        .option:hover { background: var(--border); }

        /* SIGNAL BARS */
        .signal { display: flex; gap: 2.5px; align-items: flex-end; }
        .bar { width: 4px; background: #cbd5e1; border-radius: 2px; }
        body.dark .bar { background: #1e293b; }
        .bar:nth-child(1) { height: 6px; }
        .bar:nth-child(2) { height: 9px; }
        .bar:nth-child(3) { height: 12px; }
        .bar:nth-child(4) { height: 15px; }
        .bar.active { background: #10b981 !important; }

        /* FILE UPLOAD & OTA */
        .file-box-wrapper { display: flex; height: 46px; border: 1px solid var(--border); border-radius: 10px; overflow: hidden; background: var(--input-bg); }
        .fileBox { flex-grow: 1; display: flex; align-items: center; padding: 0 16px; color: var(--sub); font-size: 0.9rem; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
        #choose { background: var(--border); color: var(--text); border: none; height: 100%; padding: 0 20px; font-weight: 600; font-size: 0.85rem; cursor: pointer; margin-top: 0; width: auto; }
        input[type="file"] { display: none; }

        .progress-container { margin-top: 20px; border-top: 1px solid var(--border); padding-top: 16px; }
        .status { text-align: center; margin-bottom: 10px; color: var(--pri); font-weight: 600; font-size: 0.95rem; display: flex; align-items: center; justify-content: center; gap: 8px; }
        .status::before { content: ""; width: 6px; height: 6px; background-color: var(--pri); border-radius: 50%; display: inline-block; animation: status-pulse 1.6s infinite ease-in-out; }
        @keyframes status-pulse { 0%, 100% { transform: scale(0.8); opacity: 0.4; } 50% { transform: scale(1.4); opacity: 1; } }
        .progress { height: 6px; background: var(--prog-bg); border-radius: 10px; overflow: hidden; }
        .progress-bar { height: 100%; width: 0; background: var(--pri); box-shadow: 0 0 10px var(--pri); transition: width 0.3s ease; }

        /* INFO SYSTEM */
        .info { margin-top: 20px; display: flex; flex-direction: column; gap: 10px; }
        .info div { display: flex; justify-content: space-between; font-size: 0.85rem; padding-bottom: 6px; border-bottom: 1px solid var(--border); }
        .info span:first-child { color: var(--sub); }
        .info span:last-child { font-weight: 500; color: var(--text); }

        .error-msg { color: #f87171; font-size: 0.85rem; text-align: center; margin-top: 10px; display: none; }
        .footer { text-align: center; margin-top: 14px; font-size: 0.8rem; letter-spacing: 1px; text-transform: uppercase; }
        .footer a { color: var(--sub); text-decoration: none; font-weight: 600; }
        .footer a:hover { color: var(--pri); }
        .hidden { display: none !important; }

        /* ================= STYLE CHUYÊN BIỆT CHO HIỆU ỨNG THÀNH CÔNG ================= */
        .icon-container { margin-bottom: 32px; display: inline-flex; justify-content: center; align-items: center; position: relative; }
        .icon-bg { position: absolute; width: 110px; height: 110px; background: var(--accent-glow); border-radius: 50%; filter: blur(25px); animation: pulse Glow 2.5s infinite; }
        @keyframes pulseGlow { 0%, 100% { transform: scale(1); opacity: 0.4; } 50% { transform: scale(1.15); opacity: 0.7; } }
        .checkmark-svg { width: 90px; height: 90px; position: relative; z-index: 2; }
        .checkmark-circle { fill: none; stroke: var(--accent-color); stroke-width: 3; stroke-dasharray: 166; stroke-dashoffset: 166; stroke-linecap: round; animation: drawCircle 0.8s cubic-bezier(0.65, 0, 0.45, 1) 0.2s forwards; }
        .checkmark-check { fill: none; stroke: var(--text-primary); stroke-width: 4; stroke-dasharray: 48; stroke-dashoffset: 48; stroke-linecap: round; stroke-linejoin: round; animation: drawCheck 0.5s cubic-bezier(0.65, 0, 0.45, 1) 0.9s forwards; }
        body.dark .checkmark-check { stroke: #ffffff; }
        @keyframes drawCircle { to { stroke-dashoffset: 0; } }
        @keyframes drawCheck { to { stroke-dashoffset: 0; } }
        .success-title { font-size: 26px; font-weight: 800; margin-bottom: 12px; letter-spacing: -0.03em; color: var(--text); }
        .success-subtitle { font-size: 16px; font-weight: 400; color: var(--sub); margin-bottom: 32px; }
        .status-box { background: rgba(255, 255, 255, 0.03); border: 1px solid var(--border); border-radius: 20px; padding: 20px; }
        .status-text { color: var(--sub); font-size: 14px; line-height: 1.8; display: flex; flex-direction: column; gap: 6px; text-align: center; }
        .bold-text { color: var(--text); font-weight: 600; }
        .dots { display: inline-flex; gap: 4px; margin-left: 6px; }
        .dot { width: 4px; height: 4px; background: var(--accent-color); border-radius: 50%; animation: bounceDot 1.4s infinite ease-in-out both; }
        .dot:nth-child(1) { animation-delay: -0.32s; }
        .dot:nth-child(2) { animation-delay: -0.16s; }
        @keyframes bounceDot { 0%, 80%, 100% { transform: scale(0); } 40% { transform: scale(1); } }
    </style>
</head>

<body>
    <div class="container">
        
        <!-- ================= MÀN HÌNH ĐĂNG NHẬP ================= -->
        <div class="card" id="loginCard">
            <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 24px;">
                <h2>CKC LOGIN</h2>
                <div class="theme-switch">
                    <span class="sun-icon">☀️</span><span class="moon-icon">🌙</span>
                    <label class="switch">
                        <input class="theme-checkbox" type="checkbox" onchange="setTheme(this.checked)"/>
                        <span class="slider"></span>
                    </label>
                </div>
            </div>

            <label>Username</label>
            <input type="text" id="username" class="input-control" placeholder="Enter username" autocomplete="off" />

            <label>Password</label>
            <input type="password" id="password" class="input-control" placeholder="Enter password" />
            
            <div class="error-msg" id="errorMsg">Incorrect username or password!</div>
            <button id="loginBtn" onclick="handleLogin()">Sign In</button>
        </div>

        <!-- ================= MÀN HÌNH ĐIỀU KHIỂN CHÍNH (SAU KHI LOGGED IN) ================= -->
        <div class="card hidden" id="mainCard">
            <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 16px;">
                <h2 id="panelTitle">WIFI CONFIG</h2>
                <div style="display: flex; align-items: center; gap: 14px;">
                    <div class="theme-switch">
                        <span class="sun-icon">☀️</span><span class="moon-icon">🌙</span>
                        <label class="switch">
                            <input class="theme-checkbox" type="checkbox" onchange="setTheme(this.checked)"/>
                            <span class="slider"></span>
                        </label>
                    </div>
                    <button onclick="handleLogout()" style="background:none; border:none; color:#f87171; font-size:0.85rem; cursor:pointer; font-weight:600; margin:0; width:auto; height:auto; box-shadow:none;">Logout</button>
                </div>
            </div>

            <!-- THANH CHUYỂN TAB -->
            <div class="tabs">
                <button class="tab-btn active" id="tabWifi" onclick="switchTab('wifi')">WiFi Setup</button>
                <button class="tab-btn" id="tabOta" onclick="switchTab('ota')">Firmware OTA</button>
            </div>

            <!-- TAB 1: CẤU HÌNH WIFI -->
            <div id="contentWifi">
                <!-- Chuyển form submit truyền thống thành JavaScript Onsubmit ngầm -->
                <form id="wifiForm" onsubmit="handleWifiSubmit(event)">
                    <label>SSID</label>
                    <input type="text" id="wifiSelect" name="ssid" class="input-control" placeholder="Select WiFi" required />                

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

                    <button type="submit" id="connectBtn">CONNECT DEVICE</button>
                </form>
                <hr>
                <button type="button" class="btn-reload" id="scanBtn" onclick="scanWifi()">RELOAD WIFI</button>
                <div class="dropdown">
                    <div class="title" id="scanStatus">Scan Wi-Fi</div>
                    <div id="networksContainer"></div>
                </div>
            </div>

            <!-- TAB 2: CẬP NHẬT OTA -->
            <div id="contentOta" class="hidden">
                <div id="uploadFormArea">
                    <label>Firmware File (.bin)</label>
                    <div class="file-box-wrapper">
                        <div class="fileBox" id="filename">No file selected</div>
                        <button id="choose" type="button" onclick="document.getElementById('file').click()">Browse</button>
                    </div>
                    <input id="file" type="file" accept=".bin" onchange="handleFileChange()" />
                    <button id="uploadBtn" onclick="handleUpload()">Update Device</button>
                </div>

                <div id="successArea" class="hidden">
                    <button class="btn btn-secondary" onclick="resetUploadState()">← Back to Upload</button>
                </div>

                <div class="progress-container">
                    <div class="status" id="status">Waiting...</div>
                    <div class="progress"><div class="progress-bar" id="bar"></div></div>
                </div>

                <!-- THÔNG SỐ ĐỘNG TỪ ESP32 -->
                <div class="info">
                    <div><span>MAC Address</span><span id="infoMac">Loading...</span></div>
                    <div><span>Current Version</span><span>v1.3.8</span></div>
                    <div><span>Chip Model</span><span id="infoChip">Loading...</span></div>
                    <div><span>Flash Size</span><span id="infoFlash">Loading...</span></div>
                    <div><span>RAM (Free/Total)</span><span id="infoRam">Loading...</span></div>
                    <div><span>Free OTA Space</span><span id="infoOta">Loading...</span></div>
                </div>
            </div>
        </div>

        <!-- ================= MÀN HÌNH THÔNG BÁO KẾT NỐI THÀNH CÔNG (TÍCH HỢP) ================= -->
        <div class="card card-success-mode hidden" id="successCard">
            <div class="icon-container">
                <div class="icon-bg"></div>
                <svg class="checkmark-svg" viewBox="0 0 52 52">
                    <circle class="checkmark-circle" cx="26" cy="26" r="25" />
                    <path class="checkmark-check" d="M14.1 27.2l7.1 7.2 16.7-16.8" />
                </svg>
            </div>

            <h1 class="success-title">Cấu hình thành công!</h1>
            <h2 class="success-subtitle">Configuration Successful!</h2>

            <div class="status-box">
                <div class="status-text">
                    <span>
                        <span class="bold-text">Thiết bị đang khởi động lại</span>
                        <span class="dots"><span class="dot"></span><span class="dot"></span><span class="dot"></span></span>
                    </span>
                    <span>
                        <span class="bold-text">Device is restarting</span>
                        <span class="dots"><span class="dot"></span><span class="dot"></span><span class="dot"></span></span>
                    </span>
                </div>
            </div>
        </div>

        <div class="footer">
            <a href="https://ait.caothang.edu.vn" target="_blank">CKC AIoT Platform</a>
        </div>
    </div>

    <script>
        const CONFIG = { mqttUser: "%MQTT_USER%", mqttPass: "%MQTT_PASS%" };
        if(!CONFIG.mqttUser.startsWith("%")) document.getElementById("mqttUser").value = CONFIG.mqttUser;
        if(!CONFIG.mqttPass.startsWith("%")) document.getElementById("mqttPass").value = CONFIG.mqttPass;

        let selectedFirmware = null;

        if (sessionStorage.getItem("authenticated") === "true") {
            showMainInterface();
        }

        function handleLogin() {
            const u = document.getElementById("username").value.trim();
            const p = document.getElementById("password").value;
            if (u === "admin" && p === "123456") {
                sessionStorage.setItem("authenticated", "true");
                showMainInterface();
            } else {
                document.getElementById("errorMsg").style.display = "block";
            }
        }

        function handleLogout() {
            sessionStorage.removeItem("authenticated");
            document.getElementById("mainCard").classList.add("hidden");
            document.getElementById("successCard").classList.add("hidden");
            document.getElementById("loginCard").classList.remove("hidden");
            document.getElementById("password").value = "";
            document.getElementById("errorMsg").style.display = "none";
        }

        function showMainInterface() {
            document.getElementById("loginCard").classList.add("hidden");
            document.getElementById("successCard").classList.add("hidden");
            document.getElementById("mainCard").classList.remove("hidden");
            switchTab('wifi');
        }

        // --- XỬ LÝ GỬI THÔNG TIN WIFI BẰNG AJAX KHÔNG RELOAD TRANG ---
        function handleWifiSubmit(event) {
            event.preventDefault(); // Chặn việc tải lại trang mặc định của form
            
            const btn = document.getElementById("connectBtn");
            btn.disabled = true;
            btn.innerText = "SAVING CONFIG...";

            const formData = new FormData(document.getElementById("wifiForm"));
            
            // Chuyển FormData sang dạng URL-encoded phù hợp với hàm xử lý ESP32 WebServer
            const urlEncoded = new URLSearchParams(formData).toString();

            fetch('/connect', {
                method: 'POST',
                headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
                body: urlEncoded
            })
            .then(res => {
                if (res.ok) {
                    // Ẩn bảng điều khiển chính, hiển thị màn hình thành công động với vòng tròn checkmark vẽ mượt mà
                    document.getElementById("mainCard").classList.add("hidden");
                    document.getElementById("successCard").classList.remove("hidden");
                } else {
                    alert("Gửi cấu hình thất bại / Configuration saving failed");
                    btn.disabled = false;
                    btn.innerText = "CONNECT DEVICE";
                }
            })
            .catch(err => {
                console.error("Error connecting device:", err);
                // Với ESP32, đôi khi việc ngắt kết nối AP để nhảy sang STA xảy ra quá nhanh khiến Fetch bị chặn, 
                // nhưng bản chất lệnh đã gửi đi thành công nên ta vẫn kích hoạt màn hình reboot
                document.getElementById("mainCard").classList.add("hidden");
                document.getElementById("successCard").classList.remove("hidden");
            });
        }

        function switchTab(tab) {
            const isWifi = tab === 'wifi';
            document.getElementById("panelTitle").innerText = isWifi ? "WIFI CONFIG" : "SYSTEM UPDATE";
            document.getElementById("tabWifi").classList.toggle("active", isWifi);
            document.getElementById("tabOta").classList.toggle("active", !isWifi);
            document.getElementById("contentWifi").classList.toggle("hidden", !isWifi);
            document.getElementById("contentOta").classList.toggle("hidden", isWifi);
            
            if(!isWifi) fetchSystemInfo();
            else scanWifi();
        }

        function togglePassword(id, eye) {
            const input = document.getElementById(id);
            if (input.type === "password") { input.type = "text"; eye.innerHTML = "🙈"; } 
            else { input.type = "password"; eye.innerHTML = "👁"; }
        }

        function selectWifi(name) {
            document.getElementById("wifiSelect").value = name;
            document.getElementById("wifiSelect").scrollIntoView({ behavior: 'smooth' });
        }

        function scanWifi() {
            const btn = document.getElementById("scanBtn");
            const statusText = document.getElementById("scanStatus");
            const container = document.getElementById("networksContainer");
            btn.disabled = true; btn.innerHTML = "SCANNING..."; statusText.innerHTML = "Scanning...";
            
            fetch('/scan').then(res => res.json()).then(data => {
                container.innerHTML = "";
                if(data.length === 0) statusText.innerHTML = "No networks found";
                else {
                    statusText.innerHTML = `Found ${data.length} Networks`;
                    data.forEach(net => {
                        const opt = document.createElement("div"); opt.className = "option";
                        opt.setAttribute("onclick", `selectWifi('${net.ssid}')`);
                        let bars = 1;
                        if (net.rssi > -60) bars = 4; else if (net.rssi > -70) bars = 3; else if (net.rssi > -80) bars = 2;
                        opt.innerHTML = `<span>${net.ssid}</span><div class="signal">` + 
                            `<div class="bar ${bars>=1?'active':''}"></div><div class="bar ${bars>=2?'active':''}"></div>` +
                            `<div class="bar ${bars>=3?'active':''}"></div><div class="bar ${bars>=4?'active':''}"></div></div>`;
                        container.appendChild(opt);
                    });
                }
            }).catch(() => { statusText.innerHTML = "Scan failed."; })
            .finally(() => { btn.disabled = false; btn.innerHTML = "RELOAD WIFI"; });
        }

        function fetchSystemInfo() {
            fetch("/info").then(res => res.json()).then(data => {
                document.getElementById("infoMac").textContent = data.mac;
                document.getElementById("infoChip").textContent = data.chip;
                document.getElementById("infoFlash").textContent = data.flash;
                document.getElementById("infoRam").textContent = data.ram;
                document.getElementById("infoOta").textContent = data.free_ota;
            }).catch(() => {
                ['infoMac','infoChip','infoFlash','infoRam','infoOta'].forEach(id => document.getElementById(id).textContent = "Error");
            });
        }

        function handleFileChange() {
            const fileInput = document.getElementById("file");
            selectedFirmware = fileInput.files[0];
            if (selectedFirmware) {
                document.getElementById("filename").textContent = `${selectedFirmware.name} (${(selectedFirmware.size / 1024).toFixed(1)} KB)`;
                document.getElementById("status").textContent = "Ready to upload";
                document.getElementById("status").style.color = "var(--pri)";
            }
        }

        function handleUpload() {
            if (!selectedFirmware) { alert("Please choose a firmware file."); return; }
            let form = new FormData(); form.append("update", selectedFirmware);
            let xhr = new XMLHttpRequest(); xhr.open("POST", "/update", true);

            const uploadBtn = document.getElementById("uploadBtn");
            const bar = document.getElementById("bar");
            const st = document.getElementById("status");

            uploadBtn.disabled = true; uploadBtn.textContent = "Uploading...";

            xhr.upload.onprogress = (e) => {
                if (e.lengthComputable) {
                    let p = Math.round((e.loaded / e.total) * 100);
                    bar.style.width = p + "%"; st.textContent = p + " %";
                }
            };
            xhr.onload = () => {
                uploadBtn.disabled = false; uploadBtn.textContent = "Update Device";
                if (xhr.status == 200) {
                    st.textContent = "Update Successful! Rebooting..."; st.style.color = "#4ade80"; bar.style.width = "100%";
                    document.getElementById("uploadFormArea").classList.add("hidden");
                    document.getElementById("successArea").classList.remove("hidden");
                } else {
                    st.textContent = "Update Failed"; st.style.color = "#f87171";
                }
            };
            xhr.onerror = () => { uploadBtn.disabled = false; uploadBtn.textContent = "Update Device"; st.textContent = "Connection Error"; st.style.color = "#f87171"; };
            xhr.send(form);
        }

        function resetUploadState() {
            selectedFirmware = null; document.getElementById("file").value = "";
            document.getElementById("filename").textContent = "No file selected";
            document.getElementById("status").textContent = "Waiting...";
            document.getElementById("status").style.color = "var(--pri)";
            document.getElementById("bar").style.width = "0%";
            document.getElementById("uploadFormArea").classList.remove("hidden");
            document.getElementById("successArea").classList.add("hidden");
        }

        function setTheme(isDark) {
            document.body.classList.toggle("dark", isDark);
            document.querySelectorAll(".theme-checkbox").forEach(cb => cb.checked = isDark);
            localStorage.setItem("theme", isDark ? "dark" : "light");
        }
        setTheme(localStorage.getItem("theme") === "dark");
    </script>
</body>
</html>

)rawliteral";
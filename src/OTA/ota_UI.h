
#include <Arduino.h>

const char WebOTA[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width,initial-scale=1.0" />
    <title>CKC OTA - Login</title>
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
        font-family:
          -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica,
          Arial, sans-serif;
      }

      body {
        background: var(--bg);
        background-image:
          radial-gradient(
            circle at top right,
            var(--glow-1),
            transparent 40%
          ),
          radial-gradient(
            circle at bottom left,
            var(--glow-2),
            transparent 50%
          );
        padding: 20px;
        display: flex;
        align-items: center;
        justify-content: center;
        min-height: 100vh;
        transition:
          background 0.3s,
          color 0.3s;
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
        transition:
          background 0.3s,
          border-color 0.3s,
          box-shadow 0.3s;
        backdrop-filter: blur(10px);
      }

      h2 {
        color: var(--text);
        font-size: 1.5rem;
        font-weight: 600;
        letter-spacing: 0.5px;
        text-shadow: var(--text-shadow);
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

      .field-group {
        margin-bottom: 16px;
      }

      label.field-label {
        display: block;
        color: var(--sub);
        font-size: 0.8rem;
        font-weight: 600;
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

      .file-box-wrapper {
        display: flex;
        height: 46px;
        border: 1px solid var(--border);
        border-radius: 10px;
        overflow: hidden;
        background: var(--input-bg);
        margin-bottom: 16px;
        transition: all 0.2s;
      }

      .file-box-wrapper:focus-within {
        border-color: var(--pri);
        box-shadow: 0 0 10px rgba(0, 210, 255, 0.3);
      }

      .fileBox {
        flex-grow: 1;
        display: flex;
        align-items: center;
        padding: 0 16px;
        color: var(--sub);
        font-size: 0.9rem;
        white-space: nowrap;
        overflow: hidden;
        text-overflow: ellipsis;
      }

      #choose {
        background: var(--border);
        color: var(--text);
        border: none;
        border-left: 1px solid var(--border);
        height: 100%;
        padding: 0 20px;
        font-weight: 600;
        font-size: 0.85rem;
        cursor: pointer;
        transition: all 0.2s;
      }

      #choose:hover {
        background: rgba(0, 210, 255, 0.15);
        color: var(--pri);
      }

      input[type="file"] {
        display: none;
      }

      .btn {
        width: 100%;
        height: 48px;
        border: 1px solid var(--pri);
        border-radius: 10px;
        background: var(--btn-bg);
        color: var(--pri);
        font-weight: 600;
        font-size: 0.95rem;
        cursor: pointer;
        transition:
          all 0.2s,
          transform 0.1s;
        letter-spacing: 0.5px;
        box-shadow: 0 0 12px rgba(0, 210, 255, 0.1);
        
        display: flex;
        align-items: center;
        justify-content: center;
        gap: 10px;
      }

      .btn:hover {
        background: var(--pri);
        color: var(--btn-text);
        box-shadow: 0 0 20px rgba(0, 210, 255, 0.4);
      }

      .btn:active {
        transform: scale(0.98);
      }

      .btn-secondary {
        border: 1px solid #64748b;
        background: rgba(100, 116, 139, 0.1);
        color: #cbd5e1;
        box-shadow: none;
        margin-top: 12px;
      }
      .btn-secondary:hover {
        background: #64748b;
        color: #090d16;
        box-shadow: 0 0 15px rgba(100, 116, 139, 0.4);
      }

      .error-msg {
        color: #f87171;
        font-size: 0.85rem;
        text-align: center;
        margin-top: -8px;
        margin-bottom: 12px;
        display: none;
        text-shadow: 0 0 8px rgba(248, 113, 113, 0.3);
      }

      .progress-container {
        margin-top: 24px;
        border-top: 1px solid var(--border);
        padding-top: 20px;
      }

      .status {
        text-align: center;
        margin-bottom: 12px;
        color: var(--pri);
        font-weight: 600;
        font-size: 0.95rem;
        display: flex;
        align-items: center;
        justify-content: center;
        gap: 8px;
        text-shadow: var(--moon-glow);
      }

      .status::before {
        content: "";
        width: 6px;
        height: 6px;
        background-color: var(--pri);
        border-radius: 50%;
        display: inline-block;
        box-shadow: 0 0 8px var(--pri);
        animation: status-pulse 1.6s infinite ease-in-out;
      }

      .status.status-error::before {
        background-color: #f87171;
        box-shadow: 0 0 8px #f87171;
      }

      @keyframes status-pulse {
        0%,
        100% {
          transform: scale(0.8);
          opacity: 0.4;
        }
        50% {
          transform: scale(1.4);
          opacity: 1;
        }
      }

      .progress {
        height: 6px;
        background: var(--prog-bg);
        border-radius: 10px;
        overflow: hidden;
        border: 1px solid rgba(0, 210, 255, 0.05);
      }

      .bar {
        height: 100%;
        width: 0;
        background: var(--pri);
        box-shadow: 0 0 10px var(--pri);
        transition: width 0.3s ease;
      }

      .info {
        margin-top: 20px;
        display: flex;
        flex-direction: column;
        gap: 12px;
      }

      .info div {
        display: flex;
        justify-content: space-between;
        font-size: 0.9rem;
        padding-bottom: 6px;
        border-bottom: 1px solid var(--border);
      }

      .info span:first-child {
        color: var(--sub);
      }

      .info span:last-child {
        font-weight: 500;
        color: var(--text);
      }

      /* CSS CHO FOOTER & LINK TRUY CẬP */
      .footer {
        text-align: center;
        margin-top: 24px;
        font-size: 0.8rem;
        letter-spacing: 1px;
        text-transform: uppercase;
      }

      .footer a {
        color: var(--sub);
        text-decoration: none;
        font-weight: 600;
        transition: all 0.2s ease;
      }

      .footer a:hover {
        color: var(--pri);
        text-shadow: var(--moon-glow);
      }

      .hidden {
        display: none !important;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <!-- 1. MÀN HÌNH ĐĂNG NHẬP (LOGIN CARD) -->
      <div class="card" id="loginCard">
        <div
          style="
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 24px;
          "
        >
          <h2 style="margin-bottom: 0; text-align: left">CKC Login</h2>

          <div class="theme-switch">
            <span class="sun-icon">☀️</span>
            <span class="moon-icon">🌙</span>
            <label class="switch">
              <input id="themeSwitchLogin" type="checkbox" checked />
              <span class="slider"></span>
            </label>
          </div>
        </div>

        <div class="field-group">
          <label class="field-label">Username</label>
          <input
            type="text"
            id="username"
            class="input-control"
            placeholder="Enter username"
            autocomplete="off"
          />
        </div>

        <div class="field-group">
          <label class="field-label">Password</label>
          <input
            type="password"
            id="password"
            class="input-control"
            placeholder="Enter password"
          />
        </div>

        <div class="error-msg" id="errorMsg">
          Incorrect username or password!
        </div>

        <button class="btn" id="loginBtn">Sign In</button>
      </div>

      <!-- 2. MÀN HÌNH GIAO DIỆN OTA -->
      <div class="card hidden" id="otaCard">
        <div
          style="
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 20px;
          "
        >
          <h2 style="margin-bottom: 0; text-align: left">CKC Update</h2>
          
          <div style="display: flex; align-items: center; gap: 14px;">
            <div class="theme-switch">
              <span class="sun-icon">☀️</span>
              <span class="moon-icon">🌙</span>
              <label class="switch">
                <input id="themeSwitch" type="checkbox" checked />
                <span class="slider"></span>
              </label>
            </div>

            <button
              id="logoutBtn"
              style="
                background: none;
                border: none;
                color: #f87171;
                font-size: 0.85rem;
                cursor: pointer;
                font-weight: 600;
              "
            >
              Logout
            </button>
          </div>
        </div>

        <div id="uploadFormArea">
          <label class="field-label">Firmware File</label>
          <div class="file-box-wrapper">
            <div class="fileBox" id="filename">No file selected</div>
            <button id="choose" type="button">Browse</button>
          </div>

          <input id="file" type="file" accept=".bin" />
          <button id="upload" class="btn">Update Device</button>
        </div>

        <div id="successArea" class="hidden">
          <button id="backBtn" class="btn btn-secondary">
            ← Back to Upload
          </button>
        </div>

        <div class="progress-container">
          <div class="status" id="status">Waiting...</div>
          <div class="progress">
            <div class="bar" id="bar"></div>
          </div>
        </div>

        <!-- CÁC THÔNG SỐ SẼ ĐƯỢC LOAD DYNAMIC TỪ SERVER -->
        <div class="info">
          <div><span>MAC Address</span><span id="infoMac">Loading...</span></div>
          <div><span>Current Version</span><span>v1.3.8</span></div>
          <div><span>Chip</span><span id="infoChip">Loading...</span></div>
          <div><span>Flash</span><span id="infoFlash">Loading...</span></div>
          <div><span>RAM (Free/Total)</span><span id="infoRam">Loading...</span></div>
          <div><span>Free OTA Space</span><span id="infoOta">Loading...</span></div>
        </div>
      </div>

      <!-- FOOTER ĐÃ ĐƯỢC THÊM ĐƯỜNG LINK TRUY CẬP TRỰC TIẾP -->
      <div class="footer">
        <a href="https://ait.caothang.edu.vn" target="_blank">CKC AIoT Platform</a>
      </div>
    </div>

    <script>
      const loginCard = document.getElementById("loginCard"),
        otaCard = document.getElementById("otaCard"),
        usernameInp = document.getElementById("username"),
        passwordInp = document.getElementById("password"),
        loginBtn = document.getElementById("loginBtn"),
        logoutBtn = document.getElementById("logoutBtn"),
        errorMsg = document.getElementById("errorMsg");

      const file = document.getElementById("file"),
        fn = document.getElementById("filename"),
        st = document.getElementById("status"),
        bar = document.getElementById("bar"),
        choose = document.getElementById("choose"),
        upload = document.getElementById("upload");

      const uploadFormArea = document.getElementById("uploadFormArea"),
        successArea = document.getElementById("successArea"),
        backBtn = document.getElementById("backBtn");

      let firmware = null;

      // Quản lý Theme
      const sw = document.getElementById("themeSwitch");
      const swLogin = document.getElementById("themeSwitchLogin");

      function setTheme(isDark) {
        document.body.classList.toggle("dark", isDark);
        sw.checked = isDark;
        swLogin.checked = isDark;
        localStorage.setItem("theme", isDark ? "dark" : "light");
      }

      if (localStorage.getItem("theme") === "light") {
        setTheme(false);
      } else {
        setTheme(true);
      }

      sw.onchange = () => setTheme(sw.checked);
      swLogin.onchange = () => setTheme(swLogin.checked);

      // --- HÀM LẤY DỮ LIỆU CHIP ĐỘNG TỪ ESP32 ---
      function fetchSystemInfo() {
  fetch("/info")
    .then(response => {
      if (!response.ok) throw new Error("Network error");
      return response.json();
    })
    .then(data => {
      document.getElementById("infoMac").textContent = data.mac;
      document.getElementById("infoChip").textContent = data.chip; 
      document.getElementById("infoFlash").textContent = data.flash;
      document.getElementById("infoRam").textContent = data.ram;
      document.getElementById("infoOta").textContent = data.free_ota;
    })
    .catch(err => {
      console.error("Failed to fetch system info", err);
      document.getElementById("infoMac").textContent = "Error";
      document.getElementById("infoChip").textContent = "Error";
      document.getElementById("infoFlash").textContent = "Error";
      document.getElementById("infoRam").textContent = "Error";
      document.getElementById("infoOta").textContent = "Error";
    });
}

      // --- AUTH LOGIC ---
      if (sessionStorage.getItem("authenticated") === "true") {
        showOtaInterface();
      }

      function showOtaInterface() {
        loginCard.classList.add("hidden");
        otaCard.classList.remove("hidden");
        resetUploadState();
        fetchSystemInfo(); // Tự động load thông số ngay khi vào giao diện OTA
      }

      function showLoginInterface() {
        otaCard.classList.add("hidden");
        loginCard.classList.remove("hidden");
        passwordInp.value = "";
        errorMsg.style.display = "none";
      }

      loginBtn.onclick = () => {
        const user = usernameInp.value.trim();
        const pass = passwordInp.value;

        if (user === "admin" && pass === "123456") {
          sessionStorage.setItem("authenticated", "true");
          showOtaInterface();
        } else {
          errorMsg.style.display = "block";
        }
      };

      passwordInp.onkeydown = (e) => {
        if (e.key === "Enter") loginBtn.click();
      };

      logoutBtn.onclick = () => {
        sessionStorage.removeItem("authenticated");
        showLoginInterface();
      };

      // --- LOGIC UPLOAD FIRMWARE ---
      choose.onclick = () => file.click();

      file.onchange = () => {
        firmware = file.files[0];
        if (firmware) {
          fn.textContent =
            firmware.name + " (" + (firmware.size / 1024).toFixed(1) + " KB)";
          st.textContent = "Ready to upload";
          st.style.color = "var(--pri)";
          st.classList.remove("status-error");
        }
      };

      upload.onclick = () => {
        if (!firmware) {
          alert("Please choose firmware.");
          return;
        }
        let form = new FormData();
        form.append("update", firmware);
        let xhr = new XMLHttpRequest();
        xhr.open("POST", "/update", true);

        upload.disabled = true;
        upload.textContent = "Uploading to device...";

        xhr.upload.onprogress = (e) => {
          if (e.lengthComputable) {
            let p = (e.loaded / e.total) * 100;
            let percentVal = Math.round(p);
            bar.style.width = p + "%";
            st.textContent = percentVal + " %";
          }
        };

        xhr.onload = () => {
          upload.disabled = false;
          upload.textContent = "Update Device";

          if (xhr.status == 200) {
            st.textContent = "Update Successful! Rebooting...";
            st.style.color = "#4ade80";
            bar.style.width = "100%";

            uploadFormArea.classList.add("hidden");
            successArea.classList.remove("hidden");
          } else {
            st.textContent = "Update Failed";
            st.style.color = "#f87171";
            st.classList.add("status-error");
          }
        };

        xhr.onerror = () => {
          upload.disabled = false;
          upload.textContent = "Update Device";
          st.textContent = "Connection Error";
          st.style.color = "#f87171";
          st.classList.add("status-error");
        };

        xhr.send(form);
      };

      function resetUploadState() {
        firmware = null;
        file.value = "";
        fn.textContent = "No file selected";
        st.textContent = "Waiting...";
        st.style.color = "var(--pri)";
        st.classList.remove("status-error");
        bar.style.width = "0%";
        upload.textContent = "Update Device";

        uploadFormArea.classList.remove("hidden");
        successArea.classList.add("hidden");
      }

      backBtn.onclick = () => {
        resetUploadState();
      };
    </script>
  </body>
</html>

)rawliteral";
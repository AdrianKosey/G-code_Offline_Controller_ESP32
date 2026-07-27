#pragma once

const char WEB_PAGE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>CNC Offline Control</title>
<style>
  :root {
    --bg-base: #080D17;
    --bg-sidebar: #11192b;
    --bg-panel: #11192b;
    --bg-surface: #1a2338;
    --border: #2a3550;
    --text-main: #ffffff;
    --text-muted: #aaaaaa;

    --accent: #ecb86f;
    --accent-dim: rgba(236, 184, 111, 0.15);
    --sidebar-selected: #334059;

    --success: #00d97e;
    --warning: #f5c542;
    --danger: #e0473e;

    --axis-x: #ff6b6b;
    --axis-y: #6bffb0;
    --axis-z: #6ba8ff;

    --radius-card: 14px;
    --radius-btn: 10px;
  }

  * { box-sizing: border-box; margin: 0; padding: 0; }

  body {
    background: var(--bg-base);
    color: var(--text-main);
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    font-size: 14px;
    line-height: 1.5;
    display: flex;
    min-height: 100vh;
    position: relative;
    overflow-x: hidden;
  }

  .sidebar {
    width: 220px;
    background: var(--bg-sidebar);
    flex-shrink: 0;
    display: flex;
    flex-direction: column;
    padding: 20px 0;
    border-right: 1px solid var(--border);
    transition: width 0.3s ease, transform 0.3s ease;
    overflow: hidden;
  }

  body.sidebar-collapsed .sidebar {
    width: 0;
    padding-left: 0;
    padding-right: 0;
    border-right: none;
  }

  .sidebar-overlay {
    display: none;
  }

  @media (max-width: 768px) {
    .sidebar {
      position: fixed;
      top: 0;
      left: 0;
      height: 100vh;
      z-index: 1000;
      width: 220px !important;
      transform: translateX(-100%);
      border-right: 1px solid var(--border);
    }

    body.sidebar-collapsed .sidebar {
      transform: translateX(0);
      width: 220px !important;
      padding-left: initial;
      padding-right: initial;
      border-right: 1px solid var(--border);
    }

    body.sidebar-collapsed .sidebar-overlay {
      display: block;
      position: fixed;
      top: 0;
      left: 0;
      width: 100vw;
      height: 100vh;
      background: rgba(0, 0, 0, 0.6);
      z-index: 999;
      backdrop-filter: blur(2px);
    }
  }

  .btn-toggle-sidebar {
    background: var(--bg-surface);
    border: 1px solid var(--border);
    color: var(--text-main);
    padding: 6px 10px;
    border-radius: var(--radius-btn);
    cursor: pointer;
    display: inline-flex;
    align-items: center;
    justify-content: center;
    margin-right: 12px;
  }
  .btn-toggle-sidebar:hover {
    background: var(--sidebar-selected);
  }

  .sidebar-brand {
    font-size: 16px;
    font-weight: 800;
    color: var(--accent);
    padding: 0 20px 20px 20px;
    border-bottom: 1px solid var(--border);
    margin-bottom: 12px;
    display: flex;
    align-items: center;
    gap: 8px;
  }
  .sidebar-brand svg { width: 20px; height: 20px; }

  .nav-item {
    display: flex;
    align-items: center;
    gap: 12px;
    padding: 14px 20px;
    cursor: pointer;
    color: var(--text-muted);
    border-left: 3px solid transparent;
    transition: all 0.15s ease;
  }
  .nav-item svg { width: 18px; height: 18px; flex-shrink: 0; }
  .nav-item:hover { background: rgba(255,255,255,0.03); color: var(--text-main); }
  .nav-item.active {
    background: var(--sidebar-selected);
    color: var(--text-main);
    border-left-color: var(--accent);
  }

  .sidebar-status {
    margin-top: auto;
    padding: 16px 20px 0 20px;
    border-top: 1px solid var(--border);
    font-size: 12px;
    color: var(--text-muted);
  }
  .status-row { display: flex; justify-content: space-between; padding: 4px 0; }
  .status-dot { width: 8px; height: 8px; border-radius: 50%; display: inline-block; margin-right: 6px; }
  .dot-ok { background: var(--success); }
  .dot-bad { background: var(--danger); }

  .content { flex: 1; padding: 24px; overflow-y: auto; width: 100%; }

  .page { display: none; }
  .page.active { display: block; }

  .page-title {
    font-size: 20px;
    font-weight: 700;
    margin-bottom: 20px;
    display: flex;
    align-items: center;
    justify-content: space-between;
  }

  #stateBadge {
    background: var(--accent-dim);
    color: var(--accent);
    padding: 6px 14px;
    border-radius: 20px;
    font-weight: 600;
    font-size: 12px;
    border: 1px solid rgba(236,184,111,0.3);
    text-transform: uppercase;
    letter-spacing: 0.5px;
  }

  .grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 16px;
  }

  .card {
    background: var(--bg-panel);
    border-radius: var(--radius-card);
    padding: 18px;
    border: 1px solid rgba(255,255,255,0.03);
  }

  .card-header {
    font-size: 14px;
    font-weight: 600;
    color: var(--text-muted);
    margin-bottom: 14px;
    text-transform: uppercase;
    letter-spacing: 0.5px;
  }

  .dro-row {
    display: flex;
    justify-content: space-between;
    align-items: center;
    background: var(--bg-surface);
    padding: 12px 16px;
    border-radius: var(--radius-btn);
    margin-bottom: 8px;
  }
  .dro-label { font-size: 15px; font-weight: 800; }
  .dro-label.x { color: var(--axis-x); }
  .dro-label.y { color: var(--axis-y); }
  .dro-label.z { color: var(--axis-z); }
  .dro-value { font-family: "SFMono-Regular", Consolas, monospace; font-size: 20px; font-weight: 700; }

  .job-filename { font-size: 15px; font-weight: 600; margin-bottom: 4px; word-break: break-all; }
  .job-feed { font-size: 12px; color: var(--text-muted); margin-bottom: 14px; }

  .progress-track { background: var(--bg-surface); border-radius: 6px; height: 10px; overflow: hidden; margin-bottom: 8px; }
  .progress-fill { background: var(--accent); height: 100%; width: 0%; transition: width 0.4s ease; }
  .progress-stats { display: flex; justify-content: space-between; font-size: 12px; color: var(--text-muted); }
  #progressPercent { color: var(--accent); font-weight: 700; }

  button {
    font-family: inherit;
    font-weight: 600;
    font-size: 13px;
    padding: 12px;
    border: none;
    border-radius: var(--radius-btn);
    background: var(--bg-surface);
    color: var(--text-main);
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 6px;
    transition: all 0.15s ease;
  }
  button:hover { background: var(--sidebar-selected); }
  button:active { opacity: 0.8; }
  button svg { width: 16px; height: 16px; }

  button.primary { background: var(--accent-dim); color: var(--accent); border: 1px solid rgba(236,184,111,0.3); }
  button.primary:hover { background: rgba(236,184,111,0.25); }
  button.stop { background: var(--danger); color: #fff; }
  button.stop:hover { background: #c53730; }

  .control-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
  .control-grid .stop { grid-column: span 2; }

  .step-selector { display: flex; gap: 8px; margin-bottom: 16px; }
  .step-btn { flex: 1; }
  .step-btn.active { background: var(--accent-dim); color: var(--accent); border: 1px solid rgba(236,184,111,0.3); }

  .jog-grid { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 8px; margin-bottom: 14px; }
  .jog-z-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; margin-bottom: 14px; }
  .jog-zero-grid { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 8px; margin-bottom: 10px; }

  .btn-x {color: var(--axis-x); }
  .btn-y {color: var(--axis-y); }
  .btn-z {color: var(--axis-z); }

  .breadcrumb {
    background: var(--bg-surface);
    padding: 10px 14px;
    border-radius: var(--radius-btn);
    font-family: monospace;
    font-size: 12px;
    color: var(--accent);
    margin-bottom: 12px;
  }

  .file-list { background: var(--bg-surface); border-radius: var(--radius-btn); max-height: 320px; overflow-y: auto; }
  .file-list::-webkit-scrollbar { width: 6px; }
  .file-list::-webkit-scrollbar-thumb { background: var(--border); border-radius: 3px; }

  .file-row {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 12px 14px;
    border-bottom: 1px solid var(--border);
  }
  .file-row:last-child { border-bottom: none; }
  .file-row:hover { background: rgba(255,255,255,0.02); }

  .file-name { display: flex; align-items: center; gap: 10px; cursor: pointer; flex: 1; font-size: 13px; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
  .file-name svg { color: var(--text-muted); flex-shrink: 0; }

  .file-actions { display: flex; gap: 6px; }
  .file-actions button { padding: 6px; background: transparent; }
  .file-actions button:hover { background: var(--bg-panel); }
  .icon-load { color: var(--success); }
  .icon-del { color: var(--danger); }

  .source-tabs { display: flex; gap: 8px; margin-bottom: 12px; }
  .source-tab { flex: 1; }
  .source-tab.active { background: var(--accent-dim); color: var(--accent); border: 1px solid rgba(236,184,111,0.3); }

  .file-controls { display: flex; gap: 10px; margin-top: 12px; }
  .file-controls button { flex: 1; font-size: 12px; }

  .console-line { padding: 2px 0; white-space: pre-wrap; word-break: break-all; }
  .console-line.out { color: var(--accent); }
  .console-line.out::before { content: "> "; }
  .console-line.in { color: var(--text-muted); }
  .console-line.in::before { content: "< "; }

  .upload-area {
    margin-top: 12px;
    background: rgba(236,184,111,0.05);
    border: 1px dashed rgba(236,184,111,0.3);
    border-radius: var(--radius-btn);
    padding: 12px;
    display: flex;
    flex-direction: column;
    gap: 10px;
  }

  input[type=file] { font-size: 12px; color: var(--text-muted); }
  input[type=file]::file-selector-button {
    background: var(--bg-panel); color: var(--text-main);
    border: 1px solid var(--border); padding: 8px 12px;
    border-radius: 6px; cursor: pointer; font-weight: 600; margin-right: 12px;
  }
</style>
</head>
<body>

<div class="sidebar-overlay" onclick="toggleSidebar()"></div>

<div class="sidebar">
  <div class="sidebar-brand" style="justify-content: space-between;">
    <div style="display: flex; align-items: center; gap: 8px;">
      <svg viewBox="0 0 24 24" fill="currentColor"><path d="M19 3H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm-7 9h-2V7h-2v5H6v2h2v5h2v-5h2v-2z"/></svg>
      <span data-i18n="brandTitle">CNC CONTROL</span>
    </div>
  </div>

  <div class="nav-item active" data-page="dashboard" onclick="showPage('dashboard', this); if(window.innerWidth <= 768) toggleSidebar();">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M3 12l9-9 9 9M5 10v10h14V10"/></svg>
    <span data-i18n="navDashboard">Dashboard</span>
  </div>
  <div class="nav-item" data-page="jog" onclick="showPage('jog', this); if(window.innerWidth <= 768) toggleSidebar();">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9 3H5a2 2 0 0 0-2 2v4m6-6h10a2 2 0 0 1 2 2v4M3 15v4a2 2 0 0 0 2 2h4m10-6v4a2 2 0 0 1-2 2h-4"/></svg>
    <span data-i18n="navJog">Jog</span>
  </div>
  <div class="nav-item" data-page="files" onclick="showPage('files', this); if(window.innerWidth <= 768) toggleSidebar();">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M22 19a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h5l2 3h9a2 2 0 0 1 2 2z"/></svg>
    <span data-i18n="navFiles">Files</span>
  </div>

  <div class="nav-item" data-page="console" onclick="showPage('console', this)">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="4 17 10 11 4 5"/><line x1="12" y1="19" x2="20" y2="19"/></svg>
    <span data-i18n="navConsole">Console</span>
  </div>

  <div class="sidebar-status">
    <div class="status-row"><span>SD</span><span id="dotSd"><span class="status-dot dot-bad"></span>--</span></div>
    <div class="status-row"><span>USB</span><span id="dotUsb"><span class="status-dot dot-bad"></span>--</span></div>
    <div class="status-row"><span data-i18n="statusMachine">Machine</span><span id="dotMachine"><span class="status-dot dot-bad"></span>--</span></div>
  </div>
</div>

<div class="content">

  <!-- DASHBOARD -->
  <div class="page active" id="page-dashboard">
    <div class="page-title">
      <div style="display: flex; align-items: center; gap: 10px;">
        <button class="btn-toggle-sidebar" onclick="toggleSidebar()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
        </button>
        <span data-i18n="navDashboard">Dashboard</span>
      </div>
      <span id="stateBadge">--</span>
    </div>

    <div class="grid">
      <div class="card">
        <div class="card-header" data-i18n="cardPosition">Position (WPos)</div>
        <div class="dro-row"><span class="dro-label x">X</span><span class="dro-value" id="posX">0.000</span></div>
        <div class="dro-row"><span class="dro-label y">Y</span><span class="dro-value" id="posY">0.000</span></div>
        <div class="dro-row"><span class="dro-label z">Z</span><span class="dro-value" id="posZ">0.000</span></div>
      </div>

      <div class="card">
        <div class="card-header" data-i18n="cardCurrentJob">Current job</div>
        <div class="job-filename" id="filename" data-i18n="noFileLoaded">No file loaded</div>
        <div class="job-feed"><span data-i18n="labelFeed">Feed</span>: <b id="feed" style="color:#fff;">0</b> mm/min</div>
        <div class="progress-track"><div id="progressBar" class="progress-fill"></div></div>
        <div class="progress-stats">
          <span id="progressText">0 / 0</span>
          <span id="progressPercent">0%</span>
        </div>
      </div>

      <div class="card">
        <div class="card-header" data-i18n="cardControls">Controls</div>
        <div class="control-grid">
          <button class="primary" onclick="sendControl('playpause')">
            <svg viewBox="0 0 24 24" fill="currentColor"><path d="M8 5v14l11-7z"/></svg> <span data-i18n="btnPlayPause">Play/Pause</span>
          </button>
          <button onclick="sendControl('framing')">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M4 4h16v16H4z"/><path d="M4 12h16M12 4v16"/></svg> <span data-i18n="btnFraming">Framing</span>
          </button>
          <button class="stop" onclick="sendControl('stop')">
            <svg viewBox="0 0 24 24" fill="currentColor"><path d="M6 6h12v12H6z"/></svg> <span data-i18n="btnStop">STOP</span>
          </button>
        </div>
      </div>
    </div>
  </div>

  <!-- JOG -->
  <div class="page" id="page-jog">
    <div class="page-title">
      <div style="display: flex; align-items: center; gap: 10px;">
        <button class="btn-toggle-sidebar" onclick="toggleSidebar()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
        </button>
        <span data-i18n="navJog">Jog</span>
      </div>
      <span id="stateBadge">--</span>
    </div>

    <div class="card" style="max-width:400px;">
      <div class="card-header" data-i18n="cardStep">Step (mm)</div>
      <div class="step-selector">
        <button class="step-btn active" data-step="0.1" onclick="selectStep(0.1, this)">0.1</button>
        <button class="step-btn" data-step="1" onclick="selectStep(1, this)">1</button>
        <button class="step-btn" data-step="10" onclick="selectStep(10, this)">10</button>
      </div>

      <div class="jog-grid">
        <div></div>
        <button class="btn-y" onclick="jog('Y', 1)">Y+</button>
        <div></div>

        <button class="btn-x" onclick="jog('X', -1)">X-</button>
        <button onclick="doHome()" data-i18n="btnHome">HOME</button>
        <button class="btn-x" onclick="jog('X', 1)">X+</button>

        <div></div>
        <button class="btn-y" onclick="jog('Y', -1)">Y-</button>
        <div></div>
      </div>

      <div class="jog-z-grid">
        <button class="btn-z" onclick="jog('Z', 1)">Z+</button>
        <button class="btn-z" onclick="jog('Z', -1)">Z-</button>
      </div>

      <div class="card-header" style="margin-top:8px;" data-i18n="cardWorkZero">Work zero</div>
      <div class="jog-zero-grid">
        <button onclick="setZero('X')">SET X0</button>
        <button onclick="setZero('Y')">SET Y0</button>
        <button onclick="setZero('Z')">SET Z0</button>
      </div>

      <button class="primary" style="width:100%;" onclick="probeZ()" data-i18n="btnProbeZ">PROBE Z</button>
    </div>
  </div>

  <!-- FILES -->
  <div class="page" id="page-files">
    <div class="page-title">
      <div style="display: flex; align-items: center; gap: 10px;">
        <button class="btn-toggle-sidebar" onclick="toggleSidebar()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
        </button>
        <span data-i18n="navFiles">Files</span>
      </div>
      <span id="stateBadge">--</span>
    </div>

    <div class="card" style="max-width:480px;">
      <div class="source-tabs">
        <button class="source-tab active" id="tabSd" onclick="selectSource('sd')">SD</button>
        <button class="source-tab" id="tabUsb" onclick="selectSource('usb')">USB</button>
      </div>

      <div id="breadcrumb" class="breadcrumb">/</div>
      <div id="fileList" class="file-list"></div>

      <div class="file-controls">
        <button onclick="createFolder()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg> <span data-i18n="btnNewFolder">New folder</span>
        </button>
      </div>

      <form id="uploadForm" class="upload-area">
        <input type="file" id="fileInput" name="file" accept=".nc,.gcode,.ngc,.tap">
        <button type="submit" class="primary">
          <svg viewBox="0 0 24 24" fill="currentColor" style="width:16px;height:16px;"><path d="M9 16h6v-6h4l-7-7-7 7h4v6zm-4 2h14v2H5v-2z"/></svg> <span data-i18n="btnUploadFile">Upload file</span>
        </button>
      </form>
    </div>
  </div>

  <!-- CONSOLE -->
  <div class="page" id="page-console">
    <div class="page-title" data-i18n="navConsole">Console</div>

    <div class="card" style="max-width:600px;">
      <div id="consoleLog" style="background:#0a0e1a; border-radius:8px; padding:12px; height:320px; overflow-y:auto; font-family:monospace; font-size:12px; margin-bottom:12px;"></div>

      <div style="display:flex; gap:8px;">
        <input type="text" id="consoleInput" placeholder="" style="flex:1; background:var(--bg-surface); border:1px solid var(--border); color:var(--text-main); padding:10px; border-radius:8px; font-family:monospace;">
        <button class="primary" onclick="sendConsoleCommand()" data-i18n="btnSend">Send</button>
      </div>
    </div>
  </div>

</div>

<script>
const translations = {
  en: {
    brandTitle: "CNC CONTROL",
    navDashboard: "Dashboard",
    navJog: "Jog",
    navFiles: "Files",
    navConsole: "Console",
    statusMachine: "Machine",
    cardPosition: "Position (WPos)",
    cardCurrentJob: "Current job",
    noFileLoaded: "No file loaded",
    labelFeed: "Feed",
    cardControls: "Controls",
    btnPlayPause: "Play/Pause",
    btnFraming: "Framing",
    btnStop: "STOP",
    cardStep: "Step (mm)",
    btnHome: "HOME",
    cardWorkZero: "Work zero",
    btnProbeZ: "PROBE Z",
    btnNewFolder: "New folder",
    btnUploadFile: "Upload file",
    btnSend: "Send",
    backText: "Back",
    confirmHoming: "Execute full homing?",
    confirmSetZero: "Set {axis}0 to current position?",
    confirmProbeZ: "Execute Probe Z?",
    confirmDelete: "Delete {path}?",
    promptNewName: "New name:",
    promptFolderName: "Folder name:",
    confirmLoadFile: "Load {path} for execution?",
    uploadingText: "Uploading...",
    alertUploadError: "Upload error.",
    alertErrorPrefix: "Error: "
  }
};

let currentLang = 'en';

function t(key, replacements = {}) {
  let text = translations[currentLang][key] || key;
  for (const [k, v] of Object.entries(replacements)) {
    text = text.replace(`{${k}}`, v);
  }
  return text;
}

function updateTexts() {
  document.querySelectorAll('[data-i18n]').forEach(el => {
    const key = el.getAttribute('data-i18n');
    el.innerText = t(key);
  });
}

let currentPath = '/';
let currentSource = 'sd';
let jogStep = 0.1;

const iconFolder = '<svg viewBox="0 0 24 24" fill="currentColor" width="16" height="16"><path d="M10 4H4c-1.1 0-2 .9-2 2v12c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2h-8l-2-2z"/></svg>';
const iconFile = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/></svg>';
const iconUp = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><circle cx="12" cy="12" r="10"/><polyline points="15 14 12 11 9 14"/><path d="M12 11v8"/></svg>';
const iconLoad = '<svg viewBox="0 0 24 24" fill="currentColor" width="14" height="14"><path d="M8 5v14l11-7z"/></svg>';
const iconEdit = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="14" height="14"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/></svg>';
const iconTrash = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="14" height="14"><polyline points="3 6 5 6 21 6"/><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>';

function showPage(page, el) {
  document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
  document.getElementById('page-' + page).classList.add('active');
  document.querySelectorAll('.nav-item').forEach(n => n.classList.remove('active'));
  el.classList.add('active');
}

function setDot(id, ok, label) {
  const el = document.getElementById(id);
  el.innerHTML = '<span class="status-dot ' + (ok ? 'dot-ok' : 'dot-bad') + '"></span>' + label;
}

async function refreshStatus() {
  try {
    const res = await fetch('/api/status');
    const data = await res.json();

    document.getElementById('stateBadge').innerText = data.state;
    document.getElementById('filename').innerText = data.filename || t('noFileLoaded');
    document.getElementById('progressBar').style.width = data.percent + '%';
    document.getElementById('progressText').innerText = data.line + ' / ' + data.totalLines;
    document.getElementById('progressPercent').innerText = data.percent + '%';
    document.getElementById('posX').innerText = Number(data.x).toFixed(3);
    document.getElementById('posY').innerText = Number(data.y).toFixed(3);
    document.getElementById('posZ').innerText = Number(data.z).toFixed(3);
    document.getElementById('feed').innerText = data.feed;

    setDot('dotSd', data.sdOk, data.sdOk ? 'OK' : 'N/A');
    setDot('dotUsb', data.usbOk, data.usbOk ? 'OK' : 'N/A');
    setDot('dotMachine', data.machineOk, data.machineOk ? 'OK' : 'N/A');
  } catch (e) { console.error("Status error:", e); }
}

async function sendControl(action) {
  await fetch('/api/control', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'action=' + action
  });
}

function selectStep(step, btn) {
  jogStep = step;
  document.querySelectorAll('.step-btn').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
}

async function jog(axis, direction) {
  await fetch('/api/jog', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'axis=' + axis + '&distance=' + (jogStep * direction)
  });
}

async function doHome() {
  if (!confirm(t('confirmHoming'))) return;
  await fetch('/api/home', { method: 'POST' });
}

async function setZero(axis) {
  if (!confirm(t('confirmSetZero', {axis: axis}))) return;
  await fetch('/api/setzero', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'axis=' + axis
  });
}

async function probeZ() {
  if (!confirm(t('confirmProbeZ'))) return;
  await fetch('/api/probez', { method: 'POST' });
}

function selectSource(source) {
  currentSource = source;
  currentPath = '/';
  document.getElementById('tabSd').classList.toggle('active', source === 'sd');
  document.getElementById('tabUsb').classList.toggle('active', source === 'usb');
  refreshFiles();
}

function joinPath(base, name) { return base === '/' ? '/' + name : base + '/' + name; }
function parentPath(path) {
  if (path === '/' || path === '') return '/';
  const idx = path.lastIndexOf('/');
  return idx <= 0 ? '/' : path.substring(0, idx);
}

async function refreshFiles() {
  try {
    const res = await fetch('/api/files?path=' + encodeURIComponent(currentPath) + '&source=' + currentSource);
    const data = await res.json();

    document.getElementById('breadcrumb').innerText = currentSource.toUpperCase() + ': ' + data.path;
    const list = document.getElementById('fileList');
    list.innerHTML = '';

    if (currentPath !== '/') {
      const upRow = document.createElement('div');
      upRow.className = 'file-row';
      upRow.innerHTML = `<div class="file-name" style="color:var(--text-muted)">${iconUp} ${t('backText')}</div>`;
      upRow.onclick = () => { currentPath = parentPath(currentPath); refreshFiles(); };
      list.appendChild(upRow);
    }

    data.entries.forEach(entry => {
      const fullPath = entry.name.startsWith('/') ? entry.name : joinPath(currentPath, entry.name);
      const shortName = entry.name.startsWith('/') ? entry.name.split('/').pop() : entry.name;
      const row = document.createElement('div');
      row.className = 'file-row';

      const infoDiv = document.createElement('div');
      infoDiv.className = 'file-name';
      const actionsDiv = document.createElement('div');
      actionsDiv.className = 'file-actions';

      if (entry.isDir) {
        infoDiv.innerHTML = `${iconFolder} ${shortName}`;
        infoDiv.onclick = () => { currentPath = fullPath; refreshFiles(); };
        actionsDiv.innerHTML =
          `<button onclick="renamePrompt('${fullPath}')">${iconEdit}</button>` +
          `<button onclick="deleteEntry('${fullPath}')" class="icon-del">${iconTrash}</button>`;
      } else {
        infoDiv.innerHTML = `${iconFile} ${shortName}`;
        infoDiv.onclick = () => selectFile(fullPath);
        actionsDiv.innerHTML =
          `<button onclick="selectFile('${fullPath}')" class="icon-load">${iconLoad}</button>` +
          `<button onclick="renamePrompt('${fullPath}')">${iconEdit}</button>` +
          `<button onclick="deleteEntry('${fullPath}')" class="icon-del">${iconTrash}</button>`;
      }

      row.appendChild(infoDiv);
      row.appendChild(actionsDiv);
      list.appendChild(row);
    });
  } catch (e) { console.error("Files error:", e); }
}

async function deleteEntry(path) {
  if (!confirm(t('confirmDelete', {path: path}))) return;
  await fetch('/api/delete', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) + '&source=' + currentSource });
  refreshFiles();
}

async function renamePrompt(path) {
  const newName = prompt(t('promptNewName'));
  if (!newName) return;
  await fetch('/api/rename', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) + '&newName=' + encodeURIComponent(newName) + '&source=' + currentSource });
  refreshFiles();
}

async function createFolder() {
  const name = prompt(t('promptFolderName'));
  if (!name) return;
  await fetch('/api/mkdir', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(joinPath(currentPath, name)) + '&source=' + currentSource });
  refreshFiles();
}

async function selectFile(path) {
  if (!confirm(t('confirmLoadFile', {path: path}))) return;
  await fetch('/api/select', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) + '&source=' + currentSource });
}

document.getElementById('uploadForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  const fileInput = document.getElementById('fileInput');
  if (!fileInput.files.length) return;

  const btn = e.target.querySelector('button');
  const originalText = btn.innerHTML;
  btn.innerHTML = t('uploadingText'); btn.disabled = true;

  const formData = new FormData();
  formData.append('file', fileInput.files[0]);

  try {
    await fetch('/api/upload?path=' + encodeURIComponent(currentPath) + '&source=' + currentSource, { method: 'POST', body: formData });
    fileInput.value = '';
  } catch(err) { alert(t('alertUploadError')); }
  finally { btn.innerHTML = originalText; btn.disabled = false; refreshFiles(); }
});

function toggleSidebar() {
  document.body.classList.toggle('sidebar-collapsed');
}

let lastConsoleVersion = -1;

async function refreshConsole() {
  try {
    const res = await fetch('/api/console');
    const data = await res.json();

    if (data.version === lastConsoleVersion) return;
    lastConsoleVersion = data.version;

    const log = document.getElementById('consoleLog');
    const wasAtBottom = log.scrollTop + log.clientHeight >= log.scrollHeight - 10;

    log.innerHTML = data.entries.map(e =>
      `<div class="console-line ${e.out ? 'out' : 'in'}">${escapeHtml(e.text)}</div>`
    ).join('');

    if (wasAtBottom) log.scrollTop = log.scrollHeight;
  } catch (e) { console.error("Console error:", e); }
}

function escapeHtml(text) {
  const div = document.createElement('div');
  div.innerText = text;
  return div.innerHTML;
}

async function sendConsoleCommand() {
  const input = document.getElementById('consoleInput');
  const command = input.value.trim();
  if (!command) return;

  const res = await fetch('/api/console/send', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'command=' + encodeURIComponent(command)
  });

  if (!res.ok) {
    const msg = await res.text();
    alert(t('alertErrorPrefix') + msg);
  }

  input.value = '';
}

document.getElementById('consoleInput').addEventListener('keypress', (e) => {
  if (e.key === 'Enter') sendConsoleCommand();
});

updateTexts();
setInterval(refreshConsole, 500);
refreshConsole();

setInterval(refreshStatus, 1000);
setInterval(refreshFiles, 5000);
refreshStatus();
refreshFiles();
</script>
</body>
</html>
)rawliteral";
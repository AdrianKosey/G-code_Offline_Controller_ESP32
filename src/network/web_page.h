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
    --bg-base: #0a0e17;
    --bg-sidebar: #111726;
    --bg-panel: #151c2e;
    --bg-surface: #1e273d;
    --border: #283552;
    --border-focus: #3e517a;
    --text-main: #f0f4f8;
    --text-muted: #8c9ba5;

    --accent: #f3b050;
    --accent-dim: rgba(243, 176, 80, 0.12);
    --sidebar-selected: #222f47;

    --success: #00e676;
    --warning: #ffb300;
    --danger: #ff5252;

    --axis-x: #ff5252;
    --axis-y: #25e679;
    --axis-z: #40c4ff;

    --radius-card: 12px;
    --radius-btn: 8px;
    --shadow: 0 8px 24px rgba(0, 0, 0, 0.3);
  }

  * { box-sizing: border-box; margin: 0; padding: 0; }

  body {
    background: var(--bg-base);
    color: var(--text-main);
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    font-size: 14px;
    line-height: 1.5;
    display: flex;
    height: 100vh;
    overflow: hidden;
  }

  /* SIDEBAR */
  .sidebar {
    width: 240px;
    background: var(--bg-sidebar);
    flex-shrink: 0;
    display: flex;
    flex-direction: column;
    padding: 16px 0;
    border-right: 1px solid var(--border);
    transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
    z-index: 10;
  }

  body.sidebar-collapsed .sidebar {
    width: 0;
    padding: 16px 0;
    border-right: none;
    overflow: hidden;
  }

  .sidebar-brand {
    font-size: 16px;
    font-weight: 800;
    color: var(--accent);
    padding: 0 20px 16px 20px;
    border-bottom: 1px solid var(--border);
    margin-bottom: 12px;
    display: flex;
    align-items: center;
    gap: 10px;
    letter-spacing: 0.5px;
  }
  .sidebar-brand svg { width: 22px; height: 22px; }

  .nav-item {
    display: flex;
    align-items: center;
    gap: 12px;
    padding: 12px 20px;
    margin: 2px 10px;
    border-radius: var(--radius-btn);
    cursor: pointer;
    color: var(--text-muted);
    font-weight: 600;
    transition: all 0.2s ease;
  }
  .nav-item svg { width: 18px; height: 18px; flex-shrink: 0; }
  .nav-item:hover { background: rgba(255,255,255,0.05); color: var(--text-main); }
  .nav-item.active {
    background: var(--sidebar-selected);
    color: var(--accent);
    box-shadow: 0 2px 8px rgba(0,0,0,0.2);
  }

  .sidebar-status {
    margin-top: auto;
    padding: 16px 20px;
    border-top: 1px solid var(--border);
    font-size: 12px;
    background: rgba(0,0,0,0.15);
  }
  .status-row { display: flex; justify-content: space-between; padding: 4px 0; font-weight: 600; }
  .status-dot { width: 8px; height: 8px; border-radius: 50%; display: inline-block; margin-right: 6px; }
  .dot-ok { background: var(--success); box-shadow: 0 0 6px var(--success); }
  .dot-bad { background: var(--danger); box-shadow: 0 0 6px var(--danger); }

  /* CONTENT AREA */
  .content {
    flex: 1;
    padding: 20px;
    overflow-y: auto;
    width: 100%;
    display: flex;
    flex-direction: column;
  }

  .page { display: none; height: 100%; }
  .page.active { display: flex; flex-direction: column; gap: 16px; }

  .page-header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding-bottom: 12px;
    border-bottom: 1px solid var(--border);
  }

  .page-title {
    font-size: 18px;
    font-weight: 700;
    display: flex;
    align-items: center;
    gap: 12px;
  }

  #stateBadge {
    background: var(--accent-dim);
    color: var(--accent);
    padding: 6px 16px;
    border-radius: 20px;
    font-weight: 700;
    font-size: 12px;
    border: 1px solid rgba(243, 176, 80, 0.3);
    text-transform: uppercase;
    letter-spacing: 1px;
  }

  /* GRID SYSTEM */
  .grid-dashboard {
    display: grid;
    grid-template-columns: 320px 1fr 320px;
    gap: 16px;
    align-items: start;
  }

  .grid-two-col {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 16px;
    align-items: start;
  }

  @media (max-width: 1280px) {
    .grid-dashboard { grid-template-columns: 300px 1fr; }
    .grid-dashboard .card-quick-jog { grid-column: span 2; }
  }

  @media (max-width: 850px) {
    .grid-dashboard, .grid-two-col { grid-template-columns: 1fr; }
    .grid-dashboard .card-quick-jog { grid-column: span 1; }
  }

  .card {
    background: var(--bg-panel);
    border-radius: var(--radius-card);
    padding: 16px;
    border: 1px solid var(--border);
    box-shadow: var(--shadow);
  }

  .card-header {
    font-size: 12px;
    font-weight: 700;
    color: var(--text-muted);
    margin-bottom: 12px;
    text-transform: uppercase;
    letter-spacing: 0.8px;
    display: flex;
    justify-content: space-between;
    align-items: center;
  }

  /* DRO DISPLAY */
  .dro-row {
    display: flex;
    justify-content: space-between;
    align-items: center;
    background: var(--bg-surface);
    padding: 10px 16px;
    border-radius: var(--radius-btn);
    margin-bottom: 10px;
    border: 1px solid rgba(255,255,255,0.03);
  }
  .dro-label { font-size: 18px; font-weight: 900; }
  .dro-label.x { color: var(--axis-x); }
  .dro-label.y { color: var(--axis-y); }
  .dro-label.z { color: var(--axis-z); }
  .dro-value {
    font-family: "SFMono-Regular", Consolas, "Liberation Mono", Menlo, monospace;
    font-size: 24px;
    font-weight: 700;
    letter-spacing: 1px;
  }

  /* CANVAS PREVIEW */
  .canvas-container {
    position: relative;
    width: 100%;
    height: 320px;
    background: #060911;
    border: 1px solid var(--border);
    border-radius: var(--radius-btn);
    overflow: hidden;
    cursor: grab;
  }
  .canvas-container:active { cursor: grabbing; }
  #pathCanvas { width: 100%; height: 100%; display: block; }

  .canvas-legend {
    position: absolute;
    top: 10px;
    left: 10px;
    display: flex;
    gap: 12px;
    background: rgba(15, 23, 42, 0.85);
    backdrop-filter: blur(4px);
    padding: 6px 12px;
    border-radius: 6px;
    border: 1px solid var(--border);
    font-size: 11px;
    z-index: 2;
  }
  .legend-item { display: flex; align-items: center; gap: 6px; font-weight: 700; }
  .legend-line { width: 18px; height: 3px; border-radius: 2px; }

  .canvas-tools {
    position: absolute;
    bottom: 10px;
    right: 10px;
    display: flex;
    gap: 6px;
    z-index: 2;
  }

  .canvas-tools button {
    padding: 4px 8px;
    font-size: 11px;
    background: rgba(15, 23, 42, 0.85);
    backdrop-filter: blur(4px);
  }

  /* JOB CONTROL CARD */
  .job-filename { font-size: 15px; font-weight: 700; color: #fff; margin-bottom: 4px; word-break: break-all; }
  .job-feed { font-size: 12px; color: var(--text-muted); margin-bottom: 12px; }

  .progress-track {
    background: var(--bg-surface);
    border-radius: 8px;
    height: 16px;
    overflow: hidden;
    margin-bottom: 8px;
    border: 1px solid var(--border);
    position: relative;
  }
  .progress-fill {
    background: linear-gradient(90deg, #f3b050, #ffca28);
    height: 100%;
    width: 0%;
    transition: width 0.2s ease;
    box-shadow: 0 0 12px rgba(243, 176, 80, 0.5);
  }
  .progress-stats { display: flex; justify-content: space-between; font-size: 12px; color: var(--text-muted); font-weight: 600; margin-bottom: 14px; }
  #progressPercent { color: var(--accent); font-weight: 800; }

  /* BUTTONS & CONTROLS */
  button {
    font-family: inherit;
    font-weight: 600;
    font-size: 13px;
    padding: 10px 14px;
    border: 1px solid var(--border);
    border-radius: var(--radius-btn);
    background: var(--bg-surface);
    color: var(--text-main);
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    transition: all 0.15s ease;
  }
  button:hover { background: var(--border-focus); color: #fff; border-color: rgba(255,255,255,0.2); }
  button:active { transform: translateY(1px); }
  button svg { width: 16px; height: 16px; }

  button.primary { background: var(--accent-dim); color: var(--accent); border-color: rgba(243, 176, 80, 0.4); }
  button.primary:hover { background: rgba(243, 176, 80, 0.25); }
  button.stop { background: rgba(255, 82, 82, 0.15); color: var(--danger); border-color: rgba(255, 82, 82, 0.4); }
  button.stop:hover { background: var(--danger); color: #fff; }

  .job-control-buttons { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 10px; }

  /* JOG PADS */
  .step-selector { display: flex; gap: 8px; margin-bottom: 14px; }
  .step-btn { flex: 1; }
  .step-btn.active { background: var(--accent-dim); color: var(--accent); border-color: var(--accent); }

  .jog-matrix { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 8px; margin-bottom: 12px; }
  .jog-z-matrix { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; margin-bottom: 12px; }
  .jog-zero-matrix { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 6px; }

  .btn-x { color: var(--axis-x); font-weight: 800; }
  .btn-y { color: var(--axis-y); font-weight: 800; }
  .btn-z { color: var(--axis-z); font-weight: 800; }

  /* FILES PAGE */
  .file-layout { display: grid; grid-template-columns: 1fr 300px; gap: 16px; }
  @media (max-width: 900px) { .file-layout { grid-template-columns: 1fr; } }

  .breadcrumb {
    background: var(--bg-surface);
    padding: 10px 14px;
    border-radius: var(--radius-btn);
    font-family: monospace;
    font-size: 13px;
    color: var(--accent);
    margin-bottom: 12px;
    border: 1px solid var(--border);
  }

  .file-list { background: var(--bg-surface); border-radius: var(--radius-btn); height: 380px; overflow-y: auto; border: 1px solid var(--border); }
  .file-list::-webkit-scrollbar { width: 6px; }
  .file-list::-webkit-scrollbar-thumb { background: var(--border); border-radius: 3px; }

  .file-row {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 10px 14px;
    border-bottom: 1px solid var(--border);
  }
  .file-row:last-child { border-bottom: none; }
  .file-row:hover { background: rgba(255,255,255,0.03); }

  .file-name { display: flex; align-items: center; gap: 10px; cursor: pointer; flex: 1; font-size: 13px; font-weight: 500; white-space: nowrap; overflow: hidden; text-overflow: ellipsis; }
  .file-actions { display: flex; gap: 4px; }
  .file-actions button { padding: 6px; background: transparent; border: none; }
  .file-actions button:hover { background: var(--bg-panel); }

  .source-tabs { display: flex; gap: 8px; margin-bottom: 12px; }
  .source-tab { flex: 1; }
  .source-tab.active { background: var(--accent-dim); color: var(--accent); border-color: var(--accent); }

  .upload-area {
    background: rgba(243, 176, 80, 0.03);
    border: 1px dashed rgba(243, 176, 80, 0.3);
    border-radius: var(--radius-btn);
    padding: 16px;
    display: flex;
    flex-direction: column;
    gap: 12px;
  }

  input[type=file] { font-size: 12px; color: var(--text-muted); }
  input[type=file]::file-selector-button {
    background: var(--bg-surface); color: var(--text-main);
    border: 1px solid var(--border); padding: 8px 12px;
    border-radius: 6px; cursor: pointer; font-weight: 600; margin-right: 12px;
  }

  /* CONSOLE */
  .console-wrapper { display: flex; flex-direction: column; flex: 1; height: 100%; min-height: 250px; }
  .console-log {
    flex: 1;
    background: #060911;
    border: 1px solid var(--border);
    border-radius: var(--radius-btn);
    padding: 10px;
    font-family: "SFMono-Regular", Consolas, monospace;
    font-size: 12px;
    overflow-y: auto;
    margin-bottom: 10px;
    line-height: 1.5;
    height: 180px;
  }
  .console-line.out { color: var(--accent); }
  .console-line.out::before { content: "> "; opacity: 0.7; }
  .console-line.in { color: var(--text-muted); }
  .console-line.in::before { content: "< "; opacity: 0.7; }

  /* OVERLAY MOBILE */
  .sidebar-overlay { display: none; }
  @media (max-width: 768px) {
    .sidebar {
      position: fixed; top: 0; left: 0; height: 100vh;
      transform: translateX(-100%); width: 240px !important;
    }
    body.sidebar-collapsed .sidebar { transform: translateX(0); }
    body.sidebar-collapsed .sidebar-overlay {
      display: block; position: fixed; top: 0; left: 0; width: 100vw; height: 100vh;
      background: rgba(0, 0, 0, 0.7); z-index: 9; backdrop-filter: blur(2px);
    }
  }

  .btn-toggle-sidebar {
    background: var(--bg-surface);
    border: 1px solid var(--border);
    color: var(--text-main);
    padding: 6px 10px;
    border-radius: var(--radius-btn);
    cursor: pointer;
  }
</style>
</head>
<body>

<div class="sidebar-overlay" onclick="toggleSidebar()"></div>

<div class="sidebar">
  <div class="sidebar-brand">
    <svg viewBox="0 0 24 24" fill="currentColor"><path d="M19 3H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm-7 9h-2V7h-2v5H6v2h2v5h2v-5h2v-2z"/></svg>
    <span>CNC CONTROL</span>
  </div>

  <div class="nav-item active" data-page="dashboard" onclick="showPage('dashboard', this)">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M3 12l9-9 9 9M5 10v10h14V10"/></svg>
    <span>Dashboard</span>
  </div>
  <div class="nav-item" data-page="jog" onclick="showPage('jog', this)">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9 3H5a2 2 0 0 0-2 2v4m6-6h10a2 2 0 0 1 2 2v4M3 15v4a2 2 0 0 0 2 2h4m10-6v4a2 2 0 0 1-2 2h-4"/></svg>
    <span>Jog Control</span>
  </div>
  <div class="nav-item" data-page="files" onclick="showPage('files', this)">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M22 19a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h5l2 3h9a2 2 0 0 1 2 2z"/></svg>
    <span>Files</span>
  </div>
  <div class="nav-item" data-page="console" onclick="showPage('console', this)">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="4 17 10 11 4 5"/><line x1="12" y1="19" x2="20" y2="19"/></svg>
    <span>Console</span>
  </div>

  <!-- Redirection Button to Task Manager / System Monitor -->
  <a href="/system" class="nav-item" style="text-decoration: none;">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="4" y="4" width="16" height="16" rx="2"/><rect x="9" y="9" width="6" height="6"/><line x1="9" y1="1" x2="9" y2="4"/><line x1="15" y1="1" x2="15" y2="4"/><line x1="9" y1="20" x2="9" y2="23"/><line x1="15" y1="20" x2="15" y2="23"/><line x1="20" y1="9" x2="23" y2="9"/><line x1="20" y1="15" x2="23" y2="15"/><line x1="1" y1="9" x2="4" y2="9"/><line x1="1" y1="15" x2="4" y2="15"/></svg>
    <span>System Monitor</span>
  </a>

  <div class="sidebar-status">
    <div class="status-row"><span>SD Card</span><span id="dotSd"><span class="status-dot dot-bad"></span>--</span></div>
    <div class="status-row"><span>USB Drive</span><span id="dotUsb"><span class="status-dot dot-bad"></span>--</span></div>
    <div class="status-row"><span>Máquina</span><span id="dotMachine"><span class="status-dot dot-bad"></span>--</span></div>
  </div>
</div>

<div class="content">

  <!-- DASHBOARD -->
  <div class="page active" id="page-dashboard">
    <div class="page-header">
      <div class="page-title">
        <button class="btn-toggle-sidebar" onclick="toggleSidebar()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
        </button>
        <span>Dashboard</span>
      </div>
      <span id="stateBadge">--</span>
    </div>

    <div class="grid-dashboard">
      <!-- Left Column: DRO + Integrated Console -->
      <div style="display:flex; flex-direction:column; gap:16px;">
        <div class="card">
          <div class="card-header">Position (WPos)</div>
          <div class="dro-row"><span class="dro-label x">X</span><span class="dro-value" id="posX">0.000</span></div>
          <div class="dro-row"><span class="dro-label y">Y</span><span class="dro-value" id="posY">0.000</span></div>
          <div class="dro-row"><span class="dro-label z">Z</span><span class="dro-value" id="posZ">0.000</span></div>
        </div>

        <div class="card">
          <div class="card-header">Live Console</div>
          <div class="console-log" id="dashConsoleLog"></div>
          <div style="display:flex; gap:6px;">
            <input type="text" id="dashConsoleInput" placeholder="Command..." style="flex:1; background:var(--bg-surface); border:1px solid var(--border); color:var(--text-main); padding:8px; border-radius:var(--radius-btn); font-family:monospace; font-size:12px;">
            <button class="primary" style="padding:6px 12px;" onclick="sendConsoleCommandFromDash()">Send</button>
          </div>
        </div>
      </div>

      <!-- Center Column: 2D Toolpath Canvas & Job Execution Control -->
      <div style="display:flex; flex-direction:column; gap:16px;">
        <div class="card" style="display:flex; flex-direction:column; gap:12px;">
          <div class="card-header">
            <span>2D Preview</span>
            <span style="font-size:10px; color:var(--text-muted);">Drag: Move | Wheel: Zoom</span>
          </div>

          <div class="canvas-container" id="canvasContainer">
            <div class="canvas-legend">
              <div class="legend-item"><div class="legend-line" style="background:#ff9100;"></div><span style="color:#ff9100;">G-Code Path</span></div>
            </div>
            <canvas id="pathCanvas"></canvas>
            <div class="canvas-tools">
              <button onclick="resetCanvasView()">Center</button>
              <button onclick="clearCanvasPath()">Clear Path</button>
            </div>
          </div>
        </div>

        <!-- Job Progress Card -->
        <div class="card">
          <div class="card-header">Job Execution Control</div>
          
          <div class="job-filename" id="filename">Sin archivo cargado</div>
          <div class="job-feed">Feedrate: <b id="feed" style="color:#fff;">0</b> mm/min</div>

          <div class="progress-track"><div id="progressBar" class="progress-fill"></div></div>
          <div class="progress-stats">
            <span id="progressText">0 / 0 lines</span>
            <span id="progressPercent">0%</span>
          </div>

          <div class="job-control-buttons">
            <button class="primary" onclick="sendControl('playpause')">
              <svg viewBox="0 0 24 24" fill="currentColor"><path d="M8 5v14l11-7z"/></svg> <span>Start / Pause</span>
            </button>
            <button onclick="sendControl('framing')">
              <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M4 4h16v16H4z"/><path d="M4 12h16M12 4v16"/></svg> <span>Framing</span>
            </button>
            <button class="stop" onclick="sendControl('stop')">
              <svg viewBox="0 0 24 24" fill="currentColor"><path d="M6 6h12v12H6z"/></svg> <span>STOP</span>
            </button>
          </div>
        </div>
      </div>

      <!-- Right Column: Quick Jog + Work Zero Direct Access -->
      <div class="card card-quick-jog">
        <div class="card-header">Quick Jog</div>
        <div class="step-selector">
          <button class="step-btn active" data-step="0.1" onclick="selectStep(0.1, this)">0.1</button>
          <button class="step-btn" data-step="1" onclick="selectStep(1, this)">1</button>
          <button class="step-btn" data-step="10" onclick="selectStep(10, this)">10</button>
        </div>

        <div class="jog-matrix">
          <div></div>
          <button class="btn-y" onclick="jog('Y', 1)">Y+</button>
          <div></div>

          <button class="btn-x" onclick="jog('X', -1)">X-</button>
          <button onclick="doHome()">HOME</button>
          <button class="btn-x" onclick="jog('X', 1)">X+</button>

          <div></div>
          <button class="btn-y" onclick="jog('Y', -1)">Y-</button>
          <div></div>
        </div>

        <div class="jog-z-matrix">
          <button class="btn-z" onclick="jog('Z', 1)">Z+</button>
          <button class="btn-z" onclick="jog('Z', -1)">Z-</button>
        </div>

        <div class="card-header" style="margin-top:14px; margin-bottom:8px;">Set Work Zero (WCS)</div>
        <div class="jog-zero-matrix">
          <button class="btn-x" onclick="setZero('X')" style="padding:6px; font-size:12px;">SET X0</button>
          <button class="btn-y" onclick="setZero('Y')" style="padding:6px; font-size:12px;">SET Y0</button>
          <button class="btn-z" onclick="setZero('Z')" style="padding:6px; font-size:12px;">SET Z0</button>
        </div>
      </div>
    </div>
  </div>

  <!-- FULL JOG PAGE -->
  <div class="page" id="page-jog">
    <div class="page-header">
      <div class="page-title">
        <button class="btn-toggle-sidebar" onclick="toggleSidebar()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
        </button>
        <span>Jog Control</span>
      </div>
    </div>

    <div class="grid-two-col">
      <div class="card">
        <div class="card-header">Step Distance (mm)</div>
        <div class="step-selector">
          <button class="step-btn active" data-step="0.1" onclick="selectStep(0.1, this)">0.1 mm</button>
          <button class="step-btn" data-step="1" onclick="selectStep(1, this)">1.0 mm</button>
          <button class="step-btn" data-step="10" onclick="selectStep(10, this)">10.0 mm</button>
        </div>

        <div class="jog-matrix">
          <div></div>
          <button class="btn-y" onclick="jog('Y', 1)">Y+</button>
          <div></div>

          <button class="btn-x" onclick="jog('X', -1)">X-</button>
          <button onclick="doHome()">HOME</button>
          <button class="btn-x" onclick="jog('X', 1)">X+</button>

          <div></div>
          <button class="btn-y" onclick="jog('Y', -1)">Y-</button>
          <div></div>
        </div>

        <div class="jog-z-matrix">
          <button class="btn-z" onclick="jog('Z', 1)">Z+</button>
          <button class="btn-z" onclick="jog('Z', -1)">Z-</button>
        </div>
      </div>

      <div class="card">
        <div class="card-header">Work Zero Controls</div>
        <div class="jog-zero-matrix" style="grid-template-columns: 1fr 1fr; gap:8px;">
          <button class="btn-x" onclick="setZero('X')">SET X0</button>
          <button class="btn-y" onclick="setZero('Y')">SET Y0</button>
          <button class="btn-z" onclick="setZero('Z')">SET Z0</button>
          <button onclick="setZero('ALL')">SET ALL ZERO</button>
        </div>

        <button class="primary" style="width:100%; padding: 14px; margin-top: 14px;" onclick="probeZ()">Z-AXIS PROBE</button>
      </div>
    </div>
  </div>

  <!-- FILES PAGE -->
  <div class="page" id="page-files">
    <div class="page-header">
      <div class="page-title">
        <button class="btn-toggle-sidebar" onclick="toggleSidebar()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
        </button>
        <span>File Explorer</span>
      </div>
    </div>

    <div class="card file-layout">
      <div>
        <div class="source-tabs">
          <button class="source-tab active" id="tabSd" onclick="selectSource('sd')">SD Card</button>
          <button class="source-tab" id="tabUsb" onclick="selectSource('usb')">USB Memory</button>
        </div>

        <div id="breadcrumb" class="breadcrumb">/</div>
        <div id="fileList" class="file-list"></div>
      </div>

      <div style="display:flex; flex-direction:column; gap:16px;">
        <div>
          <div class="card-header">Actions</div>
          <button style="width:100%" onclick="createFolder()">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg> <span>Create Folder</span>
          </button>
        </div>

        <form id="uploadForm" class="upload-area">
          <div class="card-header" style="margin:0;">Upload G-Code File</div>
          <input type="file" id="fileInput" name="file" accept=".nc,.gcode,.ngc,.tap">
          <button type="submit" class="primary">
            <svg viewBox="0 0 24 24" fill="currentColor" style="width:16px;height:16px;"><path d="M9 16h6v-6h4l-7-7-7 7h4v6zm-4 2h14v2H5v-2z"/></svg> <span>Upload File</span>
          </button>
        </form>
      </div>
    </div>
  </div>

  <!-- CONSOLE PAGE -->
  <div class="page" id="page-console">
    <div class="page-header">
      <div class="page-title">
        <button class="btn-toggle-sidebar" onclick="toggleSidebar()">
          <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><line x1="3" y1="12" x2="21" y2="12"/><line x1="3" y1="6" x2="21" y2="6"/><line x1="3" y1="18" x2="21" y2="18"/></svg>
        </button>
        <span>G-Code Console</span>
      </div>
    </div>

    <div class="card console-wrapper">
      <div class="console-log" id="consoleLog" style="height: calc(100vh - 220px);"></div>

      <div style="display:flex; gap:10px;">
        <input type="text" id="consoleInput" placeholder="Command..." style="flex:1; background:var(--bg-surface); border:1px solid var(--border); color:var(--text-main); padding:12px; border-radius:var(--radius-btn); font-family:monospace;">
        <button class="primary" onclick="sendConsoleCommand()">Send Command</button>
      </div>
    </div>
  </div>

</div>

<script>
const iconFolder = `<svg viewBox="0 0 24 24" width="16" height="16" fill="currentColor"><path d="M10 4H4c-1.1 0-1.99.9-1.99 2L2 18c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2h-8l-2-2z"/></svg>`;
const iconFile   = `<svg viewBox="0 0 24 24" width="16" height="16" fill="currentColor"><path d="M14 2H6c-1.1 0-1.99.9-1.99 2L4 20c0 1.1.89 2 1.99 2H18c1.1 0 2-.9 2-2V8l-6-6zm2 16H8v-2h8v2zm0-4H8v-2h8v2zm-3-5V3.5L18.5 9H13z"/></svg>`;
const iconUp     = `<svg viewBox="0 0 24 24" width="16" height="16" fill="none" stroke="currentColor" stroke-width="2"><path d="M12 19V5M5 12l7-7 7 7"/></svg>`;
const iconEdit   = `<svg viewBox="0 0 24 24" width="14" height="14" fill="none" stroke="currentColor" stroke-width="2"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/></svg>`;
const iconTrash  = `<svg viewBox="0 0 24 24" width="14" height="14" fill="none" stroke="currentColor" stroke-width="2"><polyline points="3 6 5 6 21 6"/><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>`;
const iconLoad   = `<svg viewBox="0 0 24 24" width="14" height="14" fill="currentColor"><path d="M8 5v14l11-7z"/></svg>`;

let currentPath = '/';
let currentSource = 'sd';
let jogStep = 0.1;

// CANVAS TOOLPATH SYSTEM
const canvasContainer = document.getElementById('canvasContainer');
const canvas = document.getElementById('pathCanvas');
const ctx = canvas.getContext('2d');

let staticPath = null; // {minX, minY, maxX, maxY, points: [[x,y], ...]}
let currentPos = { x: 0, y: 0 };
let canvasScale = 2; // base px per mm
let zoomLevel = 1.0;
let panX = 0;
let panY = 0;
let isDragging = false;
let startX = 0;
let startY = 0;

async function loadPreview() {
  try {
    const res = await fetch('/api/preview');
    staticPath = await res.json();
    drawToolpath();
  } catch (e) { console.error("Preview error:", e); }
}

function resizeCanvas() {
  if (!canvas || !canvasContainer) return;
  canvas.width = canvasContainer.clientWidth;
  canvas.height = canvasContainer.clientHeight;
  drawToolpath();
}

function resetCanvasView() {
  zoomLevel = 1.0;
  panX = 0;
  panY = 0;
  drawToolpath();
}

function clearCanvasPath() {
  staticPath = null;
  drawToolpath();
}

// Mouse Pan and Zoom
canvasContainer.addEventListener('mousedown', (e) => {
  if (e.target.tagName === 'BUTTON') return;
  isDragging = true;
  startX = e.clientX - panX;
  startY = e.clientY - panY;
});

window.addEventListener('mousemove', (e) => {
  if (!isDragging) return;
  panX = e.clientX - startX;
  panY = e.clientY - startY;
  drawToolpath();
});

window.addEventListener('mouseup', () => { isDragging = false; });

canvasContainer.addEventListener('wheel', (e) => {
  e.preventDefault();
  const zoomFactor = e.deltaY < 0 ? 1.15 : 0.85;
  zoomLevel = Math.min(Math.max(0.2, zoomLevel * zoomFactor), 20.0);
  drawToolpath();
}, { passive: false });

function drawToolpath() {
  if (!ctx || !canvas.width) return;

  ctx.clearRect(0, 0, canvas.width, canvas.height);

  const centerX = (canvas.width / 2) + panX;
  const centerY = (canvas.height / 2) + panY;
  const effectiveScale = canvasScale * zoomLevel;

  // Grid
  ctx.strokeStyle = '#1e273d';
  ctx.lineWidth = 1;
  const gridSize = 20 * effectiveScale;
  if (gridSize > 5) {
    for (let x = centerX % gridSize; x < canvas.width; x += gridSize) {
      ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, canvas.height); ctx.stroke();
    }
    for (let y = centerY % gridSize; y < canvas.height; y += gridSize) {
      ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(canvas.width, y); ctx.stroke();
    }
  }

  // Ejes
  ctx.strokeStyle = '#283552';
  ctx.lineWidth = 2;
  ctx.beginPath(); ctx.moveTo(centerX, 0); ctx.lineTo(centerX, canvas.height); ctx.stroke();
  ctx.beginPath(); ctx.moveTo(0, centerY); ctx.lineTo(canvas.width, centerY); ctx.stroke();

  // Trazo estatico completo - ya viene filtrado/calculado por el ESP32, se dibuja de una sola vez
  if (staticPath && staticPath.points && staticPath.points.length > 1) {
    ctx.strokeStyle = '#ff9100';
    ctx.lineWidth = 2;
    ctx.beginPath();

    staticPath.points.forEach((p, i) => {
      const px = centerX + p[0] * effectiveScale;
      const py = centerY - p[1] * effectiveScale;
      if (i === 0) ctx.moveTo(px, py);
      else ctx.lineTo(px, py);
    });

    ctx.stroke();
  }

  // Punto movil de la herramienta - viene de /api/status, actualizado cada segundo
  const toolX = centerX + currentPos.x * effectiveScale;
  const toolY = centerY - currentPos.y * effectiveScale;

  ctx.fillStyle = '#00e676';
  ctx.beginPath();
  ctx.arc(toolX, toolY, 5, 0, Math.PI * 2);
  ctx.fill();

  ctx.strokeStyle = '#ffffff';
  ctx.lineWidth = 1.5;
  ctx.beginPath();
  ctx.arc(toolX, toolY, 8, 0, Math.PI * 2);
  ctx.stroke();
}

window.addEventListener('resize', resizeCanvas);
setTimeout(resizeCanvas, 100);

function showPage(page, el) {
  document.querySelectorAll('.page').forEach(p => p.classList.remove('active'));
  document.getElementById('page-' + page).classList.add('active');
  document.querySelectorAll('.nav-item').forEach(n => n.classList.remove('active'));
  if(el) el.classList.add('active');
  if(window.innerWidth <= 768) toggleSidebar();
  if(page === 'dashboard') setTimeout(resizeCanvas, 50);
}

function setDot(id, ok, label) {
  const el = document.getElementById(id);
  el.innerHTML = '<span class="status-dot ' + (ok ? 'dot-ok' : 'dot-bad') + '"></span>' + label;
}

async function refreshStatus() {
  try {
    const res = await fetch('/api/status');
    const data = await res.json();

    const state = data.state || 'Idle';
    document.getElementById('stateBadge').innerText = state;
    document.getElementById('filename').innerText = data.filename || 'No file loaded';
    document.getElementById('progressBar').style.width = data.percent + '%';
    document.getElementById('progressText').innerText = data.line + ' / ' + data.totalLines + ' líneas';
    document.getElementById('progressPercent').innerText = data.percent + '%';
    
    const x = Number(data.x);
    const y = Number(data.y);
    const z = Number(data.z);

    document.getElementById('posX').innerText = x.toFixed(3);
    document.getElementById('posY').innerText = y.toFixed(3);
    document.getElementById('posZ').innerText = z.toFixed(3);
    document.getElementById('feed').innerText = data.feed;

    currentPos = { x, y };
    drawToolpath();

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
  if (!confirm("¿Ejecutar búsqueda de cero máquina (Homing)?")) return;
  await fetch('/api/home', { method: 'POST' });
}

async function setZero(axis) {
  if (!confirm(`¿Fijar cero del eje ${axis} en la posición actual?`)) return;
  await fetch('/api/setzero', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'axis=' + axis
  });
}

async function probeZ() {
  if (!confirm("¿Ejecutar secuencia de sondeo Probe en Z?")) return;
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
      upRow.innerHTML = `<div class="file-name" style="color:var(--text-muted)">${iconUp} Volver</div>`;
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
          `<button onclick="deleteEntry('${fullPath}')">${iconTrash}</button>`;
      } else {
        infoDiv.innerHTML = `${iconFile} ${shortName}`;
        infoDiv.onclick = () => selectFile(fullPath);
        actionsDiv.innerHTML =
          `<button onclick="selectFile('${fullPath}')">${iconLoad}</button>` +
          `<button onclick="renamePrompt('${fullPath}')">${iconEdit}</button>` +
          `<button onclick="deleteEntry('${fullPath}')">${iconTrash}</button>`;
      }

      row.appendChild(infoDiv);
      row.appendChild(actionsDiv);
      list.appendChild(row);
    });
  } catch (e) { console.error("Files error:", e); }
}

async function deleteEntry(path) {
  if (!confirm(`¿Eliminar ${path}?`)) return;
  await fetch('/api/delete', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) + '&source=' + currentSource });
  refreshFiles();
}

async function renamePrompt(path) {
  const newName = prompt("Nuevo nombre:");
  if (!newName) return;
  await fetch('/api/rename', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) + '&newName=' + encodeURIComponent(newName) + '&source=' + currentSource });
  refreshFiles();
}

async function createFolder() {
  const name = prompt("Nombre de la carpeta:");
  if (!name) return;
  await fetch('/api/mkdir', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(joinPath(currentPath, name)) + '&source=' + currentSource });
  refreshFiles();
}

async function selectFile(path) {
  if (!confirm(`¿Cargar ${path} para ejecución?`)) return;
  await fetch('/api/select', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) + '&source=' + currentSource });

  setTimeout(loadPreview, 1500);
}

document.getElementById('uploadForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  const fileInput = document.getElementById('fileInput');
  if (!fileInput.files.length) return;

  const btn = e.target.querySelector('button');
  const originalText = btn.innerHTML;
  btn.innerHTML = 'Subiendo...'; btn.disabled = true;

  const formData = new FormData();
  formData.append('file', fileInput.files[0]);

  try {
    await fetch('/api/upload?path=' + encodeURIComponent(currentPath) + '&source=' + currentSource, { method: 'POST', body: formData });
    fileInput.value = '';
  } catch(err) { alert('Error al subir el archivo.'); }
  finally { btn.innerHTML = originalText; btn.disabled = false; refreshFiles(); }
});

function toggleSidebar() {
  document.body.classList.toggle('sidebar-collapsed');
  setTimeout(resizeCanvas, 300);
}

let lastConsoleVersion = -1;

async function refreshConsole() {
  try {
    const res = await fetch('/api/console');
    const data = await res.json();

    if (data.version === lastConsoleVersion) return;
    lastConsoleVersion = data.version;

    const formattedLines = data.entries.map(e =>
      `<div class="console-line ${e.out ? 'out' : 'in'}">${escapeHtml(e.text)}</div>`
    ).join('');

    const log1 = document.getElementById('consoleLog');
    const log2 = document.getElementById('dashConsoleLog');

    if (log1) {
      const wasAtBottom = log1.scrollTop + log1.clientHeight >= log1.scrollHeight - 10;
      log1.innerHTML = formattedLines;
      if (wasAtBottom) log1.scrollTop = log1.scrollHeight;
    }

    if (log2) {
      const wasAtBottom = log2.scrollTop + log2.clientHeight >= log2.scrollHeight - 10;
      log2.innerHTML = formattedLines;
      if (wasAtBottom) log2.scrollTop = log2.scrollHeight;
    }
  } catch (e) { console.error("Console error:", e); }
}

function escapeHtml(text) {
  const div = document.createElement('div');
  div.innerText = text;
  return div.innerHTML;
}

async function sendConsoleCommandFromDash() {
  const input = document.getElementById('dashConsoleInput');
  await executeConsoleCommand(input.value.trim());
  input.value = '';
}

async function sendConsoleCommand() {
  const input = document.getElementById('consoleInput');
  await executeConsoleCommand(input.value.trim());
  input.value = '';
}

async function executeConsoleCommand(command) {
  if (!command) return;
  const res = await fetch('/api/console/send', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'command=' + encodeURIComponent(command)
  });

  if (!res.ok) {
    const msg = await res.text();
    alert('Error: ' + msg);
  }
}

document.getElementById('consoleInput').addEventListener('keypress', (e) => {
  if (e.key === 'Enter') sendConsoleCommand();
});
document.getElementById('dashConsoleInput').addEventListener('keypress', (e) => {
  if (e.key === 'Enter') sendConsoleCommandFromDash();
});

setInterval(refreshConsole, 500);
refreshConsole();

setInterval(refreshStatus, 1000);
setInterval(refreshFiles, 5000);
refreshStatus();
refreshFiles();
loadPreview();
</script>
</body>
</html>
)rawliteral";
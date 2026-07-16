#pragma once

const char WEB_PAGE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Offline Control</title>
<style>
  :root {
    --bg-base: #141416;
    --bg-card: #202023;
    --bg-surface: #2b2b30;
    --border: #35353c;
    --text-main: #f3f3f4;
    --text-muted: #a1a1aa;
    
    /* Accents */
    --bambu-green: #00e573;
    --klipper-blue: #00b4d8;
    --danger: #ef4444;
    --danger-hover: #dc2626;
    
    /* Axes */
    --axis-x-color: #ff5252;
    --axis-y-color: #69f0ae;
    --axis-z-color: #448aff;

    --radius-card: 16px;
    --radius-btn: 10px;
    --transition: all 0.2s ease;
  }

  * { box-sizing: border-box; margin: 0; padding: 0; }
  
  body { 
    background: var(--bg-base); 
    color: var(--text-main); 
    font-family: -apple-system, BlinkMacSystemFont, "Inter", "Segoe UI", Roboto, Helvetica, Arial, sans-serif; 
    padding: 20px;
    font-size: 14px;
    line-height: 1.5;
    -webkit-font-smoothing: antialiased;
  }

  /* Header (Top Bar) */
  .top-bar {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-bottom: 24px;
    padding-bottom: 16px;
    border-bottom: 1px solid var(--border);
  }

  .brand {
    font-size: 20px;
    font-weight: 800;
    letter-spacing: -0.5px;
    display: flex;
    align-items: center;
    gap: 10px;
  }

  .brand svg { width: 24px; height: 24px; color: var(--bambu-green); }

  #state {
    background: rgba(0, 229, 115, 0.15);
    color: var(--bambu-green);
    padding: 6px 14px;
    border-radius: 20px;
    font-weight: 600;
    font-size: 13px;
    border: 1px solid rgba(0, 229, 115, 0.3);
    text-transform: uppercase;
    letter-spacing: 0.5px;
  }

  /* Main Grid Layout */
  .dashboard {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(340px, 1fr));
    gap: 20px;
    align-items: start;
  }

  /* Cards */
  .card { 
    background: var(--bg-card); 
    border-radius: var(--radius-card); 
    padding: 20px; 
    box-shadow: 0 4px 24px rgba(0,0,0,0.2);
    border: 1px solid rgba(255,255,255,0.03);
  }

  .card-header {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 15px;
    font-weight: 600;
    color: var(--text-main);
    margin-bottom: 16px;
  }
  .card-header svg { width: 18px; height: 18px; color: var(--text-muted); }

  /* Toolhead / DRO */
  .dro-grid { display: flex; flex-direction: column; gap: 8px; }
  
  .dro-row {
    display: flex;
    align-items: center;
    justify-content: space-between;
    background: var(--bg-surface);
    padding: 12px 16px;
    border-radius: var(--radius-btn);
  }
  
  .dro-label { font-size: 16px; font-weight: 800; }
  .dro-label.x { color: var(--axis-x-color); }
  .dro-label.y { color: var(--axis-y-color); }
  .dro-label.z { color: var(--axis-z-color); }
  
  .dro-value {
    font-family: "JetBrains Mono", "SFMono-Regular", Consolas, monospace;
    font-size: 22px;
    font-weight: 700;
    color: #fff;
    letter-spacing: -0.5px;
  }

  /* Job Progress Monitor */
  .job-info { margin-bottom: 16px; }
  .job-filename { font-size: 16px; font-weight: 600; margin-bottom: 4px; word-break: break-all; }
  .job-feed { font-size: 13px; color: var(--text-muted); display: flex; justify-content: space-between; }

  .progress-wrap { background: var(--bg-surface); border-radius: 10px; padding: 16px; }
  
  .progress-track { 
    background: #18181b; 
    border-radius: 6px; 
    height: 12px; 
    overflow: hidden; 
    margin-bottom: 10px;
    box-shadow: inset 0 1px 3px rgba(0,0,0,0.5);
  }
  
  .progress-fill { 
    background: linear-gradient(90deg, #00b4d8, var(--bambu-green)); 
    height: 100%; 
    width: 0%; 
    border-radius: 6px;
    transition: width 0.4s cubic-bezier(0.4, 0, 0.2, 1); 
  }
  
  .progress-stats {
    display: flex;
    justify-content: space-between;
    font-size: 13px;
    font-weight: 500;
    color: var(--text-muted);
  }
  #progressPercent { color: var(--bambu-green); font-weight: 700; font-size: 16px; }

  /* Control Buttons */
  .control-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 12px; }
  
  button { 
    font-family: inherit;
    font-weight: 600;
    font-size: 14px;
    padding: 14px; 
    border: none; 
    border-radius: var(--radius-btn); 
    background: var(--bg-surface); 
    color: var(--text-main); 
    cursor: pointer;
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 8px;
    transition: var(--transition);
  }
  
  button:hover { background: #35353c; transform: translateY(-1px); }
  button:active { transform: translateY(1px); }
  
  button svg { width: 18px; height: 18px; }

  button.primary { background: rgba(0, 229, 115, 0.1); color: var(--bambu-green); border: 1px solid rgba(0, 229, 115, 0.2); }
  button.primary:hover { background: rgba(0, 229, 115, 0.2); }

  button.stop { 
    grid-column: span 2; 
    background: var(--danger); 
    color: #fff; 
  }
  button.stop:hover { background: var(--danger-hover); box-shadow: 0 4px 12px rgba(239, 68, 68, 0.3); }

  /* File Manager */
  .file-manager { display: flex; flex-direction: column; height: 100%; }
  
  .breadcrumb { 
    background: var(--bg-surface); 
    padding: 10px 14px; 
    border-radius: var(--radius-btn); 
    font-family: monospace; 
    font-size: 13px; 
    color: var(--klipper-blue); 
    margin-bottom: 12px;
  }
  
  .file-list { 
    background: var(--bg-surface); 
    border-radius: var(--radius-btn); 
    max-height: 250px; 
    overflow-y: auto; 
  }
  
  .file-list::-webkit-scrollbar { width: 6px; }
  .file-list::-webkit-scrollbar-thumb { background: #4f4f56; border-radius: 3px; }
  
  .file-row { 
    display: flex; 
    justify-content: space-between; 
    align-items: center; 
    padding: 12px 14px; 
    border-bottom: 1px solid var(--border); 
    transition: var(--transition);
  }
  .file-row:last-child { border-bottom: none; }
  .file-row:hover { background: rgba(255,255,255,0.03); }
  
  .file-name { 
    display: flex; align-items: center; gap: 10px; cursor: pointer; flex: 1; 
    font-weight: 500; font-size: 13px;
    white-space: nowrap; overflow: hidden; text-overflow: ellipsis; 
  }
  .file-name svg { color: var(--text-muted); flex-shrink: 0; }
  
  .file-actions { display: flex; gap: 6px; }
  .file-actions button { 
    padding: 6px; border-radius: 6px; background: transparent; 
  }
  .file-actions button:hover { background: var(--bg-card); }
  .icon-load { color: var(--bambu-green); }
  .icon-del { color: var(--danger); }

  /* Upload & Actions */
  .file-controls { display: flex; gap: 10px; margin-top: 12px; }
  .file-controls button { padding: 10px; flex: 1; font-size: 12px; }

  .upload-area {
    margin-top: 12px;
    background: rgba(0, 180, 216, 0.05);
    border: 1px dashed rgba(0, 180, 216, 0.3);
    border-radius: var(--radius-btn);
    padding: 12px;
    display: flex;
    flex-direction: column;
    gap: 10px;
  }
  
  input[type=file] { font-size: 12px; color: var(--text-muted); }
  input[type=file]::file-selector-button {
    background: var(--bg-card); color: var(--text-main);
    border: 1px solid var(--border); padding: 8px 12px; 
    border-radius: 6px; cursor: pointer; font-weight: 600; margin-right: 12px;
    transition: var(--transition);
  }
  input[type=file]::file-selector-button:hover { background: var(--bg-surface); }
  .step-btn { background: var(--bg-surface); }
  .step-btn.active { background: rgba(0, 229, 115, 0.15); color: var(--bambu-green); border: 1px solid rgba(0, 229, 115, 0.3); }
</style>
</head>
<body>

<div class="top-bar">
  <div class="brand">
    <svg viewBox="0 0 24 24" fill="currentColor"><path d="M19 3H5c-1.1 0-2 .9-2 2v14c0 1.1.9 2 2 2h14c1.1 0 2-.9 2-2V5c0-1.1-.9-2-2-2zm-7 9h-2V7h-2v5H6v2h2v5h2v-5h2v-2z"/></svg>
    Offline Control
  </div>
  <div id="state">Connecting...</div>
</div>

<div class="dashboard">

  <!-- Toolhead Card -->
  <div class="card">
    <div class="card-header">
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="3"/><path d="M12 2v4M12 18v4M4 12H2M22 12h-2"/></svg>
      Toolhead WCS
    </div>
    <div class="dro-grid">
      <div class="dro-row"><span class="dro-label x">X</span><span class="dro-value" id="posX">0.000</span></div>
      <div class="dro-row"><span class="dro-label y">Y</span><span class="dro-value" id="posY">0.000</span></div>
      <div class="dro-row"><span class="dro-label z">Z</span><span class="dro-value" id="posZ">0.000</span></div>
    </div>
  </div>

  <!-- Job Status Card -->
  <div class="card">
    <div class="card-header">
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/></svg>
      Job Status
    </div>
    
    <div class="job-info">
      <div class="job-filename" id="filename">No file loaded</div>
      <div class="job-feed">Feed Rate: <span id="feed" style="font-weight:bold; color:#fff;">0</span> mm/min</div>
    </div>

    <div class="progress-wrap">
      <div class="progress-track"><div id="progressBar" class="progress-fill"></div></div>
      <div class="progress-stats">
        <span id="progressText">0 / 0 lines</span>
        <span id="progressPercent">0%</span>
      </div>
    </div>
  </div>

  <!-- Machine Actions Card -->
  <div class="card">
    <div class="card-header">
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><rect x="3" y="3" width="18" height="18" rx="2" ry="2"/><line x1="9" y1="9" x2="15" y2="15"/><line x1="15" y1="9" x2="9" y2="15"/></svg>
      Machine Actions
    </div>
    <div class="control-grid">
      <button class="primary" id="btnPlayPause" onclick="sendControl('playpause')">
        <svg viewBox="0 0 24 24" fill="currentColor"><path d="M8 5v14l11-7z"/></svg> Play / Pause
      </button>
      <button onclick="sendControl('framing')">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M4 4h16v16H4z"/><path d="M4 12h16M12 4v16"/></svg> Frame
      </button>
      <button class="stop" onclick="sendControl('stop')">
        <svg viewBox="0 0 24 24" fill="currentColor"><path d="M6 6h12v12H6z"/></svg> CANCEL / STOP
      </button>
    </div>
  </div>

  <!-- Jog Control Card -->
  <div class="card">
    <div class="card-header">
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M9 3H5a2 2 0 0 0-2 2v4m6-6h10a2 2 0 0 1 2 2v4M3 15v4a2 2 0 0 0 2 2h4m10-6v4a2 2 0 0 1-2 2h-4"/></svg>
      Jog Control
    </div>

    <div style="display:flex; gap:8px; margin-bottom:14px;">
      <button class="step-btn active" data-step="0.1" onclick="selectStep(0.1, this)" style="flex:1; padding:8px;">0.1</button>
      <button class="step-btn" data-step="1" onclick="selectStep(1, this)" style="flex:1; padding:8px;">1</button>
      <button class="step-btn" data-step="10" onclick="selectStep(10, this)" style="flex:1; padding:8px;">10</button>
    </div>

    <div style="display:grid; grid-template-columns: 1fr 1fr 1fr; gap:8px; margin-bottom:12px;">
      <div></div>
      <button onclick="jog('Y', 1)" style="background:rgba(105,240,174,0.1); color:var(--axis-y-color);">Y+</button>
      <div></div>

      <button onclick="jog('X', -1)" style="background:rgba(255,82,82,0.1); color:var(--axis-x-color);">X-</button>
      <button onclick="doHome()" style="background:var(--bg-surface);">HOME</button>
      <button onclick="jog('X', 1)" style="background:rgba(255,82,82,0.1); color:var(--axis-x-color);">X+</button>

      <div></div>
      <button onclick="jog('Y', -1)" style="background:rgba(105,240,174,0.1); color:var(--axis-y-color);">Y-</button>
      <div></div>
    </div>

    <div style="display:grid; grid-template-columns: 1fr 1fr; gap:8px; margin-bottom:12px;">
      <button onclick="jog('Z', 1)" style="background:rgba(68,138,255,0.1); color:var(--axis-z-color);">Z+</button>
      <button onclick="jog('Z', -1)" style="background:rgba(68,138,255,0.1); color:var(--axis-z-color);">Z-</button>
    </div>

    <div style="display:grid; grid-template-columns: 1fr 1fr 1fr; gap:8px;">
      <button onclick="setZero('X')" style="font-size:12px;">SET X0</button>
      <button onclick="setZero('Y')" style="font-size:12px;">SET Y0</button>
      <button onclick="setZero('Z')" style="font-size:12px;">SET Z0</button>
    </div>

    <button onclick="probeZ()" style="width:100%; margin-top:8px; background:rgba(0,180,216,0.1); color:var(--klipper-blue);">PROBE Z</button>
  </div>

  <!-- Local Files (SD) Card -->
  <div class="card file-manager">
    <div class="card-header">
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M22 19a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h5l2 3h9a2 2 0 0 1 2 2z"/></svg>
      Local Files (SD)
    </div>
    
    <div id="breadcrumb" class="breadcrumb">/</div>
    
    <div id="fileList" class="file-list"></div>

    <div class="file-controls">
      <button onclick="createFolder()">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg> New Folder
      </button>
    </div>

    <form id="uploadForm" class="upload-area">
      <input type="file" id="fileInput" name="file" accept=".nc,.gcode,.ngc,.tap">
      <button type="submit" style="background:var(--klipper-blue); color:#000;">
        <svg viewBox="0 0 24 24" fill="currentColor" style="width:16px;height:16px;"><path d="M9 16h6v-6h4l-7-7-7 7h4v6zm-4 2h14v2H5v-2z"/></svg> Upload G-Code File
      </button>
    </form>
  </div>

</div>

<script>
let currentPath = '/';

// Inline SVGs
const iconFolder = '<svg viewBox="0 0 24 24" fill="currentColor" width="18" height="18"><path d="M10 4H4c-1.1 0-2 .9-2 2v12c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V8c0-1.1-.9-2-2-2h-8l-2-2z"/></svg>';
const iconFile = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="18" height="18"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><polyline points="14 2 14 8 20 8"/></svg>';
const iconUp = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="18" height="18"><polyline points="15 14 12 11 9 14"/><path d="M12 11v8"/><circle cx="12" cy="12" r="10"/></svg>';

const iconLoad = '<svg viewBox="0 0 24 24" fill="currentColor" width="16" height="16"><path d="M8 5v14l11-7z"/></svg>';
const iconEdit = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><path d="M11 4H4a2 2 0 0 0-2 2v14a2 2 0 0 0 2 2h14a2 2 0 0 0 2-2v-7"/><path d="M18.5 2.5a2.121 2.121 0 0 1 3 3L12 15l-4 1 1-4 9.5-9.5z"/></svg>';
const iconTrash = '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" width="16" height="16"><polyline points="3 6 5 6 21 6"/><path d="M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>';

async function refreshStatus() {
  try {
    const res = await fetch('/api/status');
    const data = await res.json();

    const stateEl = document.getElementById('state');
    stateEl.innerText = data.state;
    // Status color badge logic
    if(data.state.toLowerCase() === 'idle' || data.state.toLowerCase() === 'ready') {
      stateEl.style.color = '#00b4d8'; stateEl.style.background = 'rgba(0,180,216,0.1)'; stateEl.style.borderColor = 'rgba(0,180,216,0.3)';
    } else if (data.state.toLowerCase() === 'run' || data.state.toLowerCase() === 'running') {
      stateEl.style.color = '#00e573'; stateEl.style.background = 'rgba(0,229,115,0.1)'; stateEl.style.borderColor = 'rgba(0,229,115,0.3)';
    } else {
      stateEl.style.color = '#f59e0b'; stateEl.style.background = 'rgba(245,158,11,0.1)'; stateEl.style.borderColor = 'rgba(245,158,11,0.3)';
    }

    document.getElementById('filename').innerText = data.filename || 'No file';
    document.getElementById('progressBar').style.width = data.percent + '%';
    document.getElementById('progressText').innerText = data.line + ' / ' + data.totalLines + ' lines';
    document.getElementById('progressPercent').innerText = data.percent + '%';
    
    // Format coordinates
    document.getElementById('posX').innerText = Number(data.x).toFixed(3);
    document.getElementById('posY').innerText = Number(data.y).toFixed(3);
    document.getElementById('posZ').innerText = Number(data.z).toFixed(3);
    document.getElementById('feed').innerText = data.feed;
  } catch (e) { console.error("Status error:", e); }
}

async function sendControl(action) {
  await fetch('/api/control', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'action=' + action
  });
}

function joinPath(base, name) { return base === '/' ? '/' + name : base + '/' + name; }
function parentPath(path) {
  if (path === '/' || path === '') return '/';
  const idx = path.lastIndexOf('/');
  return idx <= 0 ? '/' : path.substring(0, idx);
}

async function refreshFiles() {
  try {
    const res = await fetch('/api/files?path=' + encodeURIComponent(currentPath));
    const data = await res.json();

    document.getElementById('breadcrumb').innerText = "SD: " + data.path;
    const list = document.getElementById('fileList');
    list.innerHTML = '';

    if (currentPath !== '/') {
      const upRow = document.createElement('div');
      upRow.className = 'file-row';
      upRow.innerHTML = `<div class="file-name" style="color:var(--text-muted)">${iconUp} Back</div>`;
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
          `<button onclick="renamePrompt('${fullPath}')" title="Rename">${iconEdit}</button>` +
          `<button onclick="deleteEntry('${fullPath}')" class="icon-del" title="Delete">${iconTrash}</button>`;
      } else {
        infoDiv.innerHTML = `${iconFile} ${shortName}`;
        infoDiv.onclick = () => selectFile(fullPath);
        actionsDiv.innerHTML =
          `<button onclick="selectFile('${fullPath}')" class="icon-load" title="Load to memory">${iconLoad}</button>` +
          `<button onclick="renamePrompt('${fullPath}')" title="Rename">${iconEdit}</button>` +
          `<button onclick="deleteEntry('${fullPath}')" class="icon-del" title="Delete">${iconTrash}</button>`;
      }

      row.appendChild(infoDiv);
      row.appendChild(actionsDiv);
      list.appendChild(row);
    });
  } catch (e) { console.error("Files error:", e); }
}

async function deleteEntry(path) {
  if (!confirm('Delete ' + path + '?')) return;
  await fetch('/api/delete', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) });
  refreshFiles();
}

async function renamePrompt(path) {
  const newName = prompt('New name:');
  if (!newName) return;
  await fetch('/api/rename', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) + '&newName=' + encodeURIComponent(newName) });
  refreshFiles();
}

async function createFolder() {
  const name = prompt('Folder name:');
  if (!name) return;
  await fetch('/api/mkdir', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(joinPath(currentPath, name)) });
  refreshFiles();
}

async function selectFile(path) {
  if (!confirm('Load ' + path + ' for execution?')) return;
  await fetch('/api/select', { method: 'POST', headers: {'Content-Type':'application/x-www-form-urlencoded'}, body: 'path=' + encodeURIComponent(path) });
}

document.getElementById('uploadForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  const fileInput = document.getElementById('fileInput');
  if (!fileInput.files.length) return;

  const btn = e.target.querySelector('button');
  const originalText = btn.innerHTML;
  btn.innerHTML = 'Uploading...'; btn.disabled = true;

  const formData = new FormData();
  formData.append('file', fileInput.files[0]);

  try {
    await fetch('/api/upload?path=' + encodeURIComponent(currentPath), { method: 'POST', body: formData });
    fileInput.value = '';
  } catch(err) { alert('Upload error.'); } 
  finally { btn.innerHTML = originalText; btn.disabled = false; refreshFiles(); }
});

let jogStep = 0.1;

function selectStep(step, btn) {
  jogStep = step;
  document.querySelectorAll('.step-btn').forEach(b => b.classList.remove('active'));
  btn.classList.add('active');
}

async function jog(axis, direction) {
  const distance = jogStep * direction;
  await fetch('/api/jog', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'axis=' + axis + '&distance=' + distance
  });
}

async function doHome() {
  if (!confirm('Perform full homing?')) return;
  await fetch('/api/home', { method: 'POST' });
}

async function setZero(axis) {
  if (!confirm('Fijar ' + axis + '0 en la posicion actual?')) return;
  await fetch('/api/setzero', {
    method: 'POST',
    headers: {'Content-Type':'application/x-www-form-urlencoded'},
    body: 'axis=' + axis
  });
}

async function probeZ() {
  if (!confirm('Run Probe Z? Confirms that the probe is connected.')) return;
  await fetch('/api/probez', { method: 'POST' });
}

setInterval(refreshStatus, 1000);
setInterval(refreshFiles, 5000);
refreshStatus();
refreshFiles();
</script>
</body>
</html>
)rawliteral";
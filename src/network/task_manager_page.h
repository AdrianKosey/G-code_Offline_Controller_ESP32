#pragma once
#include <Arduino.h>

const char TASK_MANAGER_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Performance Panel</title>
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

        * { box-sizing: border-box; }

        body {
            font-family: 'Segoe UI', system-ui, -apple-system, sans-serif;
            background-color: var(--bg-base);
            color: var(--text-main);
            margin: 0;
            padding: 0;
            display: flex;
            flex-direction: column;
            height: 100vh;
            overflow: hidden;
        }

        /* Top Navigation Header */
        header {
            background: var(--bg-sidebar);
            padding: 12px 20px;
            border-bottom: 1px solid var(--border);
            display: flex;
            justify-content: space-between;
            align-items: center;
            box-shadow: var(--shadow);
        }

        .header-brand {
            display: flex;
            align-items: center;
            gap: 15px;
        }

        .brand-title {
            font-size: 1.1rem;
            font-weight: 800;
            letter-spacing: 1.5px;
            color: var(--text-main);
            text-transform: uppercase;
            display: flex;
            align-items: center;
            gap: 8px;
        }

        .brand-title span {
            color: var(--accent);
        }

        .btn-back {
            display: inline-flex;
            align-items: center;
            gap: 8px;
            background: var(--bg-surface);
            color: var(--text-main);
            border: 1px solid var(--border);
            padding: 6px 14px;
            border-radius: var(--radius-btn);
            font-size: 0.8rem;
            font-weight: 600;
            text-transform: uppercase;
            letter-spacing: 1px;
            text-decoration: none;
            transition: all 0.2s ease;
        }

        .btn-back:hover {
            border-color: var(--accent);
            color: var(--accent);
            background: var(--accent-dim);
        }

        .live-status {
            display: flex;
            align-items: center;
            gap: 8px;
            font-size: 0.8rem;
            color: var(--text-muted);
            font-family: monospace;
        }

        .status-dot {
            width: 8px;
            height: 8px;
            background-color: var(--success);
            border-radius: 50%;
            box-shadow: 0 0 8px var(--success);
            animation: pulse 2s infinite;
        }

        @keyframes pulse {
            0% { opacity: 1; }
            50% { opacity: 0.3; }
            100% { opacity: 1; }
        }

        /* Layout Container */
        .dashboard-container {
            display: grid;
            grid-template-columns: 320px 1fr;
            gap: 15px;
            padding: 15px;
            flex: 1;
            overflow: hidden;
        }

        .sidebar {
            display: flex;
            flex-direction: column;
            gap: 15px;
            overflow-y: auto;
        }

        /* Panels & Cards */
        .panel {
            background: var(--bg-panel);
            border: 1px solid var(--border);
            border-radius: var(--radius-card);
            overflow: hidden;
            box-shadow: var(--shadow);
        }

        .panel-header {
            background: var(--bg-surface);
            padding: 10px 14px;
            font-size: 0.75rem;
            font-weight: 700;
            color: var(--accent);
            text-transform: uppercase;
            letter-spacing: 1.2px;
            border-bottom: 1px solid var(--border);
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .panel-body {
            padding: 15px;
        }

        /* Progress Meters */
        .metric-item {
            margin-bottom: 14px;
        }
        .metric-item:last-child { margin-bottom: 0; }

        .metric-info {
            display: flex;
            justify-content: space-between;
            font-size: 0.8rem;
            margin-bottom: 6px;
        }

        .metric-label {
            color: var(--text-muted);
            font-weight: 600;
        }

        .metric-val {
            font-family: monospace;
            font-weight: bold;
            color: var(--text-main);
        }

        .meter-bg {
            background: var(--bg-base);
            height: 8px;
            border-radius: 4px;
            overflow: hidden;
            border: 1px solid var(--border);
        }

        .meter-fill {
            height: 100%;
            background: var(--accent);
            width: 0%;
            transition: width 0.4s ease, background-color 0.3s ease;
        }

        /* DRO Display Boxes */
        .dro-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 10px;
        }

        .dro-card {
            background: var(--bg-surface);
            border: 1px solid var(--border);
            border-radius: var(--radius-btn);
            padding: 10px;
            text-align: center;
        }

        .dro-card.full-width {
            grid-column: span 2;
        }

        .dro-value {
            font-family: monospace;
            font-size: 1.2rem;
            font-weight: bold;
            color: var(--accent);
            letter-spacing: 1px;
        }

        .dro-label {
            font-size: 0.65rem;
            color: var(--text-muted);
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-top: 4px;
        }

        /* Task Table Panel */
        .main-panel {
            background: var(--bg-panel);
            border: 1px solid var(--border);
            border-radius: var(--radius-card);
            display: flex;
            flex-direction: column;
            overflow: hidden;
            box-shadow: var(--shadow);
        }

        .table-responsive {
            flex: 1;
            overflow-y: auto;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            font-size: 0.85rem;
            text-align: left;
        }

        th {
            background: var(--bg-surface);
            color: var(--text-muted);
            padding: 12px 16px;
            font-size: 0.75rem;
            text-transform: uppercase;
            letter-spacing: 1px;
            position: sticky;
            top: 0;
            border-bottom: 1px solid var(--border);
            z-index: 10;
        }

        td {
            padding: 10px 16px;
            border-bottom: 1px solid var(--border);
        }

        tr:hover {
            background: var(--sidebar-selected);
        }

        /* Badges CNC style */
        .badge {
            padding: 3px 8px;
            border-radius: var(--radius-btn);
            font-size: 0.7rem;
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.5px;
            display: inline-block;
        }

        .badge-running { background: rgba(0, 230, 118, 0.15); color: var(--success); border: 1px solid var(--success); }
        .badge-blocked { background: rgba(255, 179, 0, 0.15); color: var(--warning); border: 1px solid var(--warning); }
        .badge-ready { background: var(--accent-dim); color: var(--accent); border: 1px solid var(--accent); }
        .badge-suspended { background: var(--bg-surface); color: var(--text-muted); border: 1px solid var(--border); }

        .core-badge {
            font-family: monospace;
            padding: 2px 6px;
            border-radius: 4px;
            font-size: 0.75rem;
            font-weight: bold;
        }
        .core-0 { background: rgba(64, 196, 255, 0.15); color: var(--axis-z); border: 1px solid var(--axis-z); }
        .core-1 { background: var(--accent-dim); color: var(--accent); border: 1px solid var(--accent); }
        .core-any { background: var(--bg-surface); color: var(--text-muted); border: 1px solid var(--border); }

        @media (max-width: 850px) {
            .dashboard-container { grid-template-columns: 1fr; overflow-y: auto; }
            body { height: auto; overflow: auto; }
            header { flex-direction: column; gap: 10px; align-items: flex-start; }
            .header-brand { width: 100%; justify-content: space-between; }
        }
    </style>
</head>
<body>

<header>
    <div class="header-brand">
        <a href="/" class="btn-back">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round"><line x1="19" y1="12" x2="5" y2="12"></line><polyline points="12 19 5 12 12 5"></polyline></svg>
            Return
        </a>
        <div class="brand-title">
            <span>PERFORMANCE</span> PANEL
        </div>
    </div>
    <div class="live-status">
        <div class="status-dot"></div>
        <span>UPDATED: <span id="last-update" style="color: var(--accent);">--:--:--</span></span>
    </div>
</header>

<div class="dashboard-container">
    
    <!-- Sidebar: Hardware & Memory Telemetry -->
    <div class="sidebar">
        
        <!-- Memory Panel -->
        <div class="panel">
            <div class="panel-header">
                <span>RAM and System</span>
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M6 19v-2a2 2 0 0 1 2-2h8a2 2 0 0 1 2 2v2"/><rect x="4" y="3" width="16" height="12" rx="2"/></svg>
            </div>
            <div class="panel-body">
                <div class="metric-item">
                    <div class="metric-info">
                        <span class="metric-label">Available Heap</span>
                        <span id="ram-text" class="metric-val">0 KB / 0 KB</span>
                    </div>
                    <div class="meter-bg"><div id="ram-bar" class="meter-fill"></div></div>
                </div>

                <div class="metric-item">
                    <div class="metric-info">
                        <span class="metric-label" title="Pico máximo histórico de uso">Maximum RAM Usage</span>
                        <span id="ram-min-text" class="metric-val">0 KB / 0 KB</span>
                    </div>
                    <div class="meter-bg"><div id="ram-min-bar" class="meter-fill"></div></div>
                </div>

                <div class="metric-item" id="psram-group" style="display: none;">
                    <div class="metric-info">
                        <span class="metric-label">PSRAM Externa</span>
                        <span id="psram-text" class="metric-val">0 KB / 0 KB</span>
                    </div>
                    <div class="meter-bg"><div id="psram-bar" class="meter-fill"></div></div>
                </div>

                <div class="metric-item" id="spiffs-group" style="display: none;">
                    <div class="metric-info">
                        <span class="metric-label">Almacenamiento Local</span>
                        <span id="spiffs-text" class="metric-val">0 KB / 0 KB</span>
                    </div>
                    <div class="meter-bg"><div id="spiffs-bar" class="meter-fill"></div></div>
                </div>
            </div>
        </div>

        <!-- Telemetry Panel -->
        <div class="panel">
            <div class="panel-header">
                <span>Hardware Telemetry</span>
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M18 10h-1.26A8 8 0 1 0 9 20h9a5 5 0 0 0 0-10z"/></svg>
            </div>
            <div class="panel-body">
                <div class="dro-grid">
                    <div class="dro-card">
                        <div id="cpu-freq" class="dro-value">-- MHz</div>
                        <div class="dro-label">CPU clock</div>
                    </div>
                    <div class="dro-card">
                        <div id="core-temp" class="dro-value">-- °C</div>
                        <div class="dro-label">Core Temp.</div>
                    </div>
                    <div class="dro-card full-width">
                        <div id="uptime" class="dro-value" style="color: var(--success);">00:00:00</div>
                        <div class="dro-label">Uptime</div>
                    </div>
                </div>
            </div>
        </div>

        <!-- Network Panel -->
        <div class="panel">
            <div class="panel-header">
                <span>Network Connectivity</span>
                <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M5 12.55a11 11 0 0 1 14.08 0"/><path d="M1.42 9a16 16 0 0 1 21.16 0"/><path d="M8.53 16.11a6 6 0 0 1 6.95 0"/><line x1="12" y1="20" x2="12.01" y2="20"/></svg>
            </div>
            <div class="panel-body">
                <div class="metric-item">
                    <div class="metric-info">
                        <span class="metric-label">Signal Quality</span>
                        <span id="wifi-rssi-text" class="metric-val">-- dBm</span>
                    </div>
                    <div class="meter-bg"><div id="wifi-bar" class="meter-fill"></div></div>
                </div>
                <div style="font-size: 0.75rem; color: var(--text-muted); margin-top: 10px; font-family: monospace;">
                    <div style="display: flex; justify-content: space-between; margin-bottom: 4px;">
                        <span>MODE:</span> <span id="net-mode" style="color: var(--accent);">--</span>
                    </div>
                    <div style="display: flex; justify-content: space-between; margin-bottom: 4px;">
                        <span>IP:</span> <span id="ip-address" style="color: var(--text-main);">--.--.--.--</span>
                    </div>
                    <div style="display: flex; justify-content: space-between;">
                        <span>MAC:</span> <span id="mac-address" style="color: var(--text-main);">--:--:--:--:--:--</span>
                    </div>
                </div>
            </div>
        </div>

    </div>

    <!-- Main Panel: FreeRTOS Task Scheduler -->
    <div class="main-panel">
        <div class="panel-header">
            <span>Active Processes and Tasks</span>
            <span style="font-size: 0.7rem; color: var(--text-muted);">Multitarea ESP32</span>
        </div>
        <div class="table-responsive">
            <table>
                <thead>
                    <tr>
                        <th>Task Name</th>
                        <th>State</th>
                        <th>Priority</th>
                        <th>Free Stack (Words)</th>
                        <th>Assigned Core</th>
                    </tr>
                </thead>
                <tbody id="tasks-table-body">
                    <tr>
                        <td colspan="5" style="text-align: center; color: var(--text-muted); padding: 30px;">Loading tasks in real-time...</td>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>

</div>

<script>
    function formatBytes(bytes) {
        if (bytes === 0) return '0 Bytes';
        const k = 1024;
        const sizes = ['Bytes', 'KB', 'MB', 'GB'];
        const i = Math.floor(Math.log(bytes) / Math.log(k));
        return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
    }

    function formatUptime(seconds) {
        const h = Math.floor(seconds / 3600);
        const m = Math.floor((seconds % 3600) / 60);
        const s = Math.floor(seconds % 60);
        return `${h.toString().padStart(2, '0')}:${m.toString().padStart(2, '0')}:${s.toString().padStart(2, '0')}`;
    }

    function updateMeter(elementId, used, total) {
        const bar = document.getElementById(elementId);
        if (!bar) return;
        const percent = total > 0 ? (used / total) * 100 : 0;
        bar.style.width = percent + '%';
        
        if (elementId === 'wifi-bar') return;

        if (percent > 85) {
            bar.style.backgroundColor = 'var(--danger)';
        } else if (percent > 65) {
            bar.style.backgroundColor = 'var(--warning)';
        } else {
            bar.style.backgroundColor = 'var(--accent)';
        }
    }

    function getStatusBadge(status) {
        const statusMap = {
            'Running': '<span class="badge badge-running">EJC (Running)</span>',
            'Blocked': '<span class="badge badge-blocked">BLQ (Blocked)</span>',
            'Ready': '<span class="badge badge-ready">LST (Ready)</span>',
            'Suspended': '<span class="badge badge-suspended">SUS (Suspended)</span>'
        };
        return statusMap[status] || `<span class="badge badge-ready">${status}</span>`;
    }

    function getCoreBadge(coreId) {
        if (coreId === 0) return '<span class="core-badge core-0">Core 0</span>';
        if (coreId === 1) return '<span class="core-badge core-1">Core 1</span>';
        return '<span class="core-badge core-any">Any</span>';
    }

    async function fetchSystemStats() {
        try {
            const response = await fetch('/api/stats');
            if (!response.ok) throw new Error('Connection error');
            const data = await response.json();

            const now = new Date();
            document.getElementById('last-update').textContent = now.toLocaleTimeString();

            // RAM
            const ramUsed = data.ram.total - data.ram.free;
            document.getElementById('ram-text').textContent = `${formatBytes(ramUsed)} / ${formatBytes(data.ram.total)}`;
            updateMeter('ram-bar', ramUsed, data.ram.total);

            // Pico Max RAM
            const peakRamUsed = data.ram.total - data.ram.min_free;
            document.getElementById('ram-min-text').textContent = `${formatBytes(peakRamUsed)} / ${formatBytes(data.ram.total)}`;
            updateMeter('ram-min-bar', peakRamUsed, data.ram.total);

            // PSRAM
            if (data.psram && data.psram.total > 0) {
                document.getElementById('psram-group').style.display = 'block';
                const psramUsed = data.psram.total - data.psram.free;
                document.getElementById('psram-text').textContent = `${formatBytes(psramUsed)} / ${formatBytes(data.psram.total)}`;
                updateMeter('psram-bar', psramUsed, data.psram.total);
            }

            // SPIFFS
            if (data.spiffs && data.spiffs.total > 0) {
                document.getElementById('spiffs-group').style.display = 'block';
                document.getElementById('spiffs-text').textContent = `${formatBytes(data.spiffs.used)} / ${formatBytes(data.spiffs.total)}`;
                updateMeter('spiffs-bar', data.spiffs.used, data.spiffs.total);
            }

            // Telemetría Hardware
            document.getElementById('cpu-freq').textContent = `${data.hardware.cpu_freq} MHz`;
            document.getElementById('uptime').textContent = formatUptime(data.hardware.uptime);
            if (data.hardware.temp) {
                document.getElementById('core-temp').textContent = `${data.hardware.temp.toFixed(1)} °C`;
            }

            // Conectividad
            document.getElementById('net-mode').textContent = data.network.mode;
            document.getElementById('ip-address').textContent = data.network.ip;
            document.getElementById('mac-address').textContent = data.network.mac;

            if (data.network.mode === 'Ethernet') {
                document.getElementById('wifi-rssi-text').textContent = "Cableado";
                document.getElementById('wifi-bar').style.width = '100%';
                document.getElementById('wifi-bar').style.backgroundColor = 'var(--accent)';
            } else {
                document.getElementById('wifi-rssi-text').textContent = `${data.network.rssi} dBm`;
                let rssiPercent = 0;
                if (data.network.rssi >= -50) rssiPercent = 100;
                else if (data.network.rssi <= -100) rssiPercent = 0;
                else rssiPercent = 2 * (data.network.rssi + 100);

                document.getElementById('wifi-bar').style.width = rssiPercent + '%';
                document.getElementById('wifi-bar').style.backgroundColor = 'var(--accent)';
            }

            // Tabla de FreeRTOS
            const tbody = document.getElementById('tasks-table-body');
            tbody.innerHTML = '';

            if (data.tasks && data.tasks.length > 0) {
                data.tasks.forEach(task => {
                    const row = `<tr>
                        <td><strong style="color: var(--text-main); font-family: monospace;">${task.name}</strong></td>
                        <td>${getStatusBadge(task.state)}</td>
                        <td style="font-family: monospace;">${task.priority}</td>
                        <td style="font-family: monospace; color: var(--accent);">${task.stack_high_watermark}</td>
                        <td>${getCoreBadge(task.core)}</td>
                    </tr>`;
                    tbody.innerHTML += row;
                });
            } else {
                tbody.innerHTML = '<tr><td colspan="5" style="text-align: center; color: var(--text-muted);">Sin datos de procesos</td></tr>';
            }

        } catch (error) {
            console.error('Error retrieving metrics:', error);
            document.getElementById('last-update').textContent = "OFFLINE";
            document.getElementById('last-update').style.color = "var(--danger)";
        }
    }

    fetchSystemStats();
    setInterval(fetchSystemStats, 2000);
</script>
</body>
</html>
)rawliteral";
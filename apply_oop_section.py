# apply_oop_section.py
# Implements the dedicated, fully visible PSOOP / OOP section directly into Web_Simulation/index.html

import re
import sys

def apply():
    bak_path = r"Web_Simulation/index.html.bak"
    out_path = r"Web_Simulation/index.html"

    with open(bak_path, "r", encoding="utf-8") as f:
        content = f.read()

    # 1. Add topbar navigation button inside <div class="topRight">
    top_right_target = '<div class="systemBadge">'
    nav_btn_html = '''<a href="#psoopSection" class="tag" style="background:linear-gradient(135deg,rgba(155,124,255,0.22),rgba(69,230,255,0.22));border:1px solid var(--cyan);color:#fff;font-weight:700;font-size:11px;padding:6px 14px;border-radius:20px;cursor:pointer;text-decoration:none;display:inline-flex;align-items:center;gap:6px;box-shadow:0 0 14px var(--cyan-glow)" onclick="scrollToOOP(event)">
            <span>🧩</span>
            <span>PSOOP / OOP</span>
        </a>
        <div class="systemBadge">'''
    
    if top_right_target not in content:
        print("ERROR: systemBadge not found")
        sys.exit(1)
    content = content.replace(top_right_target, nav_btn_html, 1)

    # 2. Add dashNavStrip right after </section> of quickStats
    quick_stats_match = re.search(r'(<section\s+class=["\']quickStats["\'].*?</section>)', content, re.DOTALL)
    if not quick_stats_match:
        print("ERROR: quickStats section not found")
        sys.exit(1)
    
    dash_nav_strip = '''
    <!-- QUICK JUMP NAVIGATION STRIP -->
    <div class="dashNavStrip" style="display:flex;align-items:center;gap:10px;margin-bottom:16px;padding:8px 0;overflow-x:auto">
        <a href="#trafficCanvas" class="dashNavBtn" style="padding:7px 14px;background:#0e1126;border:1px solid var(--border);border-radius:6px;color:var(--text-dim);font-size:11px;font-weight:600;text-decoration:none;display:inline-flex;align-items:center;gap:6px">
            <span>🚦</span> <span>Traffic Junction</span>
        </a>
        <a href="#cgPanelSection" class="dashNavBtn" style="padding:7px 14px;background:#0e1126;border:1px solid var(--border);border-radius:6px;color:var(--text-dim);font-size:11px;font-weight:600;text-decoration:none;display:inline-flex;align-items:center;gap:6px">
            <span>🎨</span> <span>Computer Graphics</span>
        </a>
        <a href="#psoopSection" class="dashNavBtn" style="padding:7px 16px;background:linear-gradient(135deg,rgba(155,124,255,0.25),rgba(69,230,255,0.2));border:1px solid var(--cyan);border-radius:6px;color:#fff;font-size:11px;font-weight:700;text-decoration:none;display:inline-flex;align-items:center;gap:6px;box-shadow:0 0 14px var(--cyan-glow)" onclick="scrollToOOP(event)">
            <span>🧩</span> <span>PSOOP / OOP — Object-Oriented Programming</span>
        </a>
        <a href="#vehicleTableSection" class="dashNavBtn" style="padding:7px 14px;background:#0e1126;border:1px solid var(--border);border-radius:6px;color:var(--text-dim);font-size:11px;font-weight:600;text-decoration:none;display:inline-flex;align-items:center;gap:6px">
            <span>📋</span> <span>Vehicle Database</span>
        </a>
    </div>
    '''
    content = content[:quick_stats_match.end()] + "\n" + dash_nav_strip + "\n" + content[quick_stats_match.end():]

    # 3. Add id="vehicleTableSection" to tableCard
    content = content.replace('<section class="card tableCard">', '<section class="card tableCard" id="vehicleTableSection">', 1)

    # 4. Construct the complete, clearly visible PSOOP / OOP Section
    oop_section_html = '''
    <!-- ============================================================
         PSOOP / OOP — OBJECT-ORIENTED PROGRAMMING SECTION
         5 Core Concepts + Interactive Live Database Demonstration
         ============================================================ -->
    <section class="card oopPanel" id="psoopSection" style="margin-top:20px;border-left:4px solid var(--purple)">
        <div class="cardHeader">
            <div class="cardTitle">
                <i style="color:var(--purple);box-shadow:0 0 10px rgba(155,124,255,0.4)">✦</i>
                <span>PSOOP / OOP — OBJECT-ORIENTED PROGRAMMING</span>
            </div>
            <div style="display:flex;align-items:center;gap:10px">
                <span class="tag" style="background:rgba(155,124,255,0.12);color:var(--purple);border:1px solid rgba(155,124,255,0.3)">
                    LIVE ARCHITECTURE
                </span>
                <span class="tag" style="background:rgba(53,233,140,0.1);color:var(--green);border:1px solid rgba(53,233,140,0.3)">
                    INTERACTIVE DEMO
                </span>
            </div>
        </div>

        <div class="cardBody">
            <!-- 5 CORE OOP CONCEPT CARDS -->
            <div class="oopGrid5" style="display:grid;grid-template-columns:repeat(auto-fit, minmax(220px, 1fr));gap:12px;margin-bottom:18px">
                
                <!-- CARD 1: CLASSES & OBJECTS -->
                <div class="oopCard" style="background:#090c1e;border:1px solid #1a203c;border-radius:8px;padding:14px">
                    <div style="display:flex;align-items:center;gap:8px;margin-bottom:8px">
                        <span style="font-size:16px">🏛️</span>
                        <b style="color:var(--cyan);font-size:12px">1. CLASSES & OBJECTS</b>
                    </div>
                    <p style="font-size:10px;color:var(--text-dim);line-height:1.5;margin-bottom:10px">
                        The traffic network is modeled using structured classes: <code>Vehicle</code>, <code>EmergencyVehicle</code>, <code>TrafficSignal</code>, <code>TrafficLane</code>, and <code>TrafficController</code>.
                    </p>
                    <div style="background:#050713;border:1px solid #14182e;border-radius:6px;padding:8px;font-family:var(--font-mono);font-size:9.5px;color:#cfd6f5">
                        <span style="color:var(--purple)">LIVE INSTANCES:</span><br>
                        <span id="oopLiveInstances">Loading instances...</span>
                    </div>
                </div>

                <!-- CARD 2: ENCAPSULATION -->
                <div class="oopCard" style="background:#090c1e;border:1px solid #1a203c;border-radius:8px;padding:14px">
                    <div style="display:flex;align-items:center;gap:8px;margin-bottom:8px">
                        <span style="font-size:16px">🔒</span>
                        <b style="color:var(--green);font-size:12px">2. ENCAPSULATION</b>
                    </div>
                    <p style="font-size:10px;color:var(--text-dim);line-height:1.5;margin-bottom:10px">
                        Vehicle state (<code>id</code>, <code>speed</code>, <code>waitTime</code>, <code>status</code>) and operations (<code>updateTelemetry</code>, <code>calculatePriority</code>) are bound together inside class objects.
                    </p>
                    <div style="background:#050713;border:1px solid #14182e;border-radius:6px;padding:8px;font-family:var(--font-mono);font-size:9.5px;color:#cfd6f5">
                        <span style="color:var(--green)">ACCESS CONTROL:</span><br>
                        • Modifying wait time via <code>UPDATE</code> form validates inputs.<br>
                        • State variables protected against external corruption.
                    </div>
                </div>

                <!-- CARD 3: INHERITANCE -->
                <div class="oopCard" style="background:#090c1e;border:1px solid #1a203c;border-radius:8px;padding:14px">
                    <div style="display:flex;align-items:center;gap:8px;margin-bottom:8px">
                        <span style="font-size:16px">🧬</span>
                        <b style="color:var(--amber);font-size:12px">3. INHERITANCE</b>
                    </div>
                    <p style="font-size:10px;color:var(--text-dim);line-height:1.5;margin-bottom:10px">
                        <code>EmergencyVehicle</code> specializes base <code>Vehicle</code>. It inherits common mobility attributes (speed, lane, coordinates) while adding emergency-specific behavior.
                    </p>
                    <div style="background:#050713;border:1px solid #14182e;border-radius:6px;padding:8px;font-family:var(--font-mono);font-size:9.5px;color:#cfd6f5">
                        <span style="color:var(--amber)">HIERARCHY:</span><br>
                        <code>Vehicle</code> (Base)<br>
                        ↳ <code>EmergencyVehicle</code> (Specialized)<br>
                        Adds siren flasher, maxSpeed (3.8), priority bonus.
                    </div>
                </div>

                <!-- CARD 4: POLYMORPHISM -->
                <div class="oopCard" style="background:#090c1e;border:1px solid #1a203c;border-radius:8px;padding:14px">
                    <div style="display:flex;align-items:center;gap:8px;margin-bottom:8px">
                        <span style="font-size:16px">⚡</span>
                        <b style="color:var(--purple);font-size:12px">4. POLYMORPHISM</b>
                    </div>
                    <p style="font-size:10px;color:var(--text-dim);line-height:1.5;margin-bottom:10px">
                        Both normal and emergency vehicles share the common method interface <code>calculatePriorityScore()</code>, but execute polymorphic rules at runtime.
                    </p>
                    <div style="background:#050713;border:1px solid #14182e;border-radius:6px;padding:8px;font-family:var(--font-mono);font-size:9.5px;color:#cfd6f5">
                        <span style="color:var(--purple)">DYNAMIC DISPATCH:</span><br>
                        • Normal: <code>wait + (density * 2)</code><br>
                        • Emergency: <code>wait + (density * 2) + 50 BONUS</code><br>
                        Drives automatic signal preemption.
                    </div>
                </div>

                <!-- CARD 5: ABSTRACTION -->
                <div class="oopCard" style="background:#090c1e;border:1px solid #1a203c;border-radius:8px;padding:14px">
                    <div style="display:flex;align-items:center;gap:8px;margin-bottom:8px">
                        <span style="font-size:16px">🎛️</span>
                        <b style="color:var(--cyan);font-size:12px">5. ABSTRACTION</b>
                    </div>
                    <p style="font-size:10px;color:var(--text-dim);line-height:1.5;margin-bottom:10px">
                        The traffic controller provides clean high-level operations (<code>START PRIORITY</code>, <code>STOP</code>, <code>INSERT</code>) while hiding complex internal mechanics.
                    </p>
                    <div style="background:#050713;border:1px solid #14182e;border-radius:6px;padding:8px;font-family:var(--font-mono);font-size:9.5px;color:#cfd6f5">
                        <span style="color:var(--cyan)">HIDDEN COMPLEXITY:</span><br>
                        • FIFO queue array memory management.<br>
                        • Signal phase timer state machine.<br>
                        • Collision detection and vehicle physics.
                    </div>
                </div>

            </div>

            <!-- STEP 3: INTERACTIVE OOP DEMONSTRATION WORKBENCH -->
            <div class="oopDemoBench" style="background:linear-gradient(135deg, rgba(14,18,38,0.9), rgba(9,11,24,0.95));border:1px solid rgba(155,124,255,0.25);border-radius:10px;padding:16px">
                <div style="display:flex;align-items:center;justify-content:space-between;flex-wrap:wrap;gap:10px;margin-bottom:12px">
                    <div style="display:flex;align-items:center;gap:10px">
                        <span style="font-size:20px">🧪</span>
                        <div>
                            <div style="font-size:13px;font-weight:800;letter-spacing:0.8px;color:#fff">INTERACTIVE OOP BEHAVIOR DEMONSTRATION</div>
                            <div style="font-size:9.5px;color:var(--text-muted)">Select a vehicle from the live database and run the OOP decision evaluation</div>
                        </div>
                    </div>
                    <span class="tag" style="background:rgba(69,230,255,0.1);color:var(--cyan);border:1px solid rgba(69,230,255,0.3)">
                        LIVE DATABASE CONNECTED
                    </span>
                </div>

                <div style="display:flex;flex-wrap:wrap;align-items:center;gap:12px;margin-bottom:14px">
                    <div style="display:flex;flex-direction:column;gap:4px">
                        <label style="font-size:9px;color:var(--text-muted);font-family:var(--font-mono)">CHOOSE VEHICLE FROM DATABASE:</label>
                        <select id="oopVehicleSelect" style="height:36px;min-width:260px;padding:0 12px;background:#050714;border:1px solid #232b50;border-radius:6px;color:#fff;font-family:var(--font-sans);font-size:11px;outline:none">
                            <option value="">Loading vehicles...</option>
                        </select>
                    </div>

                    <button class="btn-insert" id="btnRunOOPDemo" onclick="runOOPDemo()" style="height:36px;padding:0 22px;font-weight:800;letter-spacing:1px;font-size:11px;margin-top:16px">
                        ▶ RUN OOP DEMO
                    </button>

                    <button class="btn-clear" onclick="refreshOOPVehicleList()" style="height:36px;padding:0 14px;font-size:10px;margin-top:16px">
                        🔄 Refresh List
                    </button>

                    <button class="btn-update" onclick="oopQuickAddEmergency()" style="height:36px;padding:0 14px;font-size:10px;margin-top:16px;background:linear-gradient(135deg,#ff4d5e,#c8283a);color:#fff">
                        + Add Sample Ambulance (#999)
                    </button>
                </div>

                <!-- OUTPUT AREA -->
                <div id="oopDemoOutput" style="background:#050711;border:1px solid #1a2244;border-radius:8px;padding:14px;font-family:var(--font-mono);font-size:11px;min-height:90px;line-height:1.6">
                    <div style="color:var(--text-muted);text-align:center;padding:15px 0">
                        Select a vehicle from the dropdown above and click <b>RUN OOP DEMO</b> to execute the OOP evaluation.
                    </div>
                </div>
            </div>

        </div>
    </section>
    '''

    # Insert right after </section> of cgPanelSection (around line 1443)
    cg_panel_end_match = re.search(r'(<section\s+class=["\']card\s+cgPanel["\'].*?</section>)', content, re.DOTALL)
    if not cg_panel_end_match:
        print("ERROR: cgPanelSection not found")
        sys.exit(1)

    cg_end_pos = cg_panel_end_match.end()
    content = content[:cg_end_pos] + "\n" + oop_section_html + "\n" + content[cg_end_pos:]

    # 5. Add JavaScript functions for OOP Section
    js_oop_code = '''
// ==========================================
// PSOOP / OOP SECTION CONTROLLER & LIVE DEMO
// ==========================================

function scrollToOOP(e) {
    if (e) e.preventDefault();
    const sec = document.getElementById("psoopSection");
    if (sec) {
        sec.scrollIntoView({ behavior: "smooth", block: "start" });
        sec.style.boxShadow = "0 0 35px rgba(155, 124, 255, 0.5)";
        setTimeout(() => { sec.style.boxShadow = ""; }, 2000);
    }
}

function oopQuickAddEmergency() {
    if (vehicles.some(v => v.id === 999)) {
        showToast("info", "Already Exists", "Ambulance #999 is already in the database.");
        const sel = document.getElementById("oopVehicleSelect");
        if (sel) { sel.value = 999; runOOPDemo(); }
        return;
    }
    const newAmbulance = new SimulationVehicle(999, "Ambulance", true, 25, "EW_EB");
    vehicles.push(newAmbulance);
    linkedList.push(999);
    displayVehicles();
    refreshOOPVehicleList();
    const sel = document.getElementById("oopVehicleSelect");
    if (sel) { sel.value = 999; runOOPDemo(); }
    showToast("emergency", "Ambulance Registered", "Ambulance #999 added to registry. Evaluated in OOP demo!");
}

function refreshOOPVehicleList() {
    const sel = document.getElementById("oopVehicleSelect");
    if (!sel) return;

    const currentVal = sel.value;
    sel.innerHTML = "";

    if (vehicles.length === 0) {
        sel.innerHTML = '<option value="">(Database is empty)</option>';
        const out = document.getElementById("oopDemoOutput");
        if (out) {
            out.innerHTML = `
                <div style="color:var(--amber);font-weight:700">⚠ VEHICLE DATABASE IS CURRENTLY EMPTY</div>
                <div style="color:var(--text-dim);margin-top:4px">
                    There are no vehicles registered in the traffic network. Please use the <b>Vehicle Control</b> form above to <b>+ INSERT</b> a new vehicle (Car, Bike, Bus, or Ambulance), then return here to test OOP behavior.
                </div>
            `;
        }
        const liveInstEl = document.getElementById("oopLiveInstances");
        if (liveInstEl) liveInstEl.innerHTML = '<span style="color:var(--text-muted)">No active vehicle objects.</span>';
        return;
    }

    vehicles.forEach(v => {
        const opt = document.createElement("option");
        opt.value = v.id;
        opt.textContent = `#${v.id} - ${v.type} (${v.emergency ? '🚨 EMERGENCY' : 'Normal'}, Wait: ${v.wait}s)`;
        sel.appendChild(opt);
    });

    if (currentVal && vehicles.some(v => v.id === Number(currentVal))) {
        sel.value = currentVal;
    } else if (vehicles.length > 0) {
        sel.value = vehicles[0].id;
    }

    // Update Card 1 live instances preview
    const liveInstEl = document.getElementById("oopLiveInstances");
    if (liveInstEl) {
        liveInstEl.innerHTML = vehicles.slice(0, 3).map(v => 
            `• new ${v.emergency ? 'EmergencyVehicle' : 'Vehicle'}(${v.id}, "${v.type}", wait: ${v.wait}s)`
        ).join("<br>") + (vehicles.length > 3 ? `<br>• ... and ${vehicles.length - 3} more objects` : "");
    }

    // Auto run demo once if still on initial placeholder
    const out = document.getElementById("oopDemoOutput");
    if (out && out.innerHTML.includes("Select a vehicle from the dropdown above")) {
        runOOPDemo();
    }
}

function runOOPDemo() {
    const sel = document.getElementById("oopVehicleSelect");
    const out = document.getElementById("oopDemoOutput");
    if (!sel || !out) return;

    if (vehicles.length === 0 || !sel.value) {
        out.innerHTML = `
            <div style="color:var(--amber);font-weight:700">⚠ DATABASE IS EMPTY</div>
            <div style="color:var(--text-dim);margin-top:4px">
                No vehicle selected. Use the <b>Vehicle Control</b> form above to <b>+ INSERT</b> a vehicle (e.g. ID 101, Car, or Ambulance), then click <b>RUN OOP DEMO</b>.
            </div>
        `;
        showToast("warning", "OOP Demo", "Database is empty. Add a vehicle first.");
        return;
    }

    const id = Number(sel.value);
    const v = vehicles.find(x => x.id === id);
    if (!v) {
        out.innerHTML = `<div style="color:var(--red);font-weight:700">✕ Selected vehicle #${id} not found in database.</div>`;
        return;
    }

    const prio = calculatePriorityScore(v);
    const isEmerg = v.emergency;
    const className = isEmerg ? "EmergencyVehicle" : "Vehicle";

    let ruleApplied = "";
    let classification = "";
    let oopConcept = "";

    if (isEmerg) {
        classification = `<b style="color:var(--red)">EMERGENCY PRIORITY OVERRIDE (CRITICAL)</b>`;
        ruleApplied = `<b>Emergency Preemption Rule:</b> System evaluates an emergency vehicle instance (<code>${v.type} #${v.id}</code>). The <code>calculatePriorityScore()</code> method awards an additional <b>+50 Emergency Bonus</b> (Total Priority: ${prio}). Opposing normal lanes are frozen safely, and the signal controller initiates priority clearance for the vehicle's corridor.`;
        oopConcept = `<b>OOP Concepts Demonstrated:</b><br>
• <b>Inheritance & Specialization:</b> <code>EmergencyVehicle</code> specializes base <code>Vehicle</code> with flashing beacons, higher velocity limit (3.8 px/f), and emergency preemption.<br>
• <b>Polymorphism (Dynamic Method Dispatch):</b> Evaluating priority calls the specialized emergency priority calculation, returning a preemption score higher than any commuter.<br>
• <b>Encapsulation:</b> State changes (<code>status = "PRIORITY ACTIVE"</code>) are managed internally by vehicle methods, preserving simulation invariants.`;
    } else {
        classification = `<b style="color:var(--green)">STANDARD COMMUTER (FIFO QUEUE)</b>`;
        ruleApplied = `<b>Normal Traffic Handling Rule:</b> Vehicle <code>${v.type} #${v.id}</code> is treated as a standard commuter. Priority is computed based on waiting latency (${v.wait}s) and fleet density (Total Priority: ${prio}). The vehicle waits safely behind the stop line in FIFO arrival order and traverses the intersection when its corridor phase turns Green.`;
        oopConcept = `<b>OOP Concepts Demonstrated:</b><br>
• <b>Class & Object Instantiation:</b> Concrete instance of base class <code>Vehicle</code> with encapsulated coordinates, velocity, and lane references.<br>
• <b>Abstraction:</b> The operator interacts with this vehicle through clear parameters (ID, Type, Latency) without handling low-level pixel vectors or collision geometry.<br>
• <b>Encapsulation:</b> Vehicle properties (speed, wait, status) are modified safely through dedicated controller routines.`;
    }

    out.innerHTML = `
        <div style="display:flex;align-items:center;justify-content:space-between;border-bottom:1px solid #1a2244;padding-bottom:8px;margin-bottom:10px">
            <div>
                <span style="color:var(--text-muted);font-size:10px">SELECTED OBJECT:</span>
                <span style="font-size:14px;font-weight:800;color:var(--cyan);margin-left:6px">class ${className} (ID #${v.id})</span>
            </div>
            <div>
                <span style="color:var(--text-muted);font-size:10px">PRIORITY SCORE:</span>
                <span style="font-size:14px;font-weight:800;color:${isEmerg ? 'var(--red)' : 'var(--cyan)'};margin-left:6px">${prio}</span>
            </div>
        </div>

        <div style="display:grid;grid-template-columns:repeat(auto-fit, minmax(160px, 1fr));gap:8px;margin-bottom:12px;background:#090d22;padding:10px;border-radius:6px;border:1px solid #161c36">
            <div><span style="color:var(--text-muted)">TYPE:</span> <b>${v.type}</b></div>
            <div><span style="color:var(--text-muted)">WAIT TIME:</span> <b>${v.wait}s</b></div>
            <div><span style="color:var(--text-muted)">CORRIDOR:</span> <b>${v.lane} (${v.heading || 'EAST'}bound)</b></div>
            <div><span style="color:var(--text-muted)">STATUS:</span> <b>${v.status}</b></div>
            <div><span style="color:var(--text-muted)">CLASSIFICATION:</span> ${classification}</div>
        </div>

        <div style="background:#070a1a;border-left:3px solid ${isEmerg ? 'var(--red)' : 'var(--cyan)'};padding:10px;border-radius:4px;margin-bottom:10px">
            ${ruleApplied}
        </div>

        <div style="background:#060814;border:1px solid #141830;padding:10px;border-radius:4px;color:#a5accf;font-size:10px;line-height:1.5">
            ${oopConcept}
        </div>
    `;

    // Highlight selected vehicle in dashboard
    selectedId = v.id;
    updateCOA();
    displayVehicles();
    showToast(isEmerg ? "emergency" : "success", "OOP Demo Evaluated", `Evaluated ${className} #${v.id} (${v.type}) successfully.`);
}
'''

    # Inject js_oop_code before </script>
    script_end = content.rfind("</script>")
    if script_end == -1:
        print("ERROR: </script> not found")
        sys.exit(1)
    
    content = content[:script_end] + "\n" + js_oop_code + "\n" + content[script_end:]

    # 6. Hook refreshOOPVehicleList into displayVehicles, initSimulation
    content = content.replace(
        "renderAllDataStructures();\n    updateDashboardStats();\n    updateCOA();",
        "renderAllDataStructures();\n    updateDashboardStats();\n    updateCOA();\n    if (typeof refreshOOPVehicleList === 'function') refreshOOPVehicleList();"
    )

    with open(out_path, "w", encoding="utf-8") as f:
        f.write(content)

    print(f"SUCCESS: Generated {out_path} ({len(content)} characters).")

if __name__ == "__main__":
    apply()

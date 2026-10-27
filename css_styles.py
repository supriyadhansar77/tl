# css_styles.py
# Additional CSS styles for Review 2 Subject Modules

CSS_REVIEW2 = """
/* ==========================================================================
   REVIEW 2: ACADEMIC SUBJECT MODULES STYLING
   ========================================================================== */

/* ---------- SUBJECT NAVIGATION BAR ---------- */
.subjectNavBar {
    display: flex;
    align-items: center;
    gap: 8px;
    padding: 10px 0;
    margin-bottom: 20px;
    overflow-x: auto;
    border-bottom: 1px solid var(--border);
    position: sticky;
    top: 74px;
    background: rgba(7, 9, 20, 0.95);
    backdrop-filter: blur(12px);
    z-index: 90;
}
.subjectNavBar::-webkit-scrollbar { height: 4px; }
.subjectNavBar::-webkit-scrollbar-thumb { background: rgba(155, 124, 255, 0.3); border-radius: 2px; }

.navTab {
    display: inline-flex;
    align-items: center;
    gap: 8px;
    padding: 9px 15px;
    background: #0d1024;
    border: 1px solid var(--border);
    border-radius: 8px;
    color: var(--text-dim);
    font-family: var(--font-sans);
    font-size: 11px;
    font-weight: 600;
    cursor: pointer;
    white-space: nowrap;
    transition: all 0.22s ease;
    user-select: none;
}
.navTab:hover {
    color: var(--text);
    border-color: var(--cyan);
    background: rgba(69, 230, 255, 0.08);
    transform: translateY(-1px);
}
.navTab.active {
    color: #fff;
    border-color: var(--cyan);
    background: linear-gradient(135deg, rgba(155, 124, 255, 0.25), rgba(69, 230, 255, 0.2));
    box-shadow: 0 0 16px var(--cyan-glow);
}
.tabBadge {
    display: inline-block;
    padding: 2px 6px;
    font-size: 9px;
    font-weight: 700;
    font-family: var(--font-mono);
    border-radius: 4px;
    background: rgba(155, 124, 255, 0.2);
    color: var(--purple);
    border: 1px solid rgba(155, 124, 255, 0.3);
}
.navTab.active .tabBadge {
    background: var(--cyan);
    color: #060814;
    border-color: var(--cyan);
}

/* ---------- TAB CONTENT PANELS ---------- */
.subjectTabContent {
    display: none;
    animation: tabFadeIn 0.25s ease-out;
}
.subjectTabContent.active {
    display: block;
}
.subjectTabContent.showInAll {
    display: block !important;
    margin-bottom: 35px;
}
@keyframes tabFadeIn {
    from { opacity: 0; transform: translateY(6px); }
    to { opacity: 1; transform: translateY(0); }
}

/* ---------- SUBJECT SECTION HEADERS ---------- */
.subjectHeader {
    display: flex;
    align-items: center;
    justify-content: space-between;
    padding: 16px 20px;
    background: linear-gradient(90deg, rgba(15, 18, 38, 0.95), rgba(11, 14, 28, 0.85));
    border: 1px solid var(--border);
    border-left: 4px solid var(--cyan);
    border-radius: 10px;
    margin-bottom: 18px;
    box-shadow: 0 4px 20px rgba(0, 0, 0, 0.35);
}
.subjectHeaderTitle {
    display: flex;
    align-items: center;
    gap: 12px;
}
.subjectHeaderTitle h2 {
    font-size: 16px;
    font-weight: 800;
    letter-spacing: 1px;
    background: linear-gradient(90deg, #fff, var(--cyan));
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
}
.subjectHeaderTitle p {
    font-size: 11px;
    color: var(--text-dim);
    margin-top: 3px;
}
.subjectHeaderRight {
    display: flex;
    align-items: center;
    gap: 10px;
}
.liveSyncBadge {
    display: inline-flex;
    align-items: center;
    gap: 6px;
    padding: 4px 10px;
    border-radius: 20px;
    background: rgba(53, 233, 140, 0.08);
    border: 1px solid rgba(53, 233, 140, 0.3);
    color: var(--green);
    font-size: 10px;
    font-family: var(--font-mono);
    font-weight: 600;
}

/* ---------- SECTION SUB-LAYOUTS ---------- */
.taskGrid2 {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 16px;
    margin-bottom: 16px;
}
.taskGrid3 {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    gap: 16px;
    margin-bottom: 16px;
}
@media (max-width: 960px) {
    .taskGrid2, .taskGrid3 { grid-template-columns: 1fr; }
}

/* ---------- FORM CONTROLS & ACTION BUTTONS ---------- */
.opControlBox {
    background: rgba(8, 10, 22, 0.65);
    border: 1px solid rgba(155, 124, 255, 0.12);
    border-radius: 8px;
    padding: 12px;
    margin-bottom: 14px;
}
.opFormRow {
    display: flex;
    flex-wrap: wrap;
    align-items: center;
    gap: 10px;
    margin-bottom: 8px;
}
.opInput {
    height: 32px;
    padding: 0 10px;
    background: #060814;
    border: 1px solid #1f2544;
    border-radius: 6px;
    color: var(--text);
    font-family: var(--font-mono);
    font-size: 11px;
}
.opInput:focus {
    border-color: var(--cyan);
    outline: none;
    box-shadow: 0 0 10px var(--cyan-glow);
}
.opSelect {
    height: 32px;
    padding: 0 10px;
    background: #060814;
    border: 1px solid #1f2544;
    border-radius: 6px;
    color: var(--text);
    font-family: var(--font-sans);
    font-size: 11px;
    cursor: pointer;
}
.opBtn {
    height: 32px;
    padding: 0 14px;
    border-radius: 6px;
    font-family: var(--font-sans);
    font-size: 11px;
    font-weight: 700;
    cursor: pointer;
    border: none;
    transition: all 0.2s;
    display: inline-flex;
    align-items: center;
    gap: 6px;
}
.opBtn:hover { transform: translateY(-1px); filter: brightness(1.1); }
.opBtn.primary { background: linear-gradient(135deg, var(--cyan), #2094f3); color: #050b18; }
.opBtn.success { background: linear-gradient(135deg, var(--green), #1b9b5a); color: #05140b; }
.opBtn.warning { background: linear-gradient(135deg, var(--amber), #e08b1f); color: #160c02; }
.opBtn.danger { background: linear-gradient(135deg, var(--red), #c8283a); color: #fff; }
.opBtn.secondary { background: #1a1e38; color: #b8c2e6; border: 1px solid #2a325a; }

/* ---------- TRACE & STEP TABLES ---------- */
.traceTableWrap {
    max-height: 260px;
    overflow-y: auto;
    border: 1px solid #1b203c;
    border-radius: 6px;
    background: #060814;
}
.traceTable {
    width: 100%;
    border-collapse: collapse;
    font-family: var(--font-mono);
    font-size: 10.5px;
    text-align: left;
}
.traceTable th {
    position: sticky;
    top: 0;
    background: #0e122b;
    color: var(--cyan);
    padding: 8px 10px;
    border-bottom: 1px solid #232a50;
    font-weight: 700;
    letter-spacing: 0.5px;
}
.traceTable td {
    padding: 6px 10px;
    border-bottom: 1px solid #12162a;
    color: #cfd6f5;
}
.traceTable tr:hover td {
    background: rgba(69, 230, 255, 0.05);
}
.traceTable tr.highlightRow td {
    background: rgba(255, 180, 84, 0.15);
    color: var(--amber);
    font-weight: 700;
}

/* ---------- ARITHMETIC BIT & REGISTER BOXES ---------- */
.bitGrid {
    display: flex;
    gap: 3px;
    margin: 8px 0;
    justify-content: center;
}
.bitCell {
    width: 28px;
    height: 34px;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    background: #080a18;
    border: 1px solid #1a203a;
    border-radius: 4px;
    font-family: var(--font-mono);
    font-size: 12px;
    font-weight: 700;
}
.bitCell.carry { border-color: var(--amber); color: var(--amber); }
.bitCell.one { color: var(--cyan); background: rgba(69, 230, 255, 0.08); border-color: rgba(69, 230, 255, 0.3); }
.bitCell.zero { color: var(--text-muted); }
.bitIdx { font-size: 7px; color: var(--text-muted); }

/* ---------- PIPELINE VISUALIZER ---------- */
.pipeGridWrap {
    overflow-x: auto;
    padding: 10px 0;
}
.pipeGrid {
    display: grid;
    gap: 6px;
    min-width: 650px;
}
.pipeRow {
    display: flex;
    align-items: center;
    gap: 6px;
}
.pipeLabel {
    width: 140px;
    font-family: var(--font-mono);
    font-size: 10px;
    color: var(--text);
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
}
.pipeCell {
    width: 54px;
    height: 32px;
    display: flex;
    align-items: center;
    justify-content: center;
    font-family: var(--font-mono);
    font-size: 10px;
    font-weight: 700;
    border-radius: 4px;
    border: 1px solid transparent;
}
.pipeCell.if { background: rgba(56, 139, 253, 0.2); color: var(--blue); border-color: rgba(56, 139, 253, 0.4); }
.pipeCell.id { background: rgba(155, 124, 255, 0.2); color: var(--purple); border-color: rgba(155, 124, 255, 0.4); }
.pipeCell.ex { background: rgba(255, 180, 84, 0.2); color: var(--amber); border-color: rgba(255, 180, 84, 0.4); }
.pipeCell.mem { background: rgba(69, 230, 255, 0.2); color: var(--cyan); border-color: rgba(69, 230, 255, 0.4); }
.pipeCell.wb { background: rgba(53, 233, 140, 0.2); color: var(--green); border-color: rgba(53, 233, 140, 0.4); }
.pipeCell.stall { background: rgba(255, 77, 94, 0.2); color: var(--red); border-color: rgba(255, 77, 94, 0.5); font-style: italic; }
.pipeCell.empty { background: #070916; border-color: #121528; color: #2a3150; }

/* ---------- HASH TABLE VISUALIZER ---------- */
.hashGrid {
    display: grid;
    grid-template-columns: repeat(2, 1fr);
    gap: 8px;
}
@media (max-width: 600px) { .hashGrid { grid-template-columns: 1fr; } }
.hashBucket {
    background: #080b1a;
    border: 1px solid #1b213c;
    border-radius: 6px;
    padding: 8px 10px;
    display: flex;
    align-items: center;
    gap: 10px;
}
.hashIndexBadge {
    width: 28px;
    height: 28px;
    display: grid;
    place-items: center;
    background: #141934;
    border: 1px solid var(--border);
    border-radius: 4px;
    font-family: var(--font-mono);
    font-size: 11px;
    font-weight: 800;
    color: var(--cyan);
}
.hashChainList {
    display: flex;
    flex-wrap: wrap;
    align-items: center;
    gap: 4px;
    flex: 1;
}
.hashChainNode {
    display: inline-flex;
    align-items: center;
    gap: 4px;
    padding: 3px 7px;
    background: #121730;
    border: 1px solid rgba(155, 124, 255, 0.25);
    border-radius: 4px;
    font-family: var(--font-mono);
    font-size: 9.5px;
    color: var(--text);
}
.hashChainNode.collision {
    border-color: var(--amber);
    background: rgba(255, 180, 84, 0.12);
    color: var(--amber);
}

/* ---------- 3D GRAPHICS & MATRIX DISPLAY ---------- */
.matrixGrid {
    display: grid;
    grid-template-columns: repeat(4, 1fr);
    gap: 4px;
    max-width: 320px;
    padding: 10px;
    background: #060814;
    border: 1px solid #1a203c;
    border-radius: 6px;
    font-family: var(--font-mono);
    font-size: 10px;
    text-align: center;
}
.matrixCell {
    padding: 5px 2px;
    background: #0c1024;
    border: 1px solid #182040;
    border-radius: 3px;
    color: var(--cyan);
}

/* ---------- OOP OBJECT INSPECTOR ---------- */
.oopObjectTree {
    background: #060814;
    border: 1px solid #18203e;
    border-radius: 8px;
    padding: 14px;
    font-family: var(--font-mono);
    font-size: 11px;
}
.oopProp {
    display: flex;
    justify-content: space-between;
    padding: 4px 0;
    border-bottom: 1px solid #101428;
}
.oopPropKey { color: var(--purple); }
.oopPropVal { color: var(--cyan); font-weight: 700; }
.oopMethod {
    display: inline-block;
    padding: 2px 8px;
    background: rgba(53, 233, 140, 0.1);
    border: 1px solid rgba(53, 233, 140, 0.3);
    border-radius: 4px;
    color: var(--green);
    font-size: 10px;
    margin: 3px;
}
"""

# html_sections.py
# HTML sections for Review 2 Subject Modules

NAV_BAR_HTML = r"""
    <!-- ============================================================
         REVIEW 2: ACADEMIC SUBJECT NAVIGATION BAR
         Direct one-click access to each curriculum module and full view
         ============================================================ -->
    <nav class="subjectNavBar" id="subjectNavBar">
        <button class="navTab active" data-tab="tab-overview" onclick="switchSubjectTab('tab-overview', this)">
            <span>🚦</span>
            <span>Live Command Center</span>
            <span class="tabBadge">MAIN</span>
        </button>
        <button class="navTab" data-tab="tab-pl-task3" onclick="switchSubjectTab('tab-pl-task3', this)">
            <span>📦</span>
            <span>PL — Task 3: Stack & Queue</span>
            <span class="tabBadge">PL</span>
        </button>
        <button class="navTab" data-tab="tab-pl-task4" onclick="switchSubjectTab('tab-pl-task4', this)">
            <span>🔍</span>
            <span>PL — Task 4: Search, Sort & Hash</span>
            <span class="tabBadge">PL</span>
        </button>
        <button class="navTab" data-tab="tab-coa-task3" onclick="switchSubjectTab('tab-coa-task3', this)">
            <span>🧮</span>
            <span>COA — Task 3: Computer Arithmetic</span>
            <span class="tabBadge">COA</span>
        </button>
        <button class="navTab" data-tab="tab-coa-task4" onclick="switchSubjectTab('tab-coa-task4', this)">
            <span>⚙️</span>
            <span>COA — Task 4: Control Unit & Pipeline</span>
            <span class="tabBadge">COA</span>
        </button>
        <button class="navTab" data-tab="tab-cg-task3" onclick="switchSubjectTab('tab-cg-task3', this)">
            <span>🎨</span>
            <span>CG — Task 3: 2D & 3D Graphics</span>
            <span class="tabBadge">CG</span>
        </button>
        <button class="navTab" data-tab="tab-cg-task4" onclick="switchSubjectTab('tab-cg-task4', this)">
            <span>📐</span>
            <span>CG — Task 4: Projection & Rendering</span>
            <span class="tabBadge">CG</span>
        </button>
        <button class="navTab" data-tab="tab-psoop" onclick="switchSubjectTab('tab-psoop', this)">
            <span>🧩</span>
            <span>PSOOP / OOP Architecture</span>
            <span class="tabBadge">OOP</span>
        </button>
        <button class="navTab" data-tab="tab-all" onclick="switchSubjectTab('tab-all', this)">
            <span>📑</span>
            <span>All Modules View</span>
            <span class="tabBadge">FULL</span>
        </button>
    </nav>
"""

PL_TASK3_HTML = r"""
    <!-- ============================================================
         PROGRAMMING LAB — TASK 3: STACK AND QUEUE
         ============================================================ -->
    <div id="tab-pl-task3" class="subjectTabContent">
        <div class="subjectHeader">
            <div class="subjectHeaderTitle">
                <span style="font-size:24px">📦</span>
                <div>
                    <h2>PL — Task 3: Stack & Queue Scenario</h2>
                    <p>FIFO Waiting Queue for normal commuter clearance & LIFO Stack for priority interrupt preemption</p>
                </div>
            </div>
            <div class="subjectHeaderRight">
                <div class="liveSyncBadge">● LIVE SIMULATION CONNECTED</div>
            </div>
        </div>

        <div class="taskGrid2">
            <!-- QUEUE CARD: FIFO NORMAL VEHICLE CLEARANCE -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>FIFO QUEUE: NORMAL COMMUTER BUFFER</span>
                    </div>
                    <span class="tag">FIFO (First-In, First-Out)</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:12px;line-height:1.5">
                        Normal vehicles approaching a red signal are buffered in a strict FIFO Queue. When the traffic signal transitions to Green for their corridor, vehicles are dequeued from the <b>Front</b> and allowed to traverse the intersection.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <input type="number" id="pl3QueueId" class="opInput" style="width:110px" placeholder="Vehicle ID">
                            <button class="opBtn primary" onclick="pl3Enqueue()">+ Enqueue (Rear)</button>
                            <button class="opBtn success" onclick="pl3Dequeue()">Dequeue (Front)</button>
                            <button class="opBtn secondary" onclick="pl3PeekQueue()">Peek Front</button>
                            <button class="opBtn warning" onclick="pl3ProcessSignalGreen()">Process Green Phase</button>
                            <button class="opBtn danger" onclick="pl3ClearQueue()">Clear Queue</button>
                        </div>
                    </div>

                    <div style="font-size:9px;color:var(--text-muted);margin-bottom:6px;font-family:var(--font-mono)">QUEUE VISUAL BUFFER (HEAD TO TAIL)</div>
                    <div class="dsBox" id="pl3QueueVisual" style="min-height:55px;margin-bottom:10px">
                        <span class="dsEmpty">FRONT → EMPTY QUEUE ← REAR</span>
                    </div>

                    <div style="display:grid;grid-template-columns:repeat(3,1fr);gap:8px;font-family:var(--font-mono);font-size:10px">
                        <div style="background:#080b18;padding:8px;border-radius:4px;border:1px solid #1a203c">
                            <span style="color:var(--text-muted)">HEAD / FRONT:</span> <b id="pl3QueueFront" style="color:var(--cyan)">--</b>
                        </div>
                        <div style="background:#080b18;padding:8px;border-radius:4px;border:1px solid #1a203c">
                            <span style="color:var(--text-muted)">TAIL / REAR:</span> <b id="pl3QueueRear" style="color:var(--purple)">--</b>
                        </div>
                        <div style="background:#080b18;padding:8px;border-radius:4px;border:1px solid #1a203c">
                            <span style="color:var(--text-muted)">BUFFER COUNT:</span> <b id="pl3QueueCount" style="color:var(--green)">0</b>
                        </div>
                    </div>
                </div>
            </section>

            <!-- STACK CARD: LIFO RECENT TRAFFIC EVENTS & EMERGENCY PREEMPTION -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>LIFO STACK: EMERGENCY PREEMPTION & SYSTEM ACTIONS</span>
                    </div>
                    <span class="tag">LIFO (Last-In, First-Out)</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:12px;line-height:1.5">
                        High-priority emergency alerts and critical signal override actions are pushed to the Top of the LIFO Stack. The latest emergency interrupt immediately preempts normal traffic; resolving or rolling back operations pops from the <b>Top</b>.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <input type="number" id="pl3StackId" class="opInput" style="width:110px" placeholder="Emerg ID">
                            <button class="opBtn danger" onclick="pl3PushEmergency()">Push Emergency (Top)</button>
                            <button class="opBtn warning" onclick="pl3PopEmergency()">Pop & Preempt (Top)</button>
                            <button class="opBtn secondary" onclick="pl3PeekStack()">Peek Top</button>
                            <button class="opBtn secondary" onclick="pl3RollbackEvent()">Undo / Revert Last</button>
                            <button class="opBtn danger" onclick="pl3ClearStack()">Clear Stack</button>
                        </div>
                    </div>

                    <div style="font-size:9px;color:var(--text-muted);margin-bottom:6px;font-family:var(--font-mono)">STACK MEMORY FRAME (TOP TO BASE)</div>
                    <div class="dsBox" id="pl3StackVisual" style="min-height:55px;margin-bottom:10px">
                        <span class="dsEmpty">TOP [SP] → EMPTY STACK</span>
                    </div>

                    <div style="display:grid;grid-template-columns:repeat(3,1fr);gap:8px;font-family:var(--font-mono);font-size:10px">
                        <div style="background:#080b18;padding:8px;border-radius:4px;border:1px solid #1a203c">
                            <span style="color:var(--text-muted)">STACK POINTER (SP):</span> <b id="pl3StackSP" style="color:var(--amber)">0x00</b>
                        </div>
                        <div style="background:#080b18;padding:8px;border-radius:4px;border:1px solid #1a203c">
                            <span style="color:var(--text-muted)">TOP ELEMENT:</span> <b id="pl3StackTop" style="color:var(--red)">--</b>
                        </div>
                        <div style="background:#080b18;padding:8px;border-radius:4px;border:1px solid #1a203c">
                            <span style="color:var(--text-muted)">TOTAL DEPTH:</span> <b id="pl3StackCount" style="color:var(--purple)">0</b>
                        </div>
                    </div>
                </div>
            </section>
        </div>

        <!-- BOUNDARY CASES & SCENARIO TESTING SUITE -->
        <section class="card">
            <div class="cardHeader">
                <div class="cardTitle">
                    <i>✦</i>
                    <span>INTERACTIVE SCENARIO & BOUNDARY TESTING BENCH</span>
                </div>
                <span class="tag">EDGE CASES & STRESS TESTING</span>
            </div>
            <div class="cardBody">
                <div style="display:flex;flex-wrap:wrap;gap:8px;margin-bottom:12px">
                    <button class="opBtn secondary" onclick="pl3TestBoundary('EMPTY_QUEUE')">Test 1: Empty Queue Dequeue (Underflow)</button>
                    <button class="opBtn secondary" onclick="pl3TestBoundary('EMPTY_STACK')">Test 2: Empty Stack Pop (Underflow)</button>
                    <button class="opBtn secondary" onclick="pl3TestBoundary('SINGLE_VEHICLE')">Test 3: Single Vehicle Passage</button>
                    <button class="opBtn secondary" onclick="pl3TestBoundary('MULTI_FIFO')">Test 4: Multi-Vehicle FIFO Sequence</button>
                    <button class="opBtn secondary" onclick="pl3TestBoundary('MULTI_EMERGENCY')">Test 5: Repeated Emergency Interruption</button>
                </div>
                <div id="pl3TestLog" style="padding:10px;background:#050713;border:1px solid #191e38;border-radius:6px;font-family:var(--font-mono);font-size:10.5px;color:var(--cyan);min-height:38px">
                    System ready. Select any test case to verify boundary condition handling and live state feedback.
                </div>
            </div>
        </section>
    </div>
"""

PL_TASK4_HTML = r"""
    <!-- ============================================================
         PROGRAMMING LAB — TASK 4: SEARCHING, SORTING AND HASHING
         ============================================================ -->
    <div id="tab-pl-task4" class="subjectTabContent">
        <div class="subjectHeader">
            <div class="subjectHeaderTitle">
                <span style="font-size:24px">🔍</span>
                <div>
                    <h2>PL — Task 4: Searching, Sorting & Hashing</h2>
                    <p>Real-time vehicle lookup, multi-key database sorting, and collision-resolved hash table indexing</p>
                </div>
            </div>
            <div class="subjectHeaderRight">
                <div class="liveSyncBadge">● SHARED DATABASE SYNCED</div>
            </div>
        </div>

        <!-- TOP ROW: SEARCHING & SORTING CARDS -->
        <div class="taskGrid2">
            
            <!-- SEARCHING CARD -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>VEHICLE SEARCH ENGINE</span>
                    </div>
                    <span class="tag">LINEAR & BINARY SEARCH</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:12px">
                        Search vehicle records by ID, vehicle classification, or emergency override status. Linear Search inspects elements sequentially $O(n)$; Binary Search operates on a sorted index in $O(\log n)$.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <input type="text" id="pl4SearchQuery" class="opInput" style="width:120px" placeholder="ID or Type (e.g. 102)">
                            <select id="pl4SearchAlgo" class="opSelect">
                                <option value="LINEAR">Linear Search (O(n))</option>
                                <option value="BINARY">Binary Search (O(log n))</option>
                            </select>
                            <select id="pl4SearchField" class="opSelect">
                                <option value="ID">By Vehicle ID</option>
                                <option value="TYPE">By Vehicle Type</option>
                                <option value="EMERG">By Emergency Status</option>
                            </select>
                            <button class="opBtn primary" onclick="pl4ExecuteSearch()">Execute Search</button>
                        </div>
                    </div>

                    <div id="pl4SearchResultBox" style="padding:10px;background:#050713;border:1px solid #1a203c;border-radius:6px;font-family:var(--font-mono);font-size:10.5px;min-height:65px">
                        <span style="color:var(--text-muted)">Awaiting search query. Enter an ID and select an algorithm.</span>
                    </div>

                    <div style="margin-top:10px;font-size:9px;color:var(--text-muted);font-family:var(--font-mono)">
                        COMPARISON TRACE: <span id="pl4SearchSteps" style="color:var(--cyan)">0 comparisons</span> | TIME: <span id="pl4SearchTime" style="color:var(--amber)">0.00 ms</span>
                    </div>
                </div>
            </section>

            <!-- SORTING CARD -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>DATABASE MULTI-KEY SORT ENGINE</span>
                    </div>
                    <span class="tag">BUBBLE, SELECTION & QUICK SORT</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:12px">
                        Sort actual registered vehicles by primary key (ID), dynamic latency (Wait Time), or Computed Priority. Tracks comparisons, element exchanges, and execution duration.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <select id="pl4SortKey" class="opSelect">
                                <option value="id">Key: Vehicle ID</option>
                                <option value="wait">Key: Waiting Time</option>
                                <option value="priority">Key: Priority Score</option>
                                <option value="type">Key: Vehicle Type</option>
                            </select>
                            <select id="pl4SortAlgo" class="opSelect">
                                <option value="BUBBLE">Bubble Sort (O(n²))</option>
                                <option value="SELECTION">Selection Sort (O(n²))</option>
                                <option value="QUICK">Quick Sort (O(n log n))</option>
                            </select>
                            <button class="opBtn warning" onclick="pl4ExecuteSort()">Sort Database</button>
                            <button class="opBtn secondary" onclick="pl4ResetSort()">Reset Order</button>
                        </div>
                    </div>

                    <div style="display:grid;grid-template-columns:repeat(4,1fr);gap:6px;font-family:var(--font-mono);font-size:9.5px;margin-bottom:8px">
                        <div style="background:#080b18;padding:6px;border-radius:4px;border:1px solid #1a203c">
                            ALGO: <b id="pl4SortAlgoLabel" style="color:var(--cyan)">BUBBLE</b>
                        </div>
                        <div style="background:#080b18;padding:6px;border-radius:4px;border:1px solid #1a203c">
                            COMPS: <b id="pl4SortComps" style="color:var(--amber)">0</b>
                        </div>
                        <div style="background:#080b18;padding:6px;border-radius:4px;border:1px solid #1a203c">
                            SWAPS: <b id="pl4SortSwaps" style="color:var(--purple)">0</b>
                        </div>
                        <div style="background:#080b18;padding:6px;border-radius:4px;border:1px solid #1a203c">
                            TIME: <b id="pl4SortTime" style="color:var(--green)">0.00 ms</b>
                        </div>
                    </div>

                    <div class="traceTableWrap" style="max-height:130px">
                        <table class="traceTable" id="pl4SortedTable">
                            <thead>
                                <tr><th>ID</th><th>TYPE</th><th>WAIT</th><th>PRIORITY</th><th>STATUS</th></tr>
                            </thead>
                            <tbody><!-- Populated dynamically --></tbody>
                        </table>
                    </div>
                </div>
            </section>
        </div>

        <!-- HASHING & PERFORMANCE BENCHMARK ROW -->
        <div class="taskGrid2">
            
            <!-- HASH TABLE VISUALIZER -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>HASH TABLE INDEXING (DIVISION METHOD)</span>
                    </div>
                    <span class="tag">SEPARATE CHAINING & LINEAR PROBING</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Hash Function: <code>h(k) = k mod 10</code>. Collisions occur when multiple vehicle IDs produce the same remainder. Resolved via <b>Separate Chaining</b> (linked list buckets) and <b>Linear Probing</b> open addressing.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <button class="opBtn primary" onclick="pl4SyncHashTable()">Rehash Live Database</button>
                            <button class="opBtn warning" onclick="pl4TriggerCollisionDemo()">Demo Collision (e.g. 101 & 111)</button>
                            <span style="font-size:10px;color:var(--text-muted);font-family:var(--font-mono)">
                                LOAD FACTOR: <b id="pl4HashLF" style="color:var(--cyan)">0.0</b> | COLLISIONS: <b id="pl4HashColls" style="color:var(--amber)">0</b>
                            </span>
                        </div>
                    </div>

                    <div class="hashGrid" id="pl4HashBucketGrid">
                        <!-- 10 Buckets populated dynamically -->
                    </div>
                </div>
            </section>

            <!-- PERFORMANCE & DATASET BENCHMARK -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>ALGORITHM BENCHMARK SUITE</span>
                    </div>
                    <span class="tag">EMPIRICAL COMPLEXITY ANALYSIS</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Evaluate sorting and searching behavior on different traffic fleet volumes (Current Fleet, 10, 25, 50 vehicles). Observe how comparisons and execution time scale with $N$.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <button class="opBtn secondary" onclick="pl4RunBenchmark('CURRENT')">Bench: Live Fleet</button>
                            <button class="opBtn secondary" onclick="pl4RunBenchmark(10)">Bench: N = 10</button>
                            <button class="opBtn secondary" onclick="pl4RunBenchmark(25)">Bench: N = 25</button>
                            <button class="opBtn secondary" onclick="pl4RunBenchmark(50)">Bench: N = 50</button>
                        </div>
                    </div>

                    <div class="traceTableWrap" style="max-height:190px">
                        <table class="traceTable">
                            <thead>
                                <tr><th>ALGORITHM</th><th>RECORDS</th><th>COMPARISONS</th><th>SWAPS/PROBES</th><th>TIME</th><th>COMPLEXITY</th></tr>
                            </thead>
                            <tbody id="pl4BenchmarkTbody">
                                <tr><td colspan="6" style="text-align:center;color:var(--text-muted)">Select a fleet benchmark to measure empirical performance.</td></tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </section>

        </div>
    </div>
"""

COA_TASK3_HTML = r"""
    <!-- ============================================================
         COA — TASK 3: COMPUTER ARITHMETIC
         ============================================================ -->
    <div id="tab-coa-task3" class="subjectTabContent">
        <div class="subjectHeader">
            <div class="subjectHeaderTitle">
                <span style="font-size:24px">🧮</span>
                <div>
                    <h2>COA — Task 3: Computer Arithmetic</h2>
                    <p>Processor-level binary arithmetic, add-and-shift multiplication, restoring division, and traffic timing synthesis</p>
                </div>
            </div>
            <div class="subjectHeaderRight">
                <button class="opBtn primary" onclick="coa3LoadLiveOperands()">⚡ Load From Live Traffic State</button>
            </div>
        </div>

        <div class="taskGrid2">
            <!-- 1. BINARY ADDITION -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>1. BINARY ADDITION: FULL ADDER LOGIC</span>
                    </div>
                    <span class="tag">8-BIT RIPPLE CARRY ADDER</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Computes green time extension: $\text{Base Green} + \text{Wait Time Bonus}$. Simulates an 8-bit hardware ripple-carry full adder generating Sum ($S_i = A_i \oplus B_i \oplus C_i$) and Carry Out ($C_{i+1}$).
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">A:</span>
                            <input type="number" id="coa3AddA" class="opInput" style="width:65px" value="30" min="0" max="255">
                            <span style="font-size:10px;color:var(--text-dim)">+ B:</span>
                            <input type="number" id="coa3AddB" class="opInput" style="width:65px" value="14" min="0" max="255">
                            <button class="opBtn success" onclick="coa3ComputeAddition()">Compute Addition</button>
                        </div>
                    </div>

                    <div id="coa3AddBitsDisplay" style="margin-bottom:8px"></div>

                    <div class="traceTableWrap" style="max-height:140px">
                        <table class="traceTable">
                            <thead>
                                <tr><th>BIT</th><th>A[i]</th><th>B[i]</th><th>C_IN</th><th>SUM[i]</th><th>C_OUT</th><th>LOGIC STEP</th></tr>
                            </thead>
                            <tbody id="coa3AddTbody"></tbody>
                        </table>
                    </div>
                </div>
            </section>

            <!-- 2. BINARY SUBTRACTION -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>2. BINARY SUBTRACTION: 2'S COMPLEMENT</span>
                    </div>
                    <span class="tag">INVERT & ADD 1 (A + (~B + 1))</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Computes elapsed signal clearance: $\text{Green Duration} - \text{Elapsed Seconds}$. The ALU converts subtrahend $B$ into 2's complement representation ($\sim B + 1$) and performs binary addition with $A$.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">A:</span>
                            <input type="number" id="coa3SubA" class="opInput" style="width:65px" value="30" min="0" max="255">
                            <span style="font-size:10px;color:var(--text-dim)">- B:</span>
                            <input type="number" id="coa3SubB" class="opInput" style="width:65px" value="12" min="0" max="255">
                            <button class="opBtn warning" onclick="coa3ComputeSubtraction()">Compute Subtraction</button>
                        </div>
                    </div>

                    <div id="coa3SubTraceBox" style="padding:10px;background:#050713;border:1px solid #1a203c;border-radius:6px;font-family:var(--font-mono);font-size:10.5px;min-height:90px;line-height:1.6">
                        <!-- Populated dynamically -->
                    </div>
                </div>
            </section>
        </div>

        <div class="taskGrid2">
            <!-- 3. MULTIPLICATION -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>3. MULTIPLICATION: ADD-AND-SHIFT ALGORITHM</span>
                    </div>
                    <span class="tag">MULTIPLIER (COUNT × WEIGHT)</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Calculates lane congestion score: $\text{Vehicle Count } (M) \times \text{Weight Factor } (Q)$. Demonstrates hardware registers $A$ (Accumulator), $Q$ (Multiplier), $M$ (Multiplicand) over successive clock cycles.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">M (Density):</span>
                            <input type="number" id="coa3MulM" class="opInput" style="width:65px" value="5" min="0" max="31">
                            <span style="font-size:10px;color:var(--text-dim)">× Q (Weight):</span>
                            <input type="number" id="coa3MulQ" class="opInput" style="width:65px" value="3" min="0" max="31">
                            <button class="opBtn primary" onclick="coa3ComputeMultiplication()">Run Multiplier</button>
                        </div>
                    </div>

                    <div class="traceTableWrap" style="max-height:160px">
                        <table class="traceTable">
                            <thead>
                                <tr><th>CYCLE</th><th>OPERATION</th><th>A (ACCUMULATOR)</th><th>Q (MULTIPLIER)</th><th>M (MULTIPLICAND)</th></tr>
                            </thead>
                            <tbody id="coa3MulTbody"></tbody>
                        </table>
                    </div>
                </div>
            </section>

            <!-- 4. RESTORING DIVISION -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>4. DIVISION: RESTORING DIVISION ALGORITHM</span>
                    </div>
                    <span class="tag">QUOTIENT / REMAINDER SYNTHESIS</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Calculates per-vehicle mean headway: $\text{Total Latency } (Q) \div \text{Vehicle Count } (M)$. Simulates register shift left, subtraction, sign restoration, and hardware zero-division protection.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">Dividend (Wait):</span>
                            <input type="number" id="coa3DivQ" class="opInput" style="width:65px" value="28" min="0" max="255">
                            <span style="font-size:10px;color:var(--text-dim)">÷ Divisor (Count):</span>
                            <input type="number" id="coa3DivM" class="opInput" style="width:65px" value="4" min="0" max="255">
                            <button class="opBtn danger" onclick="coa3ComputeDivision()">Run Restoring Division</button>
                        </div>
                    </div>

                    <div class="traceTableWrap" style="max-height:160px">
                        <table class="traceTable">
                            <thead>
                                <tr><th>CYCLE</th><th>OPERATION</th><th>A (REMAINDER REG)</th><th>Q (QUOTIENT REG)</th><th>M (DIVISOR)</th></tr>
                            </thead>
                            <tbody id="coa3DivTbody"></tbody>
                        </table>
                    </div>
                </div>
            </section>
        </div>
    </div>
"""

COA_TASK4_HTML = r"""
    <!-- ============================================================
         COA — TASK 4: CONTROL SIGNALS AND INSTRUCTION PIPELINING
         ============================================================ -->
    <div id="tab-coa-task4" class="subjectTabContent">
        <div class="subjectHeader">
            <div class="subjectHeaderTitle">
                <span style="font-size:24px">⚙️</span>
                <div>
                    <h2>COA — Task 4: Control Unit & Instruction Pipeline</h2>
                    <p>Control signal coordination, Hardwired vs Microprogrammed execution, and 5-stage instruction pipelining</p>
                </div>
            </div>
            <div class="subjectHeaderRight">
                <span class="tag" style="background:rgba(155,124,255,0.15);color:var(--purple);border:1px solid rgba(155,124,255,0.3)">EDUCATIONAL PROCESSOR ARCHITECTURE</span>
            </div>
        </div>

        <!-- ROW 1: CONTROL SIGNALS & CONTROL UNIT ORGANIZATION -->
        <div class="taskGrid2">
            
            <!-- CONTROL SIGNALS CARD -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>CONTROL SIGNALS COORDINATION MATRIX</span>
                    </div>
                    <span class="tag">PROCESSOR CONTROL BUS</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Control signals command ALU operation, register writeback, and data memory read/write for each executed traffic instruction. Select an instruction to inspect its active signal lines.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <select id="coa4InstSelect" class="opSelect" onchange="coa4UpdateControlSignals()">
                                <option value="0">Inst 1: LOAD R1, [EMERG_FLAG] (MemRead)</option>
                                <option value="1">Inst 2: CMP R1, 1 (ALU Compare)</option>
                                <option value="2">Inst 3: BEQ EMERGENCY_ISR (Branch)</option>
                                <option value="3">Inst 4: ADD R2, R2, 20 (ALU Add Bonus)</option>
                                <option value="4">Inst 5: STORE [SIGNAL_REG], R2 (MemWrite)</option>
                            </select>
                        </div>
                    </div>

                    <div class="traceTableWrap" style="max-height:180px">
                        <table class="traceTable">
                            <thead>
                                <tr><th>CONTROL SIGNAL</th><th>LINE STATE</th><th>FUNCTION / HARDWARE ROLE</th></tr>
                            </thead>
                            <tbody id="coa4SignalsTbody"></tbody>
                        </table>
                    </div>
                </div>
            </section>

            <!-- HARDWIRED VS MICROPROGRAMMED -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>CONTROL UNIT ORGANIZATION</span>
                    </div>
                    <span class="tag">HARDWIRED VS MICROPROGRAMMED</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Educational architectural comparison between combinational hardwired decoders (RISC) and micro-ROM memory sequencer lookups (CISC).
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <button class="opBtn primary" id="btnHardwired" onclick="coa4SetControlMode('HARDWIRED')">Hardwired Control</button>
                            <button class="opBtn secondary" id="btnMicroprog" onclick="coa4SetControlMode('MICROPROGRAMMED')">Microprogrammed Control</button>
                        </div>
                    </div>

                    <div id="coa4ControlOrgContent" style="padding:12px;background:#050713;border:1px solid #1a203c;border-radius:6px;font-family:var(--font-mono);font-size:10.5px;line-height:1.6">
                        <!-- Populated dynamically -->
                    </div>
                </div>
            </section>

        </div>

        <!-- ROW 2: 5-STAGE INSTRUCTION PIPELINE SIMULATOR -->
        <section class="card">
            <div class="cardHeader">
                <div class="cardTitle">
                    <i>✦</i>
                    <span>5-STAGE INSTRUCTION PIPELINE (IF, ID, EX, MEM, WB)</span>
                </div>
                <span class="tag">CYCLE-ACCURATE EDUCATIONAL PIPELINE</span>
            </div>
            <div class="cardBody">
                <p style="font-size:10px;color:var(--text-dim);margin-bottom:12px">
                    Instructions progress through 5 clock stages: <b>IF</b> (Instruction Fetch), <b>ID</b> (Decode & Reg Read), <b>EX</b> (Execute ALU), <b>MEM</b> (Memory Access), and <b>WB</b> (Write Back). Demonstrates data hazard stalls and hardware operand forwarding bypass paths.
                </p>

                <div class="opControlBox">
                    <div class="opFormRow">
                        <button class="opBtn primary" onclick="coa4StepPipeCycle()">Step Next Cycle (+1)</button>
                        <button class="opBtn success" onclick="coa4RunPipeAuto()">Auto-Run Pipeline</button>
                        <button class="opBtn secondary" onclick="coa4ResetPipeline()">Reset Pipeline</button>
                        <select id="coa4HazardMode" class="opSelect" onchange="coa4ResetPipeline()">
                            <option value="FORWARDING">Mode: With Operand Forwarding (Bypass Active)</option>
                            <option value="STALLS">Mode: Without Forwarding (Pipeline Bubbles / Stalls)</option>
                        </select>
                        <span style="font-size:10px;color:var(--text-muted);font-family:var(--font-mono)">
                            CURRENT CLOCK CYCLE: <b id="coa4CycleNum" style="color:var(--cyan)">1</b>
                        </span>
                    </div>
                </div>

                <div class="pipeGridWrap">
                    <div class="pipeGrid" id="coa4PipelineGrid">
                        <!-- Pipeline rows populated dynamically -->
                    </div>
                </div>

                <div id="coa4HazardExplanation" style="margin-top:10px;padding:8px 12px;background:#060814;border:1px solid #161b34;border-radius:6px;font-family:var(--font-mono);font-size:10px;color:var(--amber)">
                    Pipeline status: Instruction 1 in IF stage. Ready to advance cycles.
                </div>
            </div>
        </section>
    </div>
"""

CG_TASK3_HTML = r"""
    <!-- ============================================================
         COMPUTER GRAPHICS — TASK 3: 2D AND 3D OBJECTS
         ============================================================ -->
    <div id="tab-cg-task3" class="subjectTabContent">
        <div class="subjectHeader">
            <div class="subjectHeaderTitle">
                <span style="font-size:24px">🎨</span>
                <div>
                    <h2>CG — Task 3: 2D & 3D Graphics</h2>
                    <p>Real-world traffic junction modeling, 2D/3D viewport switching, object transformations, and rasterization algorithms</p>
                </div>
            </div>
            <div class="subjectHeaderRight">
                <div class="liveSyncBadge">● REAL-TIME RENDERING PIPELINE</div>
            </div>
        </div>

        <div class="taskGrid2">
            <!-- 3D JUNCTION VIEWPORT CANVAS -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>3D TRAFFIC JUNCTION VIEWPORT</span>
                    </div>
                    <span class="tag">PERSPECTIVE 3D PROJECTION CANVAS</span>
                </div>
                <div class="cardBody">
                    <div style="position:relative;background:#050711;border:1px solid #161b34;border-radius:8px;overflow:hidden;margin-bottom:10px">
                        <canvas id="cg3DTrafficCanvas" width="560" height="340" style="display:block;width:100%;height:340px"></canvas>
                        <div style="position:absolute;top:10px;left:10px;padding:4px 8px;background:rgba(6,8,20,0.85);border:1px solid rgba(69,230,255,0.3);border-radius:4px;font-family:var(--font-mono);font-size:9px;color:var(--cyan)">
                            CAMERA: AZIMUTH <span id="cg3DAzLabel">35°</span> | ELEVATION <span id="cg3DElevLabel">45°</span>
                        </div>
                    </div>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">Azimuth:</span>
                            <input type="range" id="cg3DAzimuth" min="-180" max="180" value="35" oninput="cg3UpdateCamera()">
                            <span style="font-size:10px;color:var(--text-dim)">Elevation:</span>
                            <input type="range" id="cg3DElevation" min="15" max="85" value="45" oninput="cg3UpdateCamera()">
                            <button class="opBtn secondary" onclick="cg3ResetCamera()">Reset Camera</button>
                        </div>
                    </div>
                </div>
            </section>

            <!-- OBJECT TRANSFORMATIONS & 4x4 MATRIX DISPLAY -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>OBJECT TRANSFORMATIONS & 4×4 MATRIX</span>
                    </div>
                    <span class="tag">TRANSLATION, ROTATION & SCALING</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Interactive affine transformations in 3D homogeneous coordinates: $[M] = [T(d_x, d_y, d_z)] \times [R_y(\theta)] \times [S(s_x, s_y, s_z)]$.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">Translate X:</span>
                            <input type="range" id="cg3TransX" min="-80" max="80" value="0" oninput="cg3UpdateTransform()">
                            <span style="font-size:10px;color:var(--text-dim)">Rotate Y:</span>
                            <input type="range" id="cg3RotY" min="0" max="360" value="0" oninput="cg3UpdateTransform()">
                            <span style="font-size:10px;color:var(--text-dim)">Scale:</span>
                            <input type="range" id="cg3Scale" min="5" max="20" value="10" oninput="cg3UpdateTransform()">
                        </div>
                    </div>

                    <div style="font-size:9px;color:var(--text-muted);margin-bottom:6px;font-family:var(--font-mono)">COMPOUND 4×4 HOMOGENEOUS MATRIX</div>
                    <div class="matrixGrid" id="cg3MatrixGrid">
                        <!-- 16 Cells populated dynamically -->
                    </div>

                    <div id="cg3TransformReadout" style="margin-top:10px;font-family:var(--font-mono);font-size:10px;color:var(--cyan)">
                        T(0, 0, 0) • R(0°) • S(1.00)
                    </div>
                </div>
            </section>
        </div>

        <!-- GRAPHICS PRIMITIVES & DRAWING ALGORITHMS (BRESENHAM & MIDPOINT CIRCLE) -->
        <section class="card">
            <div class="cardHeader">
                <div class="cardTitle">
                    <i>✦</i>
                    <span>RASTERIZATION ALGORITHMS: BRESENHAM LINE & MIDPOINT CIRCLE</span>
                </div>
                <span class="tag">INTEGER-ONLY DRAWING ALGORITHMS</span>
            </div>
            <div class="cardBody">
                <div class="taskGrid2">
                    <!-- BRESENHAM'S LINE -->
                    <div>
                        <div style="font-size:11px;font-weight:700;color:var(--cyan);margin-bottom:6px">Bresenham's Line Algorithm (Lane Dividers)</div>
                        <p style="font-size:9.5px;color:var(--text-dim);margin-bottom:8px">
                            Decision parameter $p_k = 2\Delta y - \Delta x$. If $p_k < 0$, $p_{k+1} = p_k + 2\Delta y$; else $p_{k+1} = p_k + 2\Delta y - 2\Delta x, y_{k+1} = y_k + 1$.
                        </p>
                        <canvas id="cg3BresenhamCanvas" width="280" height="150" style="background:#060814;border:1px solid #1a203c;border-radius:4px;display:block;margin-bottom:8px"></canvas>
                        <button class="opBtn secondary" onclick="cg3RunBresenhamDemo()">Redraw Lane Line Demo</button>
                    </div>

                    <!-- MIDPOINT CIRCLE -->
                    <div>
                        <div style="font-size:11px;font-weight:700;color:var(--green);margin-bottom:6px">Midpoint Circle Algorithm (Signal Lamps & Wheels)</div>
                        <p style="font-size:9.5px;color:var(--text-dim);margin-bottom:8px">
                            Initial parameter $p_0 = 1 - r$. Uses 8-way symmetric point generation across octants to plot perfect raster circles.
                        </p>
                        <canvas id="cg3CircleCanvas" width="280" height="150" style="background:#060814;border:1px solid #1a203c;border-radius:4px;display:block;margin-bottom:8px"></canvas>
                        <button class="opBtn secondary" onclick="cg3RunCircleDemo()">Redraw Signal Lamp Demo</button>
                    </div>
                </div>
            </div>
        </section>
    </div>
"""

CG_TASK4_HTML = r"""
    <!-- ============================================================
         COMPUTER GRAPHICS — TASK 4: PROJECTION, VIEWING, CLIPPING, TEXTURES, LIGHTING AND COLOURS
         ============================================================ -->
    <div id="tab-cg-task4" class="subjectTabContent">
        <div class="subjectHeader">
            <div class="subjectHeaderTitle">
                <span style="font-size:24px">📐</span>
                <div>
                    <h2>CG — Task 4: Projection, Viewing & Rendering</h2>
                    <p>Orthographic vs Perspective viewing, Cohen-Sutherland line clipping, textures, and Phong illumination</p>
                </div>
            </div>
            <div class="subjectHeaderRight">
                <span class="tag" style="background:rgba(69,230,255,0.12);color:var(--cyan);border:1px solid rgba(69,230,255,0.3)">VIEWPORT & LIGHTING ENGINE</span>
            </div>
        </div>

        <div class="taskGrid3">
            <!-- 1. PROJECTION & VIEWING -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>PROJECTION & VIEWING</span>
                    </div>
                    <span class="tag">ORTHO VS PERSPECTIVE</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        <b>Orthographic:</b> Parallel projection rays without depth foreshortening ($x' = x, y' = y$).<br>
                        <b>Perspective:</b> Rays converge to camera eye ($x' = \frac{x \cdot d}{z + d}$).
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <button class="opBtn primary" id="btnProjPersp" onclick="cg4SetProjection('PERSPECTIVE')">Perspective</button>
                            <button class="opBtn secondary" id="btnProjOrtho" onclick="cg4SetProjection('ORTHOGRAPHIC')">Orthographic</button>
                        </div>
                    </div>

                    <canvas id="cg4ProjCanvas" width="300" height="170" style="background:#060814;border:1px solid #1a203c;border-radius:6px;display:block;margin-bottom:8px"></canvas>
                    <div id="cg4ProjExplanation" style="font-family:var(--font-mono);font-size:9.5px;color:var(--cyan)">
                        Perspective projection: Distant vehicles appear smaller.
                    </div>
                </div>
            </section>

            <!-- 2. COHEN-SUTHERLAND CLIPPING -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>COHEN-SUTHERLAND CLIPPING</span>
                    </div>
                    <span class="tag">4-BIT OUTCODES & REGION TEST</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Evaluates traffic vectors against the junction bounding window $[X_{min}, X_{max}, Y_{min}, Y_{max}]$. 4-bit Outcodes determine Trivial Accept (0000), Trivial Reject, or Intersection Clipping.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <button class="opBtn secondary" onclick="cg4SetClipPreset('INSIDE')">Inside (Accept)</button>
                            <button class="opBtn secondary" onclick="cg4SetClipPreset('OUTSIDE')">Outside (Reject)</button>
                            <button class="opBtn warning" onclick="cg4SetClipPreset('CROSSING')">Crossing (Clip)</button>
                        </div>
                    </div>

                    <canvas id="cg4ClipCanvas" width="300" height="170" style="background:#060814;border:1px solid #1a203c;border-radius:6px;display:block;margin-bottom:8px"></canvas>
                    <div id="cg4ClipStatusText" style="font-family:var(--font-mono);font-size:9.5px;color:var(--amber)">
                        Outcode P1: 0000 | Outcode P2: 0000 → TRIVIALLY ACCEPTED
                    </div>
                </div>
            </section>

            <!-- 3. TEXTURE, LIGHTING & COLOUR -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>LIGHTING & PHONG MODEL</span>
                    </div>
                    <span class="tag">AMBIENT, DIFFUSE, SPECULAR</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                        Illumination formula: $I = k_a I_a + k_d I_d (\hat{N} \cdot \hat{L}) + k_s I_s (\hat{R} \cdot \hat{V})^n$. Simulates asphalt roughness, car clearcoat specular gloss, and emergency strobe emission.
                    </p>

                    <div class="opControlBox">
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">Environment:</span>
                            <select id="cg4LightingEnv" class="opSelect" onchange="cg4UpdateLighting()">
                                <option value="DAY">Daylight (High Diffuse)</option>
                                <option value="DUSK">Sunset / Dusk (Warm Ambient)</option>
                                <option value="NIGHT">Night (Headlight Beams)</option>
                            </select>
                        </div>
                        <div class="opFormRow">
                            <span style="font-size:10px;color:var(--text-dim)">Specular Shininess:</span>
                            <input type="range" id="cg4Shininess" min="2" max="64" value="16" oninput="cg4UpdateLighting()">
                        </div>
                    </div>

                    <canvas id="cg4LightCanvas" width="300" height="170" style="background:#060814;border:1px solid #1a203c;border-radius:6px;display:block;margin-bottom:8px"></canvas>
                    <div id="cg4LightStatusText" style="font-family:var(--font-mono);font-size:9.5px;color:var(--green)">
                        Daylight mode: Diffuse coefficient 0.85, Specular shininess 16.
                    </div>
                </div>
            </section>
        </div>
    </div>
"""

PSOOP_OOP_HTML = r"""
    <!-- ============================================================
         PSOOP / OOP — OBJECT-ORIENTED PROGRAMMING
         ============================================================ -->
    <div id="tab-psoop" class="subjectTabContent">
        <section class="card oopPanel" id="psoopSection" style="border-left:4px solid var(--purple)">
            <div class="subjectHeader" style="margin-bottom:16px;padding-bottom:12px;border-bottom:1px solid var(--border)">
                <div class="subjectHeaderTitle">
                    <span style="font-size:26px">🧩</span>
                    <div>
                        <h2 style="font-size:16px;font-weight:800;color:#fff;letter-spacing:0.5px">PSOOP / OOP — Object-Oriented Programming</h2>
                        <p style="font-size:11px;color:var(--text-dim);margin-top:2px">Encapsulation, Class Hierarchies, Dynamic Polymorphism, Abstraction, and Critical Thinking Decision Engine</p>
                    </div>
                </div>
                <div class="subjectHeaderRight">
                    <span class="tag" style="background:rgba(155,124,255,0.15);color:var(--purple);border:1px solid rgba(155,124,255,0.3)">
                        C++ OOP & SYSTEM ARCHITECTURE
                    </span>
                </div>
            </div>

            <!-- 5 CORE OOP CONCEPT CARDS -->
            <div class="oopGrid5" style="display:grid;grid-template-columns:repeat(auto-fit, minmax(210px, 1fr));gap:12px;margin-bottom:18px">
                
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
                        <span style="color:var(--purple);font-weight:700">LIVE INSTANCES:</span><br>
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
                        <span style="color:var(--green);font-weight:700">ACCESS CONTROL:</span><br>
                        • State variables protected against external corruption.<br>
                        • Modifying wait time via <code>UPDATE</code> form validates inputs.
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
                        <span style="color:var(--amber);font-weight:700">HIERARCHY:</span><br>
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
                        <span style="color:var(--purple);font-weight:700">DYNAMIC DISPATCH:</span><br>
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
                        <span style="color:var(--cyan);font-weight:700">HIDDEN COMPLEXITY:</span><br>
                        • FIFO queue array memory management.<br>
                        • Signal phase timer state machine.<br>
                        • Collision detection and vehicle physics.
                    </div>
                </div>

            </div>

            <!-- ============================================================
                 CO.2: CHOOSE AN APPROPRIATE PROGRAMMING SOLUTION TO REDUCE COMPLEXITY
                 ============================================================ -->
            <section class="card" style="margin-bottom:18px;border:1px solid rgba(69,230,255,0.35);background:linear-gradient(135deg, rgba(8,12,30,0.95), rgba(5,7,18,0.98))">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i style="color:var(--cyan);box-shadow:0 0 10px rgba(69,230,255,0.4)">⚡</i>
                        <span style="color:#fff">CO.2 — CHOOSE AN APPROPRIATE PROGRAMMING SOLUTION TO REDUCE COMPLEXITY</span>
                    </div>
                    <span class="tag" style="background:rgba(69,230,255,0.15);color:var(--cyan);border:1px solid rgba(69,230,255,0.35)">
                        OFFICIAL CO.2 BENCHMARK & ARCHITECTURE
                    </span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10.5px;color:var(--text-dim);line-height:1.6;margin-bottom:14px">
                        <b>Problem Statement:</b> Multi-agent four-way traffic arbitration with heterogeneous vehicle classifications, dynamic emergency overrides, and headway safety interlocks can rapidly devolve into an unmaintainable "God Object" with exploding cyclomatic complexity ($M \ge 18$) if implemented using naive procedural branching. 
                        <b>The Chosen Solution:</b> A decoupled Object-Oriented architecture leveraging <b>Polymorphic Dynamic Dispatch</b>, <b>Single Responsibility Principle (SRP)</b>, and dedicated algorithmic data structures (<b>FIFO Queue</b> + <b>Hash Table</b>), reducing cyclomatic complexity by <b>83%</b> and achieving $O(1)$ constant-time lookup and queue buffering.
                    </p>

                    <!-- SIDE-BY-SIDE ARCHITECTURAL COMPARISON -->
                    <div class="taskGrid2" style="margin-bottom:14px">
                        <!-- SOLUTION A: PROCEDURAL ANTI-PATTERN -->
                        <div style="background:#090c1f;border:1px solid #1f274a;border-radius:8px;padding:14px">
                            <div style="display:flex;align-items:center;justify-content:space-between;margin-bottom:8px">
                                <b style="color:var(--red);font-size:11.5px">SOLUTION A: NAIVE PROCEDURAL APPROACH</b>
                                <span class="tag" style="background:rgba(255,77,94,0.15);color:var(--red)">HIGH COMPLEXITY (ANTI-PATTERN)</span>
                            </div>
                            <ul style="font-size:10px;color:var(--text-dim);line-height:1.6;padding-left:18px;margin:0 0 10px 0">
                                <li><b>Monolithic Function:</b> Single 150+ LOC procedure with deeply nested if-else ladders.</li>
                                <li><b>High Cyclomatic Complexity:</b> $M = 18$ independent execution paths requiring 18 tests.</li>
                                <li><b>Scattered Globals:</b> Vehicle state held in parallel arrays without access encapsulation.</li>
                                <li><b>Linear Search Overhead:</b> Iterates through all lanes in $O(N)$ time per cycle.</li>
                                <li><b>Fragile Extensibility:</b> Adding 1 new vehicle type requires modifying 6 switch/if blocks.</li>
                            </ul>
                        </div>

                        <!-- SOLUTION B: CHOSEN MODULAR OOP SOLUTION -->
                        <div style="background:#090c1f;border:1px solid rgba(53,233,140,0.3);border-radius:8px;padding:14px">
                            <div style="display:flex;align-items:center;justify-content:space-between;margin-bottom:8px">
                                <b style="color:var(--green);font-size:11.5px">SOLUTION B: CHOSEN MODULAR OOP + DATA STRUCTURES</b>
                                <span class="tag" style="background:rgba(53,233,140,0.15);color:var(--green)">REDUCED COMPLEXITY (OPTIMAL)</span>
                            </div>
                            <ul style="font-size:10px;color:var(--text-dim);line-height:1.6;padding-left:18px;margin:0 0 10px 0">
                                <li><b>Decoupled Hierarchy:</b> <code>Vehicle</code>, <code>EmergencyVehicle</code>, <code>TrafficSignal</code>, <code>TrafficLane</code>.</li>
                                <li><b>Low Cyclomatic Complexity:</b> $M = 3$ for master arbitration; $M = 1$ per leaf method.</li>
                                <li><b>Virtual Dynamic Dispatch:</b> Priority score computed polymorphically with zero branching.</li>
                                <li><b>Constant-Time Operations:</b> $O(1)$ Hash Table key lookup and $O(1)$ linked FIFO queue.</li>
                                <li><b>Open/Closed Principle (OCP):</b> New vehicles added via simple inheritance with 0 edits to controller.</li>
                            </ul>
                        </div>
                    </div>

                    <!-- INTERACTIVE BENCHMARK RUNNER -->
                    <div class="opControlBox" style="margin-bottom:12px">
                        <div class="opFormRow">
                            <button class="opBtn primary" onclick="runCO2ComplexityBenchmark()">
                                ⚡ Run Live CO.2 Complexity Benchmark & Analysis
                            </button>
                            <button class="opBtn secondary" onclick="toggleCO2CodeComparison()">
                                👁 Toggle Source Code Side-by-Side Inspector
                            </button>
                        </div>
                    </div>

                    <!-- BENCHMARK RESULT METRICS CONTAINER -->
                    <div id="co2BenchmarkResultBox" style="padding:12px;background:#050713;border:1px solid #1a203c;border-radius:8px;font-family:var(--font-mono);font-size:10.5px;min-height:95px;line-height:1.6">
                        <span style="color:var(--text-muted)">Click <b>"Run Live CO.2 Complexity Benchmark"</b> above to execute both solutions against the active vehicle registry and compare quantitative software engineering metrics.</span>
                    </div>

                    <!-- COLLAPSIBLE SOURCE CODE COMPARISON DRAWER -->
                    <div id="co2CodeDrawer" style="display:none;margin-top:12px;background:#04060f;border:1px solid #192040;border-radius:8px;padding:12px">
                        <div style="font-size:10px;font-weight:700;color:var(--cyan);margin-bottom:8px">SOURCE CODE ARCHITECTURAL COMPARISON:</div>
                        <div class="taskGrid2">
                            <div>
                                <div style="color:var(--red);font-size:9.5px;font-weight:700;margin-bottom:4px">Naive Procedural Anti-Pattern (High Cyclomatic Complexity M=18):</div>
                                <pre style="background:#090c1e;border:1px solid #1e2444;border-radius:6px;padding:8px;font-size:8.5px;color:#ff9eaf;overflow-x:auto;max-height:160px;margin:0"><code>// Procedural Anti-Pattern (Fragile, Nested)
int calculatePriorityProcedural(int type, int emerg, int wait) {
    if (emerg == 1) {
        if (type == 1) return wait + 100;      // Ambulance
        else if (type == 2) return wait + 110; // FireTruck
        else if (type == 3) return wait + 90;  // Police
        else return wait + 80;
    } else {
        if (type == 4) return wait + 10;       // Bus
        else if (type == 5) return wait + 5;   // Truck
        else if (type == 6) return wait;       // Bike
        else return wait;                      // Car
    }
}</code></pre>
                            </div>
                            <div>
                                <div style="color:var(--green);font-size:9.5px;font-weight:700;margin-bottom:4px">Chosen OOP Solution (Polymorphic Dynamic Dispatch M=1):</div>
                                <pre style="background:#090c1e;border:1px solid #1e2444;border-radius:6px;padding:8px;font-size:8.5px;color:#9effca;overflow-x:auto;max-height:160px;margin:0"><code>// Chosen OOP Solution (Open/Closed Principle)
class Vehicle {
public:
    virtual int calculatePriority() const { 
        return waitTime; // Normal
    }
};

class EmergencyVehicle : public Vehicle {
public:
    int calculatePriority() const override { 
        return waitTime + 100; // Emergency
    }
};
// Controller: vehicle->calculatePriority(); (Zero branches)</code></pre>
                            </div>
                        </div>
                    </div>
                </div>
            </section>

            <!-- INTERACTIVE OOP DEMONSTRATION WORKBENCH -->
            <div class="oopDemoBench" style="background:linear-gradient(135deg, rgba(14,18,38,0.9), rgba(9,11,24,0.95));border:1px solid rgba(155,124,255,0.25);border-radius:10px;padding:16px;margin-bottom:18px">
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

            <!-- TWO COLUMN TESTBED: LIVE OBJECT INSPECTOR & DYNAMIC POLYMORPHISM -->
            <div class="taskGrid2" style="margin-bottom:18px">
                <!-- 1. LIVE OBJECT INSPECTOR & ENCAPSULATION -->
                <section class="card">
                    <div class="cardHeader">
                        <div class="cardTitle">
                            <i>✦</i>
                            <span>LIVE OBJECT INSPECTOR & ENCAPSULATION</span>
                        </div>
                        <span class="tag">CLASSES, OBJECTS & ACCESS GUARDS</span>
                    </div>
                    <div class="cardBody">
                        <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                            Inspect active instantiated objects from the vehicle database. Private member states (ID, speed, latency) are protected against illegal outside tampering and updated exclusively through encapsulated methods with invariant validation.
                        </p>

                        <div class="opControlBox">
                            <div class="opFormRow">
                                <span style="font-size:10px;color:var(--text-dim)">Select Object:</span>
                                <select id="psoopObjSelect" class="opSelect" onchange="psoopInspectSelectedObject()">
                                    <!-- Populated dynamically -->
                                </select>
                                <button class="opBtn secondary" onclick="psoopTestEncapsulation(true)">Test Valid Wait Mutation</button>
                                <button class="opBtn danger" onclick="psoopTestEncapsulation(false)">Test Illegal Negative Input</button>
                            </div>
                        </div>

                        <div class="oopObjectTree" id="psoopInspectorBox">
                            <!-- Populated dynamically -->
                        </div>
                    </div>
                </section>

                <!-- 2. INHERITANCE & POLYMORPHISM TESTBED -->
                <section class="card">
                    <div class="cardHeader">
                        <div class="cardTitle">
                            <i>✦</i>
                            <span>INHERITANCE & DYNAMIC POLYMORPHISM</span>
                        </div>
                        <span class="tag">VIRTUAL DISPATCH: BASE VS DERIVED</span>
                    </div>
                    <div class="cardBody">
                        <p style="font-size:10px;color:var(--text-dim);margin-bottom:10px">
                            <code>EmergencyVehicle</code> specializes base <code>Vehicle</code> via class inheritance. Both classes define <code>calculatePriority()</code>, but exhibit polymorphic dynamic dispatch based on their runtime instance type.
                        </p>

                        <div class="opControlBox">
                            <div class="opFormRow">
                                <span style="font-size:10px;color:var(--text-dim)">Input Wait Time:</span>
                                <input type="number" id="psoopPolyWait" class="opInput" style="width:65px" value="15" min="0">
                                <button class="opBtn primary" onclick="psoopRunPolymorphismDemo()">Execute Polymorphic Dispatch</button>
                            </div>
                        </div>

                        <div id="psoopPolyComparisonBox" style="padding:12px;background:#050713;border:1px solid #1a203c;border-radius:6px;font-family:var(--font-mono);font-size:10.5px;min-height:120px;line-height:1.6">
                            <!-- Populated dynamically -->
                        </div>
                    </div>
                </section>
            </div>

            <!-- CRITICAL THINKING & LOGIC BUILDING DECISION ENGINE -->
            <section class="card" style="margin-bottom:18px">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>CRITICAL THINKING & LOGIC BUILDING DECISION ENGINE (CO.4)</span>
                    </div>
                    <span class="tag">RULE EVALUATION & SYSTEM HEURISTICS</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10px;color:var(--text-dim);margin-bottom:12px">
                        Test how the Object-Oriented Controller analyzes complex, conflicting junction states and applies safety rules to determine the correct traffic action.
                    </p>

                    <div style="display:flex;flex-wrap:wrap;gap:8px;margin-bottom:14px">
                        <button class="opBtn secondary" onclick="psoopRunScenario('SCENARIO_1')">Scenario A: Emergency Arrives While Normal Vehicles Wait</button>
                        <button class="opBtn secondary" onclick="psoopRunScenario('SCENARIO_2')">Scenario B: Two Conflicting Emergency Units (Severity Arbitration)</button>
                        <button class="opBtn secondary" onclick="psoopRunScenario('SCENARIO_3')">Scenario C: Non-Existent Vehicle ID Query (Exception Recovery)</button>
                        <button class="opBtn secondary" onclick="psoopRunScenario('SCENARIO_4')">Scenario D: Starvation Threshold (Anti-Starvation Balancing)</button>
                    </div>

                    <div id="psoopScenarioResultBox" style="padding:14px;background:#050714;border:1px solid #192040;border-radius:8px;font-family:var(--font-mono);font-size:11px;line-height:1.6;color:var(--text)">
                        Select a critical thinking scenario above to inspect the Input State, Evaluated Heuristic Rules, and Resulting Signal Action.
                    </div>
                </div>
            </section>

            <!-- ARCHITECTURAL COMPLEXITY REDUCTION & MODULARITY ANALYSIS -->
            <section class="card" style="margin-bottom:18px">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>ARCHITECTURAL COMPLEXITY REDUCTION & MODULAR DESIGN</span>
                    </div>
                    <span class="tag">ENGINEERING METRICS</span>
                </div>
                <div class="cardBody">
                    <div class="taskGrid2">
                        <div style="background:#090c1e;border:1px solid #1a203c;border-radius:8px;padding:12px">
                            <b style="color:var(--cyan);font-size:11px">1. Single Responsibility & Separation of Concerns</b>
                            <p style="font-size:10px;color:var(--text-dim);line-height:1.5;margin-top:6px">
                                The system decomposes traffic into distinct classes: <code>Vehicle</code> (state & physics), <code>TrafficSignal</code> (lamp states & interlocks), <code>TrafficLane</code> (headway buffering), and <code>TrafficController</code> (arbitration). No monolithic god-objects.
                            </p>
                        </div>
                        <div style="background:#090c1e;border:1px solid #1a203c;border-radius:8px;padding:12px">
                            <b style="color:var(--green);font-size:11px">2. Open / Closed Extensibility</b>
                            <p style="font-size:10px;color:var(--text-dim);line-height:1.5;margin-top:6px">
                                New vehicle classifications (e.g. <code>VIPConvoys</code>, <code>PoliceInterceptors</code>) are added by simply inheriting from <code>Vehicle</code> and overriding <code>calculatePriority()</code>, requiring zero modifications to the core arbitration loop.
                            </p>
                        </div>
                    </div>
                    <div style="margin-top:10px;padding:10px;background:#050713;border:1px solid #14182e;border-radius:6px;font-family:var(--font-mono);font-size:9.5px;color:var(--text-dim)">
                        <span style="color:var(--amber);font-weight:700">ALGORITHMIC COMPLEXITY NOTE:</span>
                        Arbitration scan operates in $O(L \cdot V)$ time where $L=4$ lanes and $V$ is vehicles per lane. OOP encapsulation cleanly decouples time complexity from system maintenance overhead.
                    </div>
                </div>
            </section>

            <!-- MAPPING TO C++ OPENGL IMPLEMENTATION -->
            <section class="card">
                <div class="cardHeader">
                    <div class="cardTitle">
                        <i>✦</i>
                        <span>MAPPING TO NATIVE C++ OPENGL IMPLEMENTATION</span>
                    </div>
                    <span class="tag">CODEBASE SYNCHRONIZATION</span>
                </div>
                <div class="cardBody">
                    <p style="font-size:10.5px;color:var(--text-dim);margin-bottom:10px">
                        This interactive prototype models the exact object hierarchy and methods implemented in <code>PSOOP_CPP/traffic_oop.cpp</code> and <code>CG_OpenGL_3D/traffic_3d.cpp</code>:
                    </p>
                    <div class="traceTableWrap">
                        <table class="traceTable">
                            <thead>
                                <tr><th>PROTOTYPE JAVASCRIPT CLASS</th><th>C++ NATIVE CLASS / SOURCE</th><th>KEY ENCAPSULATED METHODS</th><th>OOP PRINCIPLE DEMONSTRATED</th></tr>
                            </thead>
                            <tbody>
                                <tr><td><code>SimulationVehicle</code></td><td><code>Vehicle</code> (traffic_oop.cpp)</td><td><code>calculatePriority()</code>, <code>displayInfo()</code></td><td>Encapsulation & Base Abstraction</td></tr>
                                <tr><td><code>EmergencySimulationVehicle</code></td><td><code>EmergencyVehicle : public Vehicle</code></td><td><code>calculatePriority() override</code> (+100)</td><td>Class Inheritance & Polymorphism</td></tr>
                                <tr><td><code>TrafficLaneManager</code></td><td><code>TrafficLane</code> (traffic_oop.cpp)</td><td><code>addVehicle()</code>, <code>getHighestPriorityVehicle()</code></td><td>Aggregation & Headway Container</td></tr>
                                <tr><td><code>TrafficController</code></td><td><code>TrafficController</code> (traffic_oop.cpp)</td><td><code>evaluateAndDispatchTraffic()</code></td><td>Controller Abstraction & State Machine</td></tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </section>
        </section>
    </div>
"""


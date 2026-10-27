# build_review2.py
import re
import sys
from css_styles import CSS_REVIEW2
from html_sections import NAV_BAR_HTML, PL_TASK3_HTML, PL_TASK4_HTML, COA_TASK3_HTML, COA_TASK4_HTML, CG_TASK3_HTML, CG_TASK4_HTML, PSOOP_OOP_HTML
from js_logic import JS_LOGIC

def build():
    bak_path = r"c:\Users\supri\Desktop\SmartTraffic_Review2\Web_Simulation\index.html.bak"
    out_path = r"c:\Users\supri\Desktop\SmartTraffic_Review2\Web_Simulation\index.html"

    with open(bak_path, "r", encoding="utf-8") as f:
        content = f.read()

    # 1. Inject CSS before </style>
    style_end = content.find("</style>")
    if style_end == -1:
        print("ERROR: </style> not found")
        sys.exit(1)
    
    content = content[:style_end] + "\n" + CSS_REVIEW2 + "\n" + content[style_end:]

    # 2. Inject Navigation Bar after </section> of quickStats
    quick_stats_match = re.search(r'(<section\s+class=["\']quickStats["\'].*?</section>)', content, re.DOTALL)
    if not quick_stats_match:
        print("ERROR: quickStats section not found")
        sys.exit(1)

    quick_stats_end = quick_stats_match.end()
    
    # Wrap existing mainLayout + cgPanel + tableCard in <div id="tab-overview" class="subjectTabContent active">
    main_layout_start = content.find('<div class="mainLayout">', quick_stats_end)
    if main_layout_start == -1:
        print("ERROR: mainLayout not found")
        sys.exit(1)

    table_card_match = re.search(r'(<section\s+class=["\']card\s+tableCard["\'].*?</section>)', content, re.DOTALL)
    if not table_card_match:
        print("ERROR: tableCard not found")
        sys.exit(1)

    table_card_end = table_card_match.end()

    # Reconstruct body with tabs
    part1 = content[:quick_stats_end] + "\n" + NAV_BAR_HTML + "\n"
    part2 = '<div id="tab-overview" class="subjectTabContent active">\n' + content[main_layout_start:table_card_end] + '\n</div>\n'
    
    # All other tab sections
    all_subject_tabs = "\n".join([
        PL_TASK3_HTML,
        PL_TASK4_HTML,
        COA_TASK3_HTML,
        COA_TASK4_HTML,
        CG_TASK3_HTML,
        CG_TASK4_HTML,
        PSOOP_OOP_HTML
    ])

    part3 = all_subject_tabs + "\n"

    # Rest of document from table_card_end to </script>
    rest = content[table_card_end:]

    # Inject JS_LOGIC inside <script> right before </script>
    script_end = rest.rfind("</script>")
    if script_end == -1:
        print("ERROR: </script> not found")
        sys.exit(1)

    # In rest, hook syncAllModules into initSimulation
    rest_with_js = rest[:script_end] + "\n" + JS_LOGIC + "\n" + rest[script_end:]

    final_content = part1 + part2 + part3 + rest_with_js

    # Add hook calls to displayVehicles and initSimulation in final_content
    # In displayVehicles(): ensure syncAllModules() is called if defined
    final_content = final_content.replace(
        "renderAllDataStructures();\n    updateDashboardStats();\n    updateCOA();",
        "renderAllDataStructures();\n    updateDashboardStats();\n    updateCOA();\n    if (typeof syncAllModules === 'function') syncAllModules();"
    )

    final_content = final_content.replace(
        "updateCGPanel();   // CG Engine: CO1, CO2, CO3, CO5",
        "updateCGPanel();\n    if (typeof cg3Draw3DCanvas === 'function' && (typeof currentActiveTab !== 'undefined') && (currentActiveTab === 'tab-cg-task3' || currentActiveTab === 'tab-all')) cg3Draw3DCanvas(); // CG Engine: CO1, CO2, CO3, CO5"
    )

    with open(out_path, "w", encoding="utf-8") as f:
        f.write(final_content)

    print(f"SUCCESS: Successfully generated {out_path} ({len(final_content)} characters).")

if __name__ == "__main__":
    build()

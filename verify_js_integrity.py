# verify_js_integrity.py
import re
import sys

with open("Web_Simulation/index.html", "r", encoding="utf-8") as f:
    content = f.read()

# Extract script contents
scripts = re.findall(r'<script\b[^>]*>(.*?)</script>', content, re.DOTALL)
print(f"Total script blocks found: {len(scripts)}")

full_js = "\n".join(scripts)
print(f"Total JS size: {len(full_js)} characters")

# Check balanced brackets
stack = []
open_brackets = {'(': ')', '{': '}', '[': ']'}
close_brackets = {')': '(', '}': '{', ']': '['}

in_string = False
str_char = None
in_comment = False
in_block_comment = False

errors = 0
for i, c in enumerate(full_js):
    # string handling
    if not in_comment and not in_block_comment:
        if not in_string and (c in ["'", '"', '`']):
            in_string = True
            str_char = c
            continue
        elif in_string and c == str_char and full_js[i-1] != '\\':
            in_string = False
            str_char = None
            continue

    if in_string:
        continue

    # single line comment
    if not in_block_comment and full_js[i:i+2] == '//':
        in_comment = True
    if in_comment and c == '\n':
        in_comment = False
        continue

    # block comment
    if not in_comment and full_js[i:i+2] == '/*':
        in_block_comment = True
    if in_block_comment and full_js[i:i+2] == '*/':
        in_block_comment = False
        continue

    if in_comment or in_block_comment:
        continue

    if c in open_brackets:
        stack.append((c, i))
    elif c in close_brackets:
        if not stack:
            print(f"Unmatched closing bracket '{c}' at position {i}")
            errors += 1
        else:
            top, pos = stack.pop()
            if open_brackets[top] != c:
                print(f"Mismatched bracket '{top}' at {pos} closed by '{c}' at {i}")
                errors += 1

if stack:
    print(f"Unclosed brackets remaining: {len(stack)} (e.g. {stack[-1]})")
    errors += 1

if errors == 0:
    print("SUCCESS: JavaScript brackets and strings are balanced and valid!")
else:
    print(f"WARNING: Found {errors} bracket issues")

# Verify critical functions exist
req_funcs = [
    "switchSubjectTab", "syncAllModules",
    "pl3Enqueue", "pl3Dequeue", "pl3PushEmergency", "pl3PopEmergency",
    "pl4ExecuteSearch", "pl4ExecuteSort", "pl4SyncHashTable",
    "coa3ComputeAddition", "coa3ComputeMultiplication", "coa3ComputeDivision",
    "coa4UpdateControlSignals", "coa4StepPipeCycle", "coa4ResetPipeline",
    "cg3Draw3DCanvas", "cg3RunBresenhamDemo", "cg3RunCircleDemo",
    "cg4DrawProjectionDemo", "cg4DrawClippingDemo", "cg4DrawLightingDemo",
    "psoopPopulateObjectSelect", "psoopInspectSelectedObject",
    "psoopRunPolymorphismDemo", "psoopRunScenario",
    "refreshOOPVehicleList", "runOOPDemo", "scrollToOOP"
]

missing = [fn for fn in req_funcs if f"function {fn}" not in full_js and f"{fn} =" not in full_js]
if missing:
    print(f"Missing functions: {missing}")
else:
    print(f"SUCCESS: All {len(req_funcs)} critical subject controller functions are defined!")

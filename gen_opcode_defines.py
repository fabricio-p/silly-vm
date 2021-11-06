from json import loads
from pathlib import Path
from sys import argv, stderr

template = ""
mline_comment = None
sline_comment = None
for_vm = True
if len(argv) > 1 and argv[1].lower() in ("py", "python"):
    template = "%s%s = %i"
    mline_comment = sline_comment = lambda s: f"# {s}"
    for_vm = False
else:
    template = "#define SILLY_INSTR_%s%s %i"
    mline_comment = lambda s: f"/* {s} */"
    sline_comment = lambda s: f"// {s}"

ops = list(filter(bool, Path("opcode_list.txt").read_text().split("\n")))
max_instr_width = 0
max_superinstr_width = 0

superinstr_start = 0
i = 0
while i < len(ops):
    op = ops[i].strip()
    if op.startswith('-'):
        superinstr_start = i + 1
        break;
    if op.startswith('#'):
        i += 1
        continue
    print(op)
    if len(op) > max_instr_width:
        max_instr_width = len(op)
    i += 1

if superinstr_start != 0:
    i = superinstr_start
    while i < len(ops):
        op = ops[i].strip()
        if op.startswith('#'):
            i += 1
            continue
        if len(op) > max_superinstr_width:
            max_superinstr_width = len(op)
        i += 1

defines = [mline_comment("Regular instructions ")] if for_vm else []

i = 0
code = 0
while i < len(ops):
    op = ops[i].strip()
    if op.startswith('#'):
        defines.append(sline_comment(op[2:]))
        i += 1
        continue
    if op == '':
        i += 1
        continue
    if op.startswith('-'):
        if not for_vm:
            break
        i += 1
        code = 0x100
        max_instr_width = max_superinstr_width
        defines.append(mline_comment("Superinstructions"))
        continue
    whitespace = max_instr_width - len(op)
    defines.append(template % (
                                    op.upper().replace(".", "_"),
                                    " " * whitespace, code))
    i += 1
    code += 1

print("\n".join(defines))

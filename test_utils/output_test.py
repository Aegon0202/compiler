import subprocess
import os.path
import os
in_out_fd = "../compiler2021/testcase/performance_test2021_pre"
# in_out_fd = "../compiler2021/testcase/function_test2020"
#in_out_fd = "../compiler2021/testcase/function_test2021"

for f in os.listdir("./tmp_result"):
    print(f"testing {f}")
    f_name, f_ext = os.path.splitext(f)
    in_f = os.path.join(in_out_fd, f"{f_name}.in")
    out_f = os.path.join(in_out_fd, f"{f_name}.out")
    p = os.path.join("./tmp_result", f)
    cmd: subprocess.CompletedProcess = subprocess.run(
        ["gcc", "-g", "-march=armv7-a", p, "-o", "tmp.o", "-L", "-lsysy", "./libsysy.a"], capture_output=True, text=True)
    if cmd.returncode != 0 or len(cmd.stderr) or len(cmd.stdout):
        print(f"compile error {f}")
        print(f"return code {cmd.returncode}")
        print(cmd.stderr)
        print(cmd.stdout)
        break
    cmd = None
    if os.path.exists(in_f):
        in_ff = open(in_f, "r")
        cmd: subprocess.CompletedProcess = subprocess.run(
            ["./tmp.o"], text=True, capture_output=True, input=in_ff.read())
        in_ff.close()
    else:
        cmd: subprocess.CompletedProcess = subprocess.run(
            ["./tmp.o"], text=True, capture_output=True)
    if len(cmd.stderr) and not cmd.stderr.startswith("TOTAL:") and not cmd.stderr.startswith("Timer"):
        print(f"run time error {f}")
        print(cmd.stderr)
        break
    if len(cmd.stderr):
        print(cmd.stderr)
    out_content = ""
    if len(cmd.stdout):
        if cmd.stdout[-1] == '\n':
            out_content = f"{cmd.stdout}{cmd.returncode}"
        else:
            out_content = f"{cmd.stdout}\n{cmd.returncode}"
    else:
        out_content = f"{cmd.returncode}"

    out_ff = open(out_f, "r")
    out_t = out_ff.read()
    out_ff.close()
    if out_t[-1] == "\n":
        out_t = out_t[:-1]
    if out_content != out_t:
        print(f"wrong answer {f}")
        print(out_t.encode())
        print(out_content.encode())
        break

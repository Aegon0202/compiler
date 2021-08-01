import subprocess
import os.path
import os
import sys

if __name__ == "__main__":
    test_folder_dict = {
        "2020": "../compiler2021/testcase/function_test2020",
        "2021": "../compiler2021/testcase/function_test2021",
        "2021p": "../compiler2021/testcase/performance_test2021_pre",
        "2020f": "../sysyruntimelibrary/section1/functional_test",
        "2020p": "../sysyruntimelibrary/section1/performance_test",
        "2020p2": "../sysyruntimelibrary/section2/performance_test"
    }

    in_out_fd = test_folder_dict[sys.argv[1]]

    for f in os.listdir("./tmp_result"):
        print(f"testing {f}")
        f_name, f_ext = os.path.splitext(f)
        in_f = os.path.join(in_out_fd, f"{f_name}.in")
        out_f = os.path.join(in_out_fd, f"{f_name}.out")
        p = os.path.join("./tmp_result", f)
        cmd: subprocess.CompletedProcess = subprocess.run(
            ["gcc", "-g", "-march=armv7-a", p, "-o", "tmp.o", "-L", "-lsysy", "./libsysy.a"], capture_output=True, text=True)
        if cmd.returncode != 0 or len(cmd.stderr) or len(cmd.stdout):
            print(f"compile error {p}")
            print(f"return code {cmd.returncode}")
            print(cmd.stderr)
            print(cmd.stdout)
            continue
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
            print(f"run time error {p}")
            print(cmd.stderr)
            continue
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
            print(f"wrong answer {p}")
            print(out_t.encode())
            print(out_content.encode())
            continue

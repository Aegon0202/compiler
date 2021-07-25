import subprocess
import os.path
import os


for f in os.listdir("../../compiler2021/testcase/function_test2020/"):
    print(f"testing {f}")
    f_name, f_ext = os.path.splitext(f)
    if f_ext == ".sy":
        f = os.path.join("../../compiler2021/testcase/function_test2020", f)
        cmd: subprocess.CompletedProcess = subprocess.run(["../compiler", "-S", "-o",
                                                           f"tmp_result/{f_name}.s", f], capture_output=True, text=True)
        if cmd.returncode != 0 or len(cmd.stderr):
            print(f"compile error {f}")
            print(f"return code {cmd.returncode}")
            print(cmd.stderr)
            break
        elif len(cmd.stdout):
            print(f"compile output {f}")
            print(cmd.stdout)

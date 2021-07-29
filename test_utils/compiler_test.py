import subprocess
import os.path
import os

test_folder = "../compiler2021/testcase/performance_test2021_pre"
# test_folder = "../compiler2021/testcase/function_test2021"
#test_folder = "../compiler2021/testcase/function_test2020"
if not os.path.exists("tmp_result"):
    os.mkdir("tmp_result")
for f in os.listdir(test_folder):
    f_name, f_ext = os.path.splitext(f)
    if f_ext == ".sy":
        print(f"testing {f}")
        f = os.path.join(test_folder, f)
        cmd: subprocess.CompletedProcess = subprocess.run(["./compiler", "-S", "-o",
                                                           f"tmp_result/{f_name}.s", f], capture_output=True, text=True)
        if cmd.returncode != 0 or len(cmd.stderr):
            print(f"compile error {f}")
            print(f"return code {cmd.returncode}")
            print(cmd.stderr)
            break
        elif len(cmd.stdout):
            print(f"compile output {f}")
            print(cmd.stdout)
'''scp -r ./tmp_result pi@192.168.4.1:/home/pi/Documents/git_repo/tmp/'''

import subprocess
import os.path
import os
import shutil
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

    test_folder = test_folder_dict[sys.argv[1]]
    if os.path.exists("tmp_result"):
        shutil.rmtree("tmp_result")
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
                exit(-1)
            elif len(cmd.stdout):
                print(f"compile output {f}")
                print(cmd.stdout)
    '''scp -r ./tmp_result pi@192.168.4.1:/home/pi/Documents/git_repo/tmp/'''

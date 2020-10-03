import os
import subprocess

for filename in os.listdir(os.getcwd() + "\cmake-build-debug"):
    process = subprocess.Popen(["SQLiteClone.exe"])
    # process.wait()
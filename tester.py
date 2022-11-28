import os
import subprocess

directory = 'lattests/bad'

exe = "cpp_version/latc_x86"

for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    if(f.endswith(".lat")):
        print(f)
        subprocess.run([exe, f])
    
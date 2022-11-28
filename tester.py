import os
import subprocess

directory = 'lattests/good'

exe = "cpp_version/latc_x86"

file_list = os.listdir(directory)
file_list.sort()
for filename in file_list:
    f = os.path.join(directory, filename)
    if(f.endswith(".lat")):
        #print(f)
        subprocess.run([exe, f])
    
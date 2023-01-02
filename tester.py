import os
import subprocess

directory = 'lattests/good'

exe = "cpp_version/latc_x86_32"

file_list = os.listdir(directory)
file_list.sort()
for filename in file_list:
    f = os.path.join(directory, filename)
    if(f.endswith(".lat")):
        print(f)
        subprocess.run([exe, f])

        prog = os.path.splitext(f)[0]
        with open(prog + ".myout", "w") as output:
            if os.path.exists(prog + ".input"):
                with open(prog + ".input", "rb") as input:
                    subprocess.run([prog], stdout=output, input=input.read())
            else:        
                subprocess.run([prog], stdout=output)

        out = subprocess.run(["diff", prog + ".output", prog + ".myout"], capture_output=True)
        if len(out.stdout) > 0:
            print(out.stdout)
    
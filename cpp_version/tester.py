import os
import subprocess

directory = '../mrjp-tests-master/good/basic'
directory = '../lattests/good'

exe = "./latc_x86_32"

file_list = os.listdir(directory)
file_list.sort()
all = 0
passed = 0
for filename in file_list:
    f = os.path.join(directory, filename)
    if(f.endswith(".lat")):
        all += 1
        try:
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
            else:
                passed += 1
        except:
            print("FAILED\n")

print("passed " + str(passed) + "/" + str(all))
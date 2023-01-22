import os
import subprocess

directories = ['lattests/good', './mrjp-tests-master/good/basic',
'lattests/extensions/objects1','lattests/extensions/objects2',
'lattests/extensions/struct',  './mrjp-tests-master/good/virtual',
'./mrjp-tests-master/gr5',
"latte-compiler-tests/good/classes/chains",
"latte-compiler-tests/good/basic/blocks",
"latte-compiler-tests/good/basic/ifs",
"latte-compiler-tests/good/basic/returns",
"latte-compiler-tests/good/basic/simple",
"latte-compiler-tests/good/basic/while",
"latte-compiler-tests/good/classes/simple"]

sum_all = 0
sum_passed = 0
exe = "./cpp_version/latc"
for directory in directories:
    directory = "tests/" + directory
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
                subprocess.run("rm " + prog, shell=True, check=True)
                if len(out.stdout) > 0:
                    print(out.stdout)
                else:
                    passed += 1
            except:
                print("FAILED\n")

    subprocess.run("rm " + directory + "/*.s", shell=True, check=True)
    subprocess.run("rm " + directory + "/*.myout", shell=True, check=True)
    print("passed " + str(passed) + "/" + str(all))
    sum_all += all
    sum_passed += passed

bad_directs = ["tests/lattests/bad", "tests/mrjp-tests-master/bad/infinite_loop","tests/mrjp-tests-master/bad/semantic",
 "tests/latte-compiler-tests/bad/basic/fn-call-error",
 "tests/latte-compiler-tests/bad/basic/incorrect-return-type",
 "tests/latte-compiler-tests/bad/basic/main-with-args",
 "tests/latte-compiler-tests/bad/basic/no-main",
 "tests/latte-compiler-tests/bad/basic/no-return-from-non-void-f",
 "tests/latte-compiler-tests/bad/basic/repeated-arg-name",
 "tests/latte-compiler-tests/bad/basic/type-error-in-exprs",
 "tests/latte-compiler-tests/bad/basic/variable-not-declared",
 "tests/latte-compiler-tests/bad/basic/variable-redeclared",
 "tests/latte-compiler-tests/bad/structs",
 "tests/latte-compiler-tests/bad/objects"]
 #"tests/latte-compiler-tests/bad/basic/",

all_bad = 0
all_found = 0

for dir in bad_directs:
    file_list = os.listdir(dir)
    file_list.sort()
    all = 0
    found_err = 0
    for filename in file_list:
        f = os.path.join(dir, filename)
        if(f.endswith(".lat")):
            all += 1
            print(f)
            try:
                out = subprocess.run([exe, f], capture_output=True)
                a = out.stderr.decode("utf-8")
                first_line = a.partition('\n')[0]

                if first_line == "ERROR":
                    found_err += 1
                else:
                    print(first_line)
            except:
                print("FAILED\n")

    print("found errs " + str(found_err) + "/" + str(all))
    all_bad += all
    all_found += found_err

print("\nSUMMARY passed " + str(sum_passed) + "/" + str(sum_all))
print("SUMMARY found errs " + str(all_found) + "/" + str(all_bad)+ '\n')
import subprocess

def runTestSpellChecker(path, inputPath = ''): 
    print(inputPath)
    process = start(path)
    testDataFile = open(inputPath, mode = 'r')

    testDataInput = testDataFile.readlines()
    countDelemiter = 0
    passed = True
    for line in testDataInput:
        if countDelemiter > 1:
            outData = read(process) + "\n"
            #print(outData)
            if outData != line:
                passed = False
                print("Expected: " + line)
                print("  Actual: " + outData)
        else :
            write(process, line)
            #print(line)
        if line == "===\n":
            countDelemiter = countDelemiter + 1

    if passed:
        print("Test " + path + " passed")

    terminate(process)


def start(executable_file):
    return subprocess.Popen(
        executable_file,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE)


def read(process):
    return process.stdout.readline().decode("utf-8").strip()


def write(process, message):
    process.stdin.write(f"{message.strip()}\n".encode("utf-8"))
    process.stdin.flush()


def terminate(process):
    process.stdin.close()
    process.terminate()
    process.wait(timeout=0.2)

runTestSpellChecker("../../x64/Release/Spell Checker.exe", 'testData/test.txt')
runTestSpellChecker("../../x64/Release/Spell Checker.exe", 'testData/example.txt')
runTestSpellChecker("../../x64/Release/Spell Checker.exe", 'testData/test2.txt')




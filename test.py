import unittest
import subprocess
import sys

# opener = "open" if sys.platform == "darwin" else "xdg-open"

process = subprocess.Popen([ "cmake-build-debug/DBLite"],
                           stdin=subprocess.PIPE,
                           stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE,
                           text=True)

command = "insert 1 ishan ishan@test.com\n"
process.stdin.write(command)
command_select = 'select\n'
process.stdin.write(command_select)
command_exit = '.exit'
process.stdin.write(command_exit)
# process.stdin.close()

output, error = process.communicate()


if output:
    print("Output:\n", output)
if error:
    print("Error:\n", error)
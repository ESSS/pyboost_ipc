import subprocess
import sys
import os

command = [os.path.join(os.environ['PREFIX'], 'python_d.exe'), '-c', 'import pyboost_ipc']
sys.exit(subprocess.call(command, shell=True))

import os
import sys
import glob
from io import StringIO

original_stdout = sys.stdout
sys.stdout = StringIO()
from PyOneLiner import OneLiner
sys.stdout = original_stdout

def Oliner(f):
    oneliner = OneLiner(f, type_="python")
    oneliner.binary()
    return oneliner.done()

def gen(out, txt):
    with open(out, "w") as file:
        file.write(txt)
        
if len(sys.argv) == 3:
    ff = sys.argv[1]
    o = sys.argv[2]
    
    text = Oliner(ff)
    gen(o, text)
    
    if os.path.exists(o):
        print(f"File {o} berhasil dibuat")
    else:
        print(f"File {o} gagal dibuat")
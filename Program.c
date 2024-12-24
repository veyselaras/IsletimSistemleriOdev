#include "Program.h"

// Arka plan süreçleri bittiğinde yazdırmak için handler
void sigchld_handler(int signo) {
    int durum;
    pid_t pid;
    // Non-blocking wait
    while ((pid = waitpid(-1, &durum, WNOHANG)) > 0) {
        // Arka plan listeden çıkar ve sonucu göster
        printf("[%d] retval: %d\n", pid, WEXITSTATUS(durum));
        fflush(stdout);
        arkaplanProcess_t** onceki = &arkaplanListesi;
        arkaplanProcess_t* suanki = arkaplanListesi;
        while (suanki) {
            if (suanki->pid == pid) {
                *onceki = suanki->sonraki;
                free(suanki);
                break;
            }
            onceki = &suanki->sonraki;
            suanki = suanki->sonraki;
        }
    }
}

// Arka plan sürecini listeye ekleme
void arkaPlanProcessEkle(pid_t pid) {
    arkaplanProcess_t* yeniProcess = malloc(sizeof(arkaplanProcess_t));
    yeniProcess->pid = pid;
    yeniProcess->sonraki = arkaplanListesi;
    arkaplanListesi = yeniProcess;
}


// Tüm arka plan süreçlerinin bitmesini beklemek
void arkplanBekle() {
    arkaplanProcess_t* suanki = arkaplanListesi;
    while (suanki) {
        int durum;
        waitpid(suanki->pid, &durum, 0);
        printf("[%d] retval: %d\n", suanki->pid, WEXITSTATUS(durum));
        arkaplanProcess_t* temp = suanki;
        suanki = suanki->sonraki;
        free(temp);
    }
}

// Komutun parse edilmesi
int parse_command(char* satir, char** args, char** girisDosyasi, char** cikisDosyasi, int* arkaplan, char*** pipeKomutları, int* pipeSayac) {
    *girisDosyasi = NULL;
    *cikisDosyasi = NULL;
    *arkaplan = 0;
    *pipeSayac = 0;
    *pipeKomutları = NULL;

    {
        char* harf = satir;
        int count = 0;
        while ((harf = strchr(harf, '|')) != NULL) {
            count++;
            harf++;
        }
        *pipeSayac = count;
    }

    if (*pipeSayac > 0) {
        *pipeKomutları = malloc(sizeof(char*) * (*pipeSayac + 1));
        int index = 0;
        char* kayitPtr;
        char* token = strtok_r(satir, "|", &kayitPtr);
        while (token) {
            (*pipeKomutları)[index++] = strdup(token);
            token = strtok_r(NULL, "|", &kayitPtr);
        }
        return 0;
    }

    char* kayitPtr;
    int argc = 0;
    char* token = strtok_r(satir, " \t", &kayitPtr);
    while (token && argc < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok_r(NULL, " \t", &kayitPtr);
            if (token) {
                *girisDosyasi = token;
            }
            else {
                fprintf(stderr, "Giris dosyasi belirtilmedi.\n");
                return -1;
            }
        }
        else if (strcmp(token, ">") == 0) {
            token = strtok_r(NULL, " \t", &kayitPtr);
            if (token) {
                *cikisDosyasi = token;
            }
            else {
                fprintf(stderr, "Cikis dosyasi belirtilmedi.\n");
                return -1;
            }
        }
        else if (strcmp(token, "&") == 0) {
            *arkaplan = 1;
        }
        else {
            args[argc++] = token;
        }
        token = strtok_r(NULL, " \t", &kayitPtr);
    }
    args[argc] = NULL;

    return 0;
}
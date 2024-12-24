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
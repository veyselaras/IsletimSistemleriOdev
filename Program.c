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
int a=10;
# Sisop-2-2024-MH-IT01

## Anggota
### Nayla Raissa Azzahra (5027231054)
### Ryan Adya Purwanto (5027231046)
### Rafael Gunawan (5027231019)

## Ketentuan
### Struktur repository seperti berikut : 
	—soal_1:
		— virus.c
	—soal_2:
		— management.c
	 —soal_3:
		— admin.c
	—soal_4:
		— setup.c

## Soal 1
> Nayla 5027231054

Soal nomor 1 meminta kita untuk mereplace beberapa string yang ada di sebuah file dengan ketentuan:
1. String m4LwAr3 direplace dengan string [MALWARE]
2. String 5pYw4R3 direplace dengan string [SPYWARE]
3. String R4nS0mWaR3 direplace dengan string [RANSOMWARE]

Program harus berjalan secara daemon dengan jeda 15 detik, dan setiap penghapusan string dicatat dalam file bernama virus.log dengan format isi  [dd-mm-YYYY][HH:MM:SS] Suspicious string at <file_name> successfully replaced!. Program dapat menerima input path berupa ‘argv’ untuk mengatur folder dimana file akan dieksekusi.

### Penyelesaian
Untuk soal nomor 4, saya membuat dua folder. Folder pertama bernama "target" untuk menyimpan file.txt yang berisi string-string yang akan diubah sesuai ketentuan soal. Sedangkan folder kedua bernama "virusfolder" untuk menyimpan program virus.c serta tempat untuk meletakkan file virus.log.
### virus.c
''' bash
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_FILE "virus.log"
'''
- Header <time.h> berfungsi untuk deklarasi fungsi-fungsi dan tipe data terkait waktu dan tanggal
- Header <sys/stat.h> berfungsi untuk memberikan akses ke fungsi-fungsi dan struktur data terkait dengan informasi status file. Dalam kode tersebut, header ini digunakan karena fungsi truncate() yang digunakan untuk memangkas file ke panjang yang ditentukan.
- Header  <sys/types.h> berisi definisi tipe data khusus yang berkaitan dengan sistem, seperti tipe data pid_t, yang merupakan tipe data untuk menyimpan ID proses (Process ID).
- #define LOG_FILE "virus.log" berfungsi untuk mendefinisikan nama file log yang akan digunakan, yaitu "virus.log"
'''bash
void log_replacement(const char *filename) {
    FILE *logFile = fopen(LOG_FILE, "a");
    if (logFile == NULL) {
        perror("Error opening log file");
        return;
    }
'''
Masuk ke fungsi pertama, yaitu log_replacement untuk menentukan isi dari file virus.log. Pertama-tama, file log akan dibuka terlebih dahulu. Jika file log gagal dibuka, maka akan muncul pesan error "Error opening log file".
'''bash
time_t now;
    struct tm *timeinfo;
    char buffer[80];

    time(&now);
    timeinfo = localtime(&now);

    strftime(buffer, sizeof(buffer), "[%d-%m-%Y][%H:%M:%S]", timeinfo);
    fprintf(logFile, "%s Suspicious string at %s successfully replaced!\n", buffer, filename);
    fclose(logFile);
}
'''
Potongan kode ini berfungsi untuk mengambil informasi mengenai waktu saat ini, menyimpannya ke dalam variabel timeinfo, lalu mencetaknya menjadi string bersamaan dengan pesan Suspicious string at %s successfully replaced!. Setelah pencatatan penghapusan string dilakukan ke dalam file log, maka file log ini akan ditutup menggunakan fclose.
'''bash
void replace_string_in_file(const char *filepath) {
    FILE *fp = fopen(filepath, "r+");
    if (!fp) return;
'''
Masuk ke fungsi kedua, yaitu replace string untuk mengganti string sesuai dengan ketentuan di soal. Pertama-tama, file.txt 

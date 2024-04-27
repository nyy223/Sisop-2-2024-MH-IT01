# Laporan Resmi Praktikum Sisop-2-2024-MH-IT01

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
Untuk soal nomor 1, saya membuat dua folder. Folder pertama bernama "target" untuk menyimpan file.txt yang berisi string-string yang akan diubah sesuai ketentuan soal. Sedangkan folder kedua bernama "virusfolder" untuk menyimpan program virus.c serta tempat untuk meletakkan file virus.log.
### virus.c
```bash
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_FILE "virus.log"
```
- Header <time.h> berfungsi untuk deklarasi fungsi-fungsi dan tipe data terkait waktu dan tanggal
- Header <sys/stat.h> berfungsi untuk memberikan akses ke fungsi-fungsi dan struktur data terkait dengan informasi status file. Dalam kode tersebut, header ini digunakan karena fungsi truncate() yang digunakan untuk memangkas file ke panjang yang ditentukan.
- Header  <sys/types.h> berisi definisi tipe data khusus yang berkaitan dengan sistem, seperti tipe data pid_t, yang merupakan tipe data untuk menyimpan ID proses (Process ID).
- #define LOG_FILE "virus.log" berfungsi untuk mendefinisikan nama file log yang akan digunakan, yaitu "virus.log"
```bash
void log_replacement(const char *filename) {
    FILE *logFile = fopen(LOG_FILE, "a");
    if (logFile == NULL) {
        perror("Error opening log file");
        return;
    }
```
Masuk ke fungsi pertama, yaitu log_replacement untuk menentukan isi dari file virus.log. Pertama-tama, file log akan dibuka terlebih dahulu. Jika file log gagal dibuka, maka akan muncul pesan error "Error opening log file".
```bash
time_t now;
    struct tm *timeinfo;
    char buffer[80];

    time(&now);
    timeinfo = localtime(&now);

    strftime(buffer, sizeof(buffer), "[%d-%m-%Y][%H:%M:%S]", timeinfo);
    fprintf(logFile, "%s Suspicious string at %s successfully replaced!\n", buffer, filename);
    fclose(logFile);
}
```
Potongan kode ini berfungsi untuk mengambil informasi mengenai waktu saat ini, menyimpannya ke dalam variabel timeinfo, lalu mencetaknya menjadi string bersamaan dengan pesan Suspicious string at %s successfully replaced!. Setelah pencatatan penghapusan string dilakukan ke dalam file log, maka file log ini akan ditutup menggunakan fclose.
```bash
void replace_string_in_file(const char *filepath) {
    FILE *fp = fopen(filepath, "r+");
    if (!fp) return;
```
Masuk ke fungsi kedua, yaitu replace string untuk mengganti string sesuai dengan ketentuan di soal. Pertama-tama, file.txt akan dibuka dengan akses read and write (r+).
```bash
fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);
char *data = (char *)malloc(fsize * 2); // Assume maximum expansion is less than double
    if (!data) {
        fclose(fp);
        return;
    }
    fread(data, 1, fsize, fp);
    data[fsize] = '\0';
```
Bagian ini berfungsi untuk mengambil dan menghitung ukuran file, lalu mengalokasikan memori yang tepat untuk file tersebut. Kemudian, konten dan isi dari file.txt akan dibaca menggunakan perintah fread. 
```bash
int changed = 0;
    char *replacements[][2] = {
        {"m4LwAr3", "[MALWARE]"},
        {"5pYw4R3", "[SPYWARE]"},
        {"R4nS0mWaR3", "[RANSOMWARE]"},
        {NULL, NULL}
    };

    char *result = malloc(fsize * 2); // Buffer for the result
    if (!result) {
        free(data);
        fclose(fp);
        return;
    }
    strcpy(result, data); // Start with the original content
```
Pada bagian kode ini, terdapat array bernama replacement yang berisi list-list string yang akan diganti beserta string penggantinya. Setelah itu, akan dialokasikan memori untuk variabel result. Variabel result digunakan untuk menyimpan isi file yang sudah dimodifikasi stringnya. 
```bash
    for (int i = 0; replacements[i][0] != NULL; i++) {
        char *pos = result;
        char *temp;
        while ((pos = strstr(pos, replacements[i][0])) != NULL) {
            temp = strdup(pos + strlen(replacements[i][0])); // Save the tail
            memcpy(pos, replacements[i][1], strlen(replacements[i][1]));
            strcpy(pos + strlen(replacements[i][1]), temp);
            pos += strlen(replacements[i][1]); // Move past the replacement
            free(temp);
            changed = 1;
        }
    }

    if (changed) {
        fseek(fp, 0, SEEK_SET);
        fwrite(result, 1, strlen(result), fp);
        truncate(filepath, strlen(result)); // Adjust the file size
        log_replacement(filepath);
    }
    free(data);
    free(result);
    fclose(fp);
}
```
Terdapat variabel pos yang berfungsi untuk menunjuk ke string yang akan diubah sesuai ketentuan soal, sedangkan variabel temp untuk menyimpan sisa string yang tidak diubah. Loop while berfungsi untuk mengganti string yang diinginkan sesuai dengan ketentuan di soal. Variabel pos akan menunjuk ke lokasi setelahnya, agar loop terus berjalan untuk mengganti string yang lain. Loop akan terus berjalan selama terdapat string yang masih belum diubah. 

Nilai pada variabel "changed" akan diset untuk berubah menjadi 1 untuk menandakan bahwa telah terjadi proses pergantian pada string. Jika terjadi perubahan, maka file akan ditulis kembali dengan isi file yang baru. Fungsi log_replacement dipanggil untuk mencatat perubahan dan menyimpannya ke dalam file virus.log.
```bash
void run_daemon(const char *dir_path) {
    pid_t pid = fork();
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    } else if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    umask(0);
    setsid();
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        DIR *d = opendir(dir_path);
        if (d) {
            struct dirent *dir;
            while ((dir = readdir(d)) != NULL) {
                if (dir->d_type == DT_REG) {
                    char path[1024];
                    sprintf(path, "%s/%s", dir_path, dir->d_name);
                    replace_string_in_file(path);
                }
            }
            closedir(d);
        }
        sleep(15); // wait for 15 seconds
    }
}
```
Potongan kode di atas berfungsi untuk menjalankan program secara daemon, yang didalamnya akan dilakukan pemanggilan terhadap fungsi replace string untuk melakukan pergantian pada string. Program akan di set untuk terus berjalan di background dengan jeda 15 detik.
```bash
int main(int argc, char **argv) {
     FILE *logFile = fopen(LOG_FILE, "a");
    if (logFile == NULL) {
        perror("Error creating log file");
        return EXIT_FAILURE;
    }
    fclose(logFile);
    
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    run_daemon(argv[1]);
    return EXIT_SUCCESS;
}
```
Potongan kode di atas adalah fungsi main. Pertama, file log akan dibuka terlebih dahulu dan akan dibuat file log baru jika belum terbuat sebelumnya. Selanjutnya, akan dilakukan pengecekan kondisi if. Jika argumen yang diberikan oleh user kurang dari 2, atau tidak memenuhi sesuai dengan ketentuan yang ada di soal (dapat menerima input path berupa 'argv'), maka program tidak akan berjalan. Sedangkan jika sudah memenuhi, maka akan dijalankan program virus.c secara daemon. 

#### Dokumentasi Pengerjaan
<img width="1440" alt="Screenshot 2024-04-27 at 07 25 26" src="https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/b2d2c8a3-70d1-4002-90ef-0199ab244854">

#### Kendala yang dialami
File virus.log sempat tidak terbuat saat sudah menjalankan program, karena perintah untuk membuka file dan membuatnya jika belum ada belum saya letakkan di fungsi main.
![kendala no1](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/304d58a5-cd81-4d9d-aa54-80a39500bbaa)

## Soal 2
> Ryan 5027231046

Dalam soal ini kita disuruh untuk membangun sebuah sistem manajemen file dan backup/restore yang dirancang untuk lingkungan Linux. Ini dimaksudkan untuk melakukan otomasi berbagai operasi file, termasuk mengunduh, mengekstrak, mendekripsi, mengubah nama, dan menghapus file. Selain itu, menyediakan fungsionalitas untuk backup dan restore file untuk memastikan integritas dan pemulihan data. Sistem ini dirancang untuk berjalan sebagai daemon, memungkinkan operasi di latar belakang tanpa intervensi pengguna.

### Fitur

* **Operasi File Otomatis**: Mengotomatiskan serangkaian operasi file termasuk mengunduh, mengekstrak, mendekripsi, mengubah nama, dan menghapus file.
* **Backup dan Restore**: Mampu melakukan backup dan restore file untuk memastikan integritas dan pemulihan data.
* **Pengelolaan Sinyal**: Merespons sinyal untuk beralih antara mode default, mode backup, dan mode restore.
* **Mode Daemon**: Berjalan sebagai daemon, memungkinkan operasi di latar belakang.
* **Penutupan yang Efisien**: Dapat ditutup dengan aman dan efisien untuk menghindari beban sistem.

### Memulai
#### Persyaratan

* Lingkungan Linux
* Kompiler C (misalnya, gcc)
* Pustaka C standar

#### Instalasi

* Kloning repositori atau unduh kode sumber.
* Kompilasi kode sumber menggunakan kompiler C. Misalnya, menggunakan gcc:

```sh
gcc -o management management.c
```

* Jalankan program yang dikompilasi.

### Penggunaan

Program ini dapat dijalankan dengan argumen baris perintah untuk menentukan mode operasi. Secara default, program ini berjalan dalam mode default. Untuk berjalan dalam mode backup atau restore, gunakan opsi -m diikuti oleh backup atau restore.

```sh
./management
./management -m backup
./management -m restore
```
### Mode default

Program ini akan secara otomatis masuk ke dalam mode default yang terdiri atas fungsi yang memanggil fungsi lain untuk melakukan tugas masing-masing. Sebagian besar dari sub-fungsi tersebut menggunakan `fork()` dan `exec()` sesuai keperluan. Fungsi yang terlibat sebagai berikut:

```c
void runDefault() {
    fileReset();
    fileDownload();
    fileUnzip();
    fileDecrypt();
    fileRename();
    fileDelete();
}
```

### Mode backup/restore

Program ini akan masuk ke dalam mode backup atau restore bila diberikan argumen yang sesuai di belakang opsi -m. Dalam mode backup, file terpindah dari library ke dalam folder backup. Sedangkan dalam mode restore, file terpindah dari folder backup kembali ke folder library. Fungsi yang terlibat sebagai berikut:

```c
case 1:
    runBackup();
    break;

case 2:
    runRestore();
    break;
```

### Sinyal

Inisiasi mode signal sebagai berikut:
```c
volatile sig_atomic_t mode = 0;

void set_runDefault(int sig) { 
    mode = 0; 
}

void initiate_backup_process(int sig) { 
    mode = 1; 
}

void start_restore_process(int sig) { 
    mode = 2; 
}

void terminate_program(int sig) { 
    exit(0);
}
```

Dan agar signal dapat dipanggil menggunakan:

```c
signal(SIGRTMIN, set_runDefault);
signal(SIGUSR1, initiate_backup_process);
signal(SIGUSR2, start_restore_process);
signal(SIGKILL, terminate_program);
```

Program ini merespons sinyal berikut:

* SIGRTMIN: Beralih ke mode default.
* SIGUSR1: Memulai proses backup.
* SIGUSR2: Memulai proses restore.
* SIGKILL: Menghentikan program.

Contoh penggunaan signal adalah dengan mengetik command pada terminal sebagai berikut.

```sh
# Format:
kill -<kode signal> <pid>
# Contoh:
kill -SIGRTMIN 13478
```

### Logging

Sistem kerja logging pada program ini menggunakan `fprintf` yang tersebar di seluruh fungsi penting yang terhubung dengan satu file log khusus (history.log). Berikut contoh panggilan log pada `fileDelete()`:

```c
// Inisiasi log (main)
getcwd(workingDirectory, sizeof(workingDirectory));
strcpy(pathToFile, workingDirectory);
strcpy(pathToLogFile, pathToFile);
strcat(pathToLogFile, "/history.log");

// Inisiasi variabel pada log (main)
username = (char*)malloc(1024);
username = getlogin();
runtime = time(NULL);
digitime = localtime(&runtime);

// Contoh panggilan log
FILE *file = fopen(pathToLogFile, "a"); 
if (file != NULL) {  
    fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully deleted.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
    fclose(file); 
}
```

### Dokumentasi

Screenshot dari keberhasilan seluruh fitur program ini adalah sebagai berikut:

![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/162680331/38d1dfc4-db17-40e0-a081-b43ace9ead4d)


#### Kendala yang dialami
sebelumnya masih menggunakan command system untuk membuat folder library dan backup, serta hanya selesai sampai download dan ekstrak file saja.


## Soal 3
> Rafa 5027231019
### Soal
#### Pak Heze adalah seorang admin yang baik. Beliau ingin membuat sebuah program admin yang dapat memantau para pengguna sistemnya. Bantulah Pak Heze untuk membuat program  tersebut!
#### a). Nama program tersebut dengan nama admin.c
#### b). Program tersebut memiliki fitur menampilkan seluruh proses yang dilakukan oleh seorang user dengan menggunakan command:
#### ./admin <user>
#### c). Program dapat memantau proses apa saja yang dilakukan oleh user. Fitur ini membuat program berjalan secara daemon dan berjalan terus menerus. Untuk menjalankan fitur ini menggunakan command: 
#### ./admin -m <user>
#### Dan untuk mematikan fitur tersebut menggunakan: 
#### ./admin -s <user>
#### Program akan mencatat seluruh proses yang dijalankan oleh user di file <user>.log dengan format:
#### [dd:mm:yyyy]-[hh:mm:ss]_pid-process_nama-process_GAGAL/JALAN
#### d). Program dapat menggagalkan proses yang dijalankan user setiap detik secara terus menerus dengan menjalankan: 
#### ./admin -c user
#### sehingga user tidak bisa menjalankan proses yang dia inginkan dengan baik. Fitur ini dapat dimatikan dengan command:
#### ./admin -a user
#### e). Ketika proses yang dijalankan user digagalkan, program juga akan melog dan menset log tersebut sebagai GAGAL. Dan jika di log menggunakan fitur poin c, log akan ditulis dengan JALAN

### Penyelesaian
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <sys/types.h>
	#include <string.h>
	#include <time.h>
	#include <signal.h>
	
	#define PID_FILE "daemon.pid"
	#define STATUS_FILE "status.txt"
	
	int daemon_pid = -1;
	
	void log_activity(char *user, char *activity, int success) {
	    FILE *fp;
	    char filename[50];
	    time_t current_time;
	    struct tm *local_time;
	    char timestamp[50];
	
	    time(&current_time);
	    local_time = localtime(&current_time);
	    strftime(timestamp, sizeof(timestamp), "%d:%m:%Y-%H:%M:%S", local_time);
	
	    snprintf(filename, sizeof(filename), "%s.log", user);
	    fp = fopen(filename, "a");
	
	    if (fp != NULL) {
	        fprintf(fp, "[%s]-%d-%s_%s\n", timestamp, getpid(), activity, success ? "JALAN" : "GAGAL");
	        fclose(fp);
	    } else {
	        printf("Failed to open log file for user %s.\n", user);
	    }
	}
	
	int read_status() {
	    FILE *status_file = fopen(STATUS_FILE, "r");
	    int blocked = 0;
	    if (status_file != NULL) {
	        fscanf(status_file, "%d", &blocked);
	        fclose(status_file);
	    }
	    return blocked;
	}
	
	void block_internet() {
	    system("iptables -A OUTPUT -d google.com -j DROP");
	}
	
	void allow_internet() {
	    system("iptables -D OUTPUT -d google.com -j DROP");
	}
	
	void monitor_activity(char *user) {
	    int blocked = read_status();
	    if (blocked) {
	        log_activity(user, "Monitoring activity", 0);
	        printf("Monitoring activity failed for user %s.\n", user);
	        return;
	    }
	
	    pid_t pid;
	    FILE *pid_file;
	
	    pid = fork();
	
	    if (pid == 0) {
	        setsid();
	        while (1) {
	            blocked = read_status();
	            if (blocked) {
	                break;
	            }
	            log_activity(user, "Monitoring activity", 1);
	            sleep(1);
	        }
	        exit(0);
	    } else if (pid > 0) {
	        daemon_pid = pid;
	        pid_file = fopen(PID_FILE, "w");
	        if (pid_file != NULL) {
	            fprintf(pid_file, "%d", daemon_pid);
	            fclose(pid_file);
	        }
	        printf("Monitoring started for user %s.\n", user);
	    } else {
	        printf("Fork failed.\n");
	    }
	}
	
	void stop_monitoring(char *user) {
	    int blocked = read_status();
	    if (blocked) {
	        log_activity(user, "Stopping monitoring", 0);
	        printf("Stopping monitoring failed for user %s.\n", user);
	        return;
	    }
	
	    FILE *pid_file;
	    int pid;
	
	    pid_file = fopen(PID_FILE, "r");
	    if (pid_file != NULL) {
	        fscanf(pid_file, "%d", &pid);
	        fclose(pid_file);
	        if (pid > 0) {
	            kill(pid, SIGTERM);
	            remove(PID_FILE);
	            daemon_pid = -1;
	            log_activity(user, "Stopping monitoring", 1);
	            printf("Monitoring stopped for user %s.\n", user);
	        } else {
	            printf("No running monitoring process found for user %s.\n", user);
	        }
	    } else {
	        printf("No running monitoring process found for user %s.\n", user);
	    }
	}
	
	void block_activity(char *user) {
	    FILE *status_file = fopen(STATUS_FILE, "w");
	    if (status_file != NULL) {
	        fprintf(status_file, "1");
	        fclose(status_file);
	        block_internet();
	        log_activity(user, "Blocking activity", 1);
	        printf("Activity blocked for user %s.\n", user);
	    } else {
	        printf("Failed to open status file.\n");
	    }
	}
	
	void allow_activity(char *user) {
	    FILE *status_file = fopen(STATUS_FILE, "w");
	    if (status_file != NULL) {
	        fprintf(status_file, "0");
	        fclose(status_file);
	        allow_internet(); // Add this line to allow internet access
	        log_activity(user, "Allowing activity", 1);
	        printf("Activity allowed for user %s.\n", user);
	    } else {
	        printf("Failed to open status file.\n");
	    }
	}
	
	void sigterm_handler(int signum) {
	    printf("Received SIGTERM signal. Exiting.\n");
	    exit(signum);
	}
	
	int main(int argc, char *argv[]) {
	    signal(SIGTERM, sigterm_handler);
	
	    if (argc < 3) {
	        printf("-m membuat program berjalan secara daemon dan berjalan terus menerus\n-s mematikan program yang berjalan secara daemon dan berjalan terus menerus\n-c menggagalkan proses yang dijalankan user setiap detik secara terus menerus\n-a mengembalikan akses untuk menjalankan user setiap detik secara terus menerus\n", argv[0]);
	        return 1;
	    }
	
	    char *option = argv[1];
	    char *user = argv[2];
	
	    if (strcmp(option, "-m") == 0) {
	        monitor_activity(user);
	    } else if (strcmp(option, "-s") == 0) {
	        stop_monitoring(user);
	    } else if (strcmp(option, "-c") == 0) {
	        block_activity(user);
	    } else if (strcmp(option, "-a") == 0) {
	        allow_activity(user);
	    } else {
	        printf("Invalid option.\n");
	        return 1;
	    }
	
	    return 0;
	}

### Penjelasan
#### - Pertama, kita bikin sebuah program yang bernama "admin.c" menggunakan:
	touch admin.c

#### - Kemudian, pada bagian (b), kita diminta untuk membuat agar program tersebut memiliki fitur menampilkan seluruh proses yang dilakukan oleh seorang user dengan menggunakan command ./admin <user>
	    if (argc < 3) {
	        printf("-m membuat program berjalan secara daemon dan berjalan terus menerus\n-s mematikan program yang berjalan secara daemon dan berjalan terus menerus\n-c menggagalkan proses yang dijalankan user setiap detik secara terus menerus\n-a mengembalikan akses untuk menjalankan user setiap detik secara terus menerus\n", argv[0]);
	        return 1;
	    }
##### Program ini memberikan opsi untuk memulai, menghentikan, memblokir, dan mengembalikan akses monitoring secara terus menerus. Hal ini memungkinkan pengguna untuk mengatur dan memantau aktivitas dengan fleksibilitas dalam lingkungan yang berjalan secara terus menerus. Jadi, saat kita melakukan ./admin <user>, ini akan menampilkan fitur-fitur yang dapat digunakan oleh pengguna.

#### Lalu, pada bagian (c), kita diminta untuk membuat program berjalan secara daemon dan berjalan terus menerus. Untuk menjalankan fitur ini menggunakan command ./admin -m <user> dan untuk mematikan fitur tersebut menggunakan ./admin -s <user>. KIta diminta juga agar program mencatat seluruh proses yang dijalankan oleh user di file <user>.log dengan format [dd:mm:yyyy]-[hh:mm:ss]_pid-process_nama-process_GAGAL/JALAN.
	void log_activity(char *user, char *activity, int success) {
	    FILE *fp;
	    char filename[50];
	    time_t current_time;
	    struct tm *local_time;
	    char timestamp[50];
	
	    time(&current_time);
	    local_time = localtime(&current_time);
	    strftime(timestamp, sizeof(timestamp), "%d:%m:%Y-%H:%M:%S", local_time);
	
	    snprintf(filename, sizeof(filename), "%s.log", user);
	    fp = fopen(filename, "a");
	
	    if (fp != NULL) {
	        fprintf(fp, "[%s]-%d-%s_%s\n", timestamp, getpid(), activity, success ? "JALAN" : "GAGAL");
	        fclose(fp);
	    } else {
	        printf("Failed to open log file for user %s.\n", user);
	    }
	}	
 
 	void monitor_activity(char *user) {
	    int blocked = read_status();
	    if (blocked) {
	        log_activity(user, "Monitoring activity", 0);
	        printf("Monitoring activity failed for user %s.\n", user);
	        return;
	    }
	
	    pid_t pid;
	    FILE *pid_file;
	
	    pid = fork();
	
	    if (pid == 0) {
	        setsid();
	        while (1) {
	            blocked = read_status();
	            if (blocked) {
	                break;
	            }
	            log_activity(user, "Monitoring activity", 1);
	            sleep(1);
	        }
	        exit(0);
	    } else if (pid > 0) {
	        daemon_pid = pid;
	        pid_file = fopen(PID_FILE, "w");
	        if (pid_file != NULL) {
	            fprintf(pid_file, "%d", daemon_pid);
	            fclose(pid_file);
	        }
	        printf("Monitoring started for user %s.\n", user);
	    } else {
	        printf("Fork failed.\n");
	    }
	}
	
	void stop_monitoring(char *user) {
	    int blocked = read_status();
	    if (blocked) {
	        log_activity(user, "Stopping monitoring", 0);
	        printf("Stopping monitoring failed for user %s.\n", user);
	        return;
	    }
	
	    FILE *pid_file;
	    int pid;
	
	    pid_file = fopen(PID_FILE, "r");
	    if (pid_file != NULL) {
	        fscanf(pid_file, "%d", &pid);
	        fclose(pid_file);
	        if (pid > 0) {
	            kill(pid, SIGTERM);
	            remove(PID_FILE);
	            daemon_pid = -1;
	            log_activity(user, "Stopping monitoring", 1);
	            printf("Monitoring stopped for user %s.\n", user);
	        } else {
	            printf("No running monitoring process found for user %s.\n", user);
	        }
	    } else {
	        printf("No running monitoring process found for user %s.\n", user);
	    }
	}
##### Pada program ini, fungsi log_activity digunakan untuk mencatat aktivitas pengguna dalam sebuah file log dengan format waktu, aktivitas, dan status. Fungsi monitor_activity mengawasi aktivitas pengguna dengan menggunakan proses daemon. Jika ada blok pada status, fungsi ini akan mencatat kegagalan monitoring. Fungsi stop_monitoring digunakan untuk menghentikan proses monitoring yang sedang berjalan, mencatatnya dalam log jika berhasil, dan memberikan pesan kesalahan jika gagal.

#### - Selanjutnya, pada bagian (d), kita diminta agar program dapat menggagalkan proses yang dijalankan user setiap detik secara terus menerus dengan menjalankan ./admin -c user sehingga user tidak bisa menjalankan proses yang dia inginkan dengan baik. Fitur ini dapat dimatikan dengan command ./admin -a user.
	int read_status() {
	    FILE *status_file = fopen(STATUS_FILE, "r");
	    int blocked = 0;
	    if (status_file != NULL) {
	        fscanf(status_file, "%d", &blocked);
	        fclose(status_file);
	    }
	    return blocked;
	}
	
	void block_internet() {
	    system("iptables -A OUTPUT -d google.com -j DROP");
	}
	
	void allow_internet() {
	    system("iptables -D OUTPUT -d google.com -j DROP");
	}
 	void block_activity(char *user) {
	    FILE *status_file = fopen(STATUS_FILE, "w");
	    if (status_file != NULL) {
	        fprintf(status_file, "1");
	        fclose(status_file);
	        block_internet();
	        log_activity(user, "Blocking activity", 1);
	        printf("Activity blocked for user %s.\n", user);
	    } else {
	        printf("Failed to open status file.\n");
	    }
	}
	
	void allow_activity(char *user) {
	    FILE *status_file = fopen(STATUS_FILE, "w");
	    if (status_file != NULL) {
	        fprintf(status_file, "0");
	        fclose(status_file);
	        allow_internet(); // Add this line to allow internet access
	        log_activity(user, "Allowing activity", 1);
	        printf("Activity allowed for user %s.\n", user);
	    } else {
	        printf("Failed to open status file.\n");
	    }
	}
##### Pada program ini, fungsi read_status digunakan untuk membaca status dari file STATUS_FILE, yang menunjukkan apakah aktivitas pengguna sedang diblokir atau tidak. Fungsi block_internet digunakan untuk memblokir akses internet ke google.com menggunakan iptables. Fungsi allow_internet digunakan untuk mengizinkan kembali akses internet ke google.com dengan menghapus aturan iptables yang sebelumnya dibuat. Fungsi block_activity digunakan untuk memblokir aktivitas pengguna dengan menulis nilai "1" ke file status, menjalankan fungsi block_internet, dan mencatat aktivitas tersebut dalam log. Fungsi allow_activity digunakan untuk mengizinkan kembali aktivitas pengguna dengan menulis nilai "0" ke file status, menjalankan fungsi allow_internet, dan mencatat aktivitas tersebut dalam log.

#### - Terakhir, pada bagian (e), kita diminta agar ketika proses yang dijalankan user digagalkan, program juga akan melog dan menset log tersebut sebagai GAGAL. Dan jika di log menggunakan fitur poin c, log akan ditulis dengan JALAN.
	    if (fp != NULL) {
	        fprintf(fp, "[%s]-%d-%s_%s\n", timestamp, getpid(), activity, success ? "JALAN" : "GAGAL");
	        fclose(fp);
	    } else {
	        printf("Failed to open log file for user %s.\n", user);
	    }
	}
##### Program ini bertugas untuk mencatat aktivitas pengguna ke dalam file log. Jika file log berhasil dibuka (fp != NULL), fungsi fprintf akan menulis data aktivitas ke dalam file log. Data yang ditulis meliputi timestamp (waktu saat aktivitas terjadi), PID proses, deskripsi aktivitas, dan status keberhasilan aktivitas ("JALAN" atau "GAGAL" berdasarkan parameter success). Setelah penulisan selesai, file log akan ditutup dengan fungsi fclose(fp). Jika file log gagal dibuka, maka program akan mencetak pesan kesalahan menggunakan printf.

### Revisi
#### Pada saat demo, ada sedikit kesalahan dimana saat saya melakukan ./admin -c <user> untuk memblokir akses pengguna, saya tetap bisa mengakses internet dan seharusnya itu tidak bisa. Jadi disini saya melakukan sedikit perbaikan, yaitu dengan menambahkan:
	void block_internet() {
	    system("iptables -A OUTPUT -d google.com -j DROP");
	}
	
	void allow_internet() {
	    system("iptables -D OUTPUT -d google.com -j DROP");
	}
##### Pada program ini, fungsi block_internet digunakan untuk memblokir akses internet ke google.com dengan menambahkan aturan pada iptables menggunakan perintah sistem. Sedangkan fungsi allow_internet digunakan untuk mengizinkan kembali akses internet ke google.com dengan menghapus aturan yang sebelumnya ditambahkan pada iptables menggunakan perintah sistem.

### Dokumentasi
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/9ff82512-9c2b-4d9c-803a-bd17f80381f9)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/2b83dd6f-ad5e-4967-b589-5fb6c7030d5a)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/f497aae0-04d0-4b2b-974a-582965e30ffa)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/bee3f740-b05b-4166-a7a6-c279b49bc01a)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/27f9429e-1925-4882-b5e1-011ac71205c9)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/5819060d-0e07-4723-80ec-7001c22ff5f3)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/59f205ef-5ee3-44e8-8659-450c6445538f)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/b24e1b3b-4286-4ae4-a812-b3beb9d760ea)
![image](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/151918510/85719c9f-9531-4333-b0a0-e17afb7d743a)

## Soal 4
> Nayla 5027231054

Soal nomor 4 meminta kita untuk membuat sebuah program bernama virus.c yang dapat membuka lebih dari satu aplikasi dan lebih dari satu instance untuk masing-masing aplikasi. Adapun ketentuan untuk input perintahnya adalah :
a. ./setup -o firefox 2 wireshark 2
Program akan membuka 2 jendela aplikasi firefox dan 2 jendela aplikasi wireshark.
Program juga dapat membuka aplikasi dengan menggunakan file konfigurasi dengan menggunakan 
b. ./setup -f file.conf 
Format file konfigurasi dibebaskan, namun pastikan dapat menjalankan fitur dari poin 2.
Contoh isi file.conf:
Firefox 2
Wireshark 3
Ketika menjalankan command contoh, program akan membuka 2 jendela aplikasi firefox dan 3 jendela aplikasi wireshark.

Selain itu, program juga dapat mematikan aplikasi yang sedang dibuka. Ketentuannya adalah :
./setup -k
Program juga dapat mematikan aplikasi yang dijalankan sesuai dengan file konfigurasi. 
Contohnya: 
./setup -k file.conf 
Command ini hanya mematikan aplikasi yang dijalankan dengan 
./setup -f file.conf

### Penyelesaian
```bash
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
```
Adalah beberapa libraries yang digunakan dalam kode ini. Fungsi dari masing-masing library adalah:
* #include <stdio.h>: Mendefinisikan fungsi-fungsi standar untuk input dan output.
* #include <stdlib.h>: Mendefinisikan fungsi-fungsi standar untuk alokasi memori, konversi angka, dan fungsi umum lainnya.
* #include <string.h>: Mendefinisikan fungsi-fungsi standar untuk manipulasi string.
* #include <unistd.h>: Mendefinisikan fungsi-fungsi standar untuk sistem operasi POSIX (Portable Operating System Interface).
* #include <sys/wait.h>: Mendefinisikan fungsi-fungsi untuk mengelola proses yang sedang menunggu.
* #include <signal.h>: Mendefinisikan fungsi-fungsi untuk penanganan sinyal.
```bash
#define MAX_APPS 20
#define MAX_NAME_LENGTH 100
```
Mendefinisikan maksimum jumlah aplikasi yang dapat dibuka dan panjang maksimum nama aplikasi yang akan dibuka.
```bash
typedef struct {
    char config_name[MAX_NAME_LENGTH];
    int pids[MAX_APPS * 3];
    int pid_count;
} AppRecord;
```
Mendefinisikan struktur data AppRecord yang menyimpan data mengenai konfigurasi aplikasi yang akan dibuka.
```bash
void launch_applications(char *apps[], int counts[], int n, const char* source) {
    pid_t pid;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < counts[i]; j++) {
            pid = fork();
            if (pid == 0) {
                // Child process
                execlp(apps[i], apps[i], NULL);
                perror("Failed to start application");
                exit(1);
            }
        }
    }
}
```
* pid_t pid;: Deklarasi variabel pid untuk menyimpan ID proses.
* for (int i = 0; i < n; i++) {: Looping melalui setiap aplikasi yang akan diluncurkan.
* for (int j = 0; j < counts[i]; j++) {: Looping sebanyak instance yang akan diluncurkan dari aplikasi saat ini.
* pid = fork();: Memulai proses baru dengan fork(). Proses baru akan menjadi salinan identik dari proses induk.
* if (pid == 0) {: Periksa apakah ini adalah proses anak. Bagian dalam kondisi ini hanya akan dieksekusi oleh proses anak.
* execlp(apps[i], apps[i], NULL);: Gantikan image proses saat ini dengan aplikasi yang diinginkan menggunakan execlp(). Argument pertama adalah nama aplikasi, yang kedua adalah argumen yang akan diberikan kepada aplikasi (NULL karena tidak ada argumen tambahan).
```bash
void kill_applications_from_command_line() {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("pkill", "pkill", "firefox", NULL);
        exit(0);
    } else if (pid < 0) {
        perror("Error: Fork failed");
    } else {
        wait(NULL);
    }

    pid = fork();
    if (pid == 0) {
        execlp("pkill", "pkill", "gedit", NULL);
        exit(0);
    } else if (pid < 0) {
        perror("Error: Fork failed");
    } else {
        wait(NULL);
    }
}
```
* pid_t pid = fork();: Memanggil fungsi fork() untuk membuat proses baru. Proses baru akan memiliki salinan identik dari proses pemanggil (proses induk). Nilai pid akan berbeda di proses induk dan anak. Jika fork() berhasil, nilai pid akan positif dan akan bernilai 0 di proses anak.
* if (pid == 0) { ... } else if (pid < 0) { ... } else { ... }: Memeriksa nilai pid untuk menentukan apakah proses yang baru dibuat adalah proses anak, proses induk, atau jika fork() gagal.
* execlp("pkill", "pkill", "firefox", NULL);: Di dalam blok if (pid == 0), proses anak akan menjalankan perintah pkill untuk menutup aplikasi firefox. Fungsi execlp() digunakan untuk menjalankan program dengan menyediakan argumen baris perintah. Argumen pertama adalah nama program yang akan dijalankan (pkill), diikuti oleh argumen-argumen yang akan diteruskan ke program tersebut. NULL menandakan akhir dari daftar argumen.
* exit(0);: Setelah menjalankan perintah untuk menutup aplikasi, proses anak keluar (exit) dengan kode keluar 0, menandakan bahwa proses tersebut berhasil.
* else if (pid < 0) { perror("Error: Fork failed"); }: Jika fork() gagal, pesan kesalahan akan dicetak menggunakan perror().
* else { wait(NULL); }: Di dalam blok else, proses induk menunggu (wait()) hingga proses anak selesai. Ini memastikan bahwa proses induk tidak berlanjut sampai proses anak selesai dieksekusi.
* Kode serupa berlaku untuk menutup aplikasi gedit dan libreoffice. Proses akan bercabang lagi untuk menjalankan perintah pkill.
```bash
void kill_applications_from_config(const char* source) {
    FILE *file = fopen(source, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    char app[MAX_NAME_LENGTH];
    while (fscanf(file, "%s", app) == 1) {
        pid_t pid = fork();
        if (pid == 0) {
            execlp("pkill", "pkill", app, NULL);
            exit(0);
        } else if (pid < 0) {
            perror("Error: Fork failed");
        } else {
            wait(NULL);
        }
    }
    fclose(file);
}
```
* FILE *file = fopen(source, "r");: Fungsi fopen digunakan untuk membuka file dengan mode "r" yang berarti untuk membaca (read). source adalah parameter yang berisi nama file konfigurasi yang akan dibaca. 
* char app[MAX_NAME_LENGTH];: Membuat sebuah array karakter app dengan panjang maksimum MAX_NAME_LENGTH. Ini akan digunakan untuk menyimpan nama aplikasi yang dibaca dari file konfigurasi.
* while (fscanf(file, "%s", app) == 1) {: Menggunakan loop while untuk membaca setiap baris dari file konfigurasi. fscanf membaca string dari file dan menyimpannya di dalam app. Loop akan terus berjalan selama fscanf berhasil membaca satu elemen (dalam hal ini, satu nama aplikasi).
* pid_t pid = fork();: Membuat proses baru dengan fork(). Proses baru ini akan digunakan untuk menutup aplikasi yang sesuai dengan nama yang dibaca.
* if (pid == 0) {: Ini adalah cabang yang dijalankan oleh child process (proses anak) setelah berhasil di-fork.
* execlp("pkill", "pkill", app, NULL);: Memanggil execlp untuk menjalankan perintah pkill dari shell. pkill adalah perintah yang digunakan untuk menutup proses berdasarkan nama aplikasi. Argument app digunakan untuk menentukan nama aplikasi yang akan ditutup. Argument NULL menandakan akhir dari daftar argumen.
* exit(0);: Keluar dari proses anak setelah menjalankan perintah pkill. Ini menghindari proses anak untuk melanjutkan ke loop selanjutnya.
* else if (pid < 0) { perror("Error: Fork failed"); }: Ini adalah cabang yang dijalankan jika proses gagal di-fork. Pesan kesalahan dicetak menggunakan perror.
* else { wait(NULL); }: Ini adalah cabang yang dijalankan oleh proses parent (proses induk). Ini menunggu proses anak selesai dengan wait(NULL). Ini memastikan bahwa proses parent tidak melanjutkan eksekusi sebelum proses anak selesai.
* fclose(file);: Setelah selesai membaca file konfigurasi, file ditutup dengan fclose.
```bash
void read_config_and_launch(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char app[MAX_NAME_LENGTH];
    int count;
    char *apps[MAX_APPS];
    int counts[MAX_APPS];
    int n = 0;

    while (fscanf(file, "%s %d", app, &count) == 2) {
        apps[n] = strdup(app);
        counts[n] = count;
        n++;
    }

    fclose(file);
    launch_applications(apps, counts, n, filename);

    for (int i = 0; i < n; i++) 
        free(apps[i]);
}
```
* FILE *file = fopen(filename, "r");: Fungsi fopen digunakan untuk membuka file dengan nama yang diberikan dalam mode "r" (artinya, membaca). Hasilnya disimpan dalam pointer file. Jika pembukaan file gagal, fopen akan mengembalikan NULL.
* char app[MAX_NAME_LENGTH]; int count;: Variabel lokal app digunakan untuk menyimpan nama aplikasi, sedangkan variabel count digunakan untuk menyimpan jumlah instance aplikasi yang akan diluncurkan.
* char *apps[MAX_APPS]; int counts[MAX_APPS]; int n = 0;: apps adalah array pointer ke string yang akan menyimpan nama aplikasi, dan counts adalah array integer yang akan menyimpan jumlah instance dari masing-masing aplikasi. n adalah jumlah aplikasi yang telah dibaca dari file.
* while (fscanf(file, "%s %d", app, &count) == 2) { ... }: Melakukan loop untuk membaca baris demi baris dari file konfigurasi. Setiap baris terdiri dari nama aplikasi dan jumlah instance, dipisahkan oleh spasi. fscanf membaca dua token (nama aplikasi dan jumlah instance) dari setiap baris, dan loop berlanjut selama fscanf berhasil membaca keduanya.
* apps[n] = strdup(app); counts[n] = count; n++;: Setiap kali sebuah baris dibaca dari file, nama aplikasi disalin ke dalam array apps menggunakan strdup (untuk menghindari masalah dengan memori yang diatur ulang), dan jumlah instance disimpan di array counts. n ditambahkan setiap kali baris berhasil dibaca.
* fclose(file);: Setelah semua baris dibaca, file ditutup dengan menggunakan fclose.
* launch_applications(apps, counts, n, filename);: Setelah konfigurasi dibaca, fungsi launch_applications dipanggil untuk meluncurkan aplikasi sesuai dengan konfigurasi yang dibaca dari file.
* for (int i = 0; i < n; i++) free(apps[i]);: Setelah semua aplikasi diluncurkan, memori yang dialokasikan untuk menyimpan nama aplikasi dengan menggunakan strdup harus dibebaskan kembali dengan free untuk menghindari kebocoran memori.
```bash
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s -o <app1> <num1> ... | -f <config_file> | -k [config_file]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-o") == 0) {
        char *apps[MAX_APPS];
        int counts[MAX_APPS];
        int n = 0;
        
        for (int i = 2; i < argc; i += 2) {
            apps[n] = argv[i];
            counts[n] = atoi(argv[i + 1]);
            n++;
        }

        launch_applications(apps, counts, n, "command-line");
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
        read_config_and_launch(argv[2]);
    } else if (strcmp(argv[1], "-k") == 0) {
        if (argc == 3) {
            kill_applications_from_config(argv[2]);
        } else {
            kill_applications_from_command_line();
        }
    } else {
        fprintf(stderr, "Invalid command or arguments\n");
        return 1;
    }

    return 0;
}
```
* if (argc < 2) { ... }: Memeriksa apakah jumlah argumen yang diberikan kurang dari 2. Jika iya, program akan mencetak pesan penggunaan yang menjelaskan cara menggunakan program dan kemudian keluar dengan kode kesalahan 1.
* if (strcmp(argv[1], "-o") == 0) { ... }: Memeriksa apakah argumen pertama (argv[1]) adalah -o, yang menandakan bahwa pengguna ingin meluncurkan aplikasi dari baris perintah dengan argumen berikutnya berisi nama aplikasi dan jumlahnya. Jika benar, program akan melakukan langkah-langkah berikut:a. Membuat array apps dan counts untuk menyimpan nama aplikasi dan jumlahnya.b. Melakukan iterasi melalui argumen berikutnya (argv[i]) dimulai dari indeks 2, yang merupakan nama aplikasi.c. Setiap aplikasi dan jumlahnya ditambahkan ke dalam array apps dan counts.d. Memanggil fungsi launch_applications dengan argumen yang sesuai untuk meluncurkan aplikasi.
* else if (strcmp(argv[1], "-f") == 0 && argc == 3) { ... }: Memeriksa apakah argumen pertama adalah -f dan jumlah total argumen adalah 3. Jika ya, ini menunjukkan bahwa pengguna ingin membaca konfigurasi aplikasi dari file. Program akan memanggil fungsi read_config_and_launch dengan argumen yang sesuai untuk membaca file konfigurasi dan meluncurkan aplikasi sesuai dengan konfigurasi tersebut.
* else if (strcmp(argv[1], "-k") == 0) { ... }: Memeriksa apakah argumen pertama adalah -k, yang menandakan bahwa pengguna ingin menutup aplikasi. Jika ya, program akan memeriksa jumlah argumen tambahan.a. Jika jumlah argumen adalah 3, ini menunjukkan bahwa pengguna ingin menutup aplikasi yang ditentukan dalam file konfigurasi. Program akan memanggil kill_applications_from_config dengan argumen yang sesuai.b. Jika jumlah argumen tidak sama dengan 3, ini menunjukkan bahwa pengguna ingin menutup aplikasi tertentu (firefox dan gedit) dari baris perintah. Program akan memanggil kill_applications_from_command_line.
* else { ... }: Jika argumen tidak cocok dengan opsi yang valid, program akan mencetak pesan kesalahan dan keluar dengan kode kesalahan 1.

#### Dokumentasi Pengerjaan
<img width="1440" alt="Screenshot 2024-04-27 at 19 51 46" src="https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/0122e435-1b3e-4e0b-aa2e-c9d39fcdbd93">
<img width="1440" alt="Screenshot 2024-04-27 at 19 50 17" src="https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/ce3270c8-5a3f-4d0c-af6b-c0fa5359da8d">

#### Revisi
* Masih belum bisa mematikan aplikasi, baik yang dibuka melalui command line ataupun dibuka melalui file config.
* Code sebelum revisi
  ```bash
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <sys/wait.h>
  #include <signal.h>
  #define MAX_APPS 20

  typedef struct {
    char config_name[100];
    int pids[MAX_APPS*3];
    int pid_count;
  } AppRecord;

  AppRecord records[MAX_APPS];
  int record_count = 0;

  void launch_applications(char *apps[], int counts[], int n, const char* source) {
    int index = -1;
    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].config_name, source) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        index = record_count++;
        strcpy(records[index].config_name, source);
        records[index].pid_count = 0;
    }

    pid_t pid;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < counts[i]; j++) {
            pid = fork();
            if (pid == 0) {
                // Child process
                execlp(apps[i], apps[i], NULL);
                perror("Failed to start application");
                exit(1);
            } else if (pid > 0) {
                // Store the PID in appropriate record
                records[index].pids[records[index].pid_count++] = pid;
            }
        }
    }
  }

  void kill_applications_from_command_line() {
    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].config_name, "command-line") == 0) {
            for (int j = 0; j < records[i].pid_count; j++) {
                kill(records[i].pids[j], SIGTERM);
            }
            records[i].pid_count = 0;  // Reset PID count for this record
            printf("Applications from command-line have been terminated.\n");
            return;
        }
    }
    printf("No running applications found from command-line.\n");
  }

  void kill_applications_from_config(const char* source) {
    FILE *file = fopen(source, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    int found = 0;
    for (int i = 0; i < record_count; i++) {
        if (strcmp(records[i].config_name, source) == 0) {
            for (int j = 0; j < records[i].pid_count; j++) {
                kill(records[i].pids[j], SIGTERM);
            }
            records[i].pid_count = 0;  // Reset PID count for this record
            found = 1;
            printf("Applications from %s have been terminated.\n", source);
        }
    }

    fclose(file);

    if (!found) {
        printf("No running applications found from %s.\n", source);
    }
  }

  void read_config_and_launch(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char app[100];
    int count;
    char *apps[MAX_APPS];
    int counts[MAX_APPS];
    int n = 0;

    while (fscanf(file, "%s %d", app, &count) == 2) {
        apps[n] = strdup(app);
        counts[n] = count;
        n++;
    }

    fclose(file);
    launch_applications(apps, counts, n, filename);

    for (int i = 0; i < n; i++) free(apps[i]);
  }

  int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s -o <app1> <num1> ... | -f <config_file> | -k [config_file]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-o") == 0) {
        char *apps[MAX_APPS];
        int counts[MAX_APPS];
        int n = 0;
        
        for (int i = 2; i < argc; i += 2) {
            apps[n] = argv[i];
            counts[n] = atoi(argv[i + 1]);
            n++;
        }

        launch_applications(apps, counts, n, "command-line");
    } else if (strcmp(argv[1], "-f") == 0 && argc == 3) {
        read_config_and_launch(argv[2]);
    } else if (strcmp(argv[1], "-k") == 0) {
        if (argc == 3) {
            kill_applications_from_config(argv[2]);
        } else {
            kill_applications_from_command_line();
        }
    } else {
        fprintf(stderr, "Invalid command or arguments\n");
        return 1;
    }

    return 0;
  }
  ```
  * Pada bagian fungsi untuk mematikan aplikasi, awalnya saya menggunakan kill sigterm. Tetapi pada akhirnya saya menggunakan pkill untuk mematikan aplikasi. Dokumentasi error untuk kode yang lama :
<img width="1440" alt="kendala no4 (gabisa kill)" src="https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/b79adce6-f24b-4409-98a4-83c0193de7cb">
Terdapat pesan "No running application", padahal aplikasi yang diinginkan sedang terbuka.

#### Kendala yang dialami
Kendala saat menggunakan kode baru yang sudah direvisi :
<img width="1440" alt="Screenshot 2024-04-27 at 19 56 30" src="https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/8b763ce8-560c-43db-a417-3fcfe1ee5c0c">
Terdapat pesan exiting due to channel error saat ingin mematikan aplikasi yang dibuka dari command line
<img width="1440" alt="Screenshot 2024-04-27 at 19 59 22" src="https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/33895949-31c2-40a0-bc5f-eb10d298a940">
Ada masalah saat ingin mematikan aplikasi yang dibuka dari file config

![kendala no1](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/e16a8660-b629-444a-9462-06ac27785576)# Laporan Resmi Praktikum Sisop-2-2024-MH-IT01

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

### Dokumentasi Pengerjaan
<img width="1440" alt="Screenshot 2024-04-27 at 07 25 26" src="https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/b2d2c8a3-70d1-4002-90ef-0199ab244854">

### Kendala yang dialami
File virus.log sempat tidak terbuat saat sudah menjalankan program, karena perintah untuk membuka file dan membuatnya jika belum ada belum saya letakkan di fungsi main.
![kendala no1](https://github.com/nyy223/Sisop-2-2024-MH-IT01/assets/80509033/304d58a5-cd81-4d9d-aa54-80a39500bbaa)


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

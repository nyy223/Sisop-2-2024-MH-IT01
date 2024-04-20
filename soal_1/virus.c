#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_FILE "virus.log"

//fungsi untuk menentukan isi dan format nama dari file virus.log
void log_replacement(const char *filename) {
    FILE *logFile = fopen(LOG_FILE, "a");
    if (logFile == NULL) {
        perror("Error opening log file");
        return;
    }

    time_t now;
    struct tm *timeinfo;
    char buffer[80];

    time(&now);
    timeinfo = localtime(&now);

    strftime(buffer, sizeof(buffer), "[%d-%m-%Y][%H:%M:%S]", timeinfo);
    fprintf(logFile, "%s Suspicious string at %s successfully replaced!\n", buffer, filename);
    fclose(logFile);
}

//fungsi untuk merubah string sesuai ketentuan di soal
void replace_string_in_file(const char *filepath) {
    FILE *fp = fopen(filepath, "r+");
    if (!fp) return;

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    char *data = (char *)malloc(fsize * 2); 
    if (!data) {
        fclose(fp);
        return;
    }
    fread(data, 1, fsize, fp);
    data[fsize] = '\0';

    int changed = 0;
    char *replacements[][2] = {
        {"m4LwAr3", "[MALWARE]"},
        {"5pYw4R3", "[SPYWARE]"},
        {"R4nS0mWaR3", "[RANSOMWARE]"},
        {NULL, NULL}
    };

    char *result = malloc(fsize * 2); 
    if (!result) {
        free(data);
        fclose(fp);
        return;
    }
    strcpy(result, data); 

    for (int i = 0; replacements[i][0] != NULL; i++) {
        char *pos = result;
        char *temp;
        while ((pos = strstr(pos, replacements[i][0])) != NULL) {
            temp = strdup(pos + strlen(replacements[i][0])); 
            memcpy(pos, replacements[i][1], strlen(replacements[i][1]));
            strcpy(pos + strlen(replacements[i][1]), temp);
            pos += strlen(replacements[i][1]); 
            free(temp);
            changed = 1;
        }
    }

    if (changed) {
        fseek(fp, 0, SEEK_SET);
        fwrite(result, 1, strlen(result), fp);
        truncate(filepath, strlen(result)); 
        log_replacement(filepath);
    }

    free(data);
    free(result);
    fclose(fp);
}

//fungsi untuk menjalankan program secara daemon
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
        sleep(15); 
    }
}

//fungsi main
int main(int argc, char **argv) {
  //membuat file virus.log jika belum terbuat
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

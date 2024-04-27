#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>

char *username;
time_t runtime;
struct tm *digitime;

char workingDirectory[1024];
char pathToFile[1024];
char pathToLogFile[1024];

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

void runBackup()
{
    char path[1024];
    strcpy(path, workingDirectory);
    strcat(path, "/library/");
    char backup_path[1024];
    strcpy(backup_path, path);
    strcat(backup_path, "backup/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    if (pid == 0)
    {
        struct stat st = {0};
        if (stat(backup_path, &st) == 0) exit(EXIT_SUCCESS);

        char *cmd = "/bin/mkdir";
        char *argv[3] = { "mkdir", backup_path, NULL };
        execv(cmd, argv);
        exit(EXIT_SUCCESS); 
    }

    else
    {
        int status;
        wait(&status);
    }

    while ((entry = readdir(dir)) != NULL) 
    {
        char *filename = entry->d_name;

        if (strstr(filename, "m0V3") == NULL) continue;

        if (filename[0] == '.') continue;

        FILE *file = fopen(pathToLogFile, "a"); 
        if (file != NULL) 
        {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully moved to backup.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }

        char file_path[1024];
        strcpy(file_path, path);
        strcat(file_path, filename);
        char file_backup_path[1024];
        strcpy(file_backup_path, backup_path);
        strcat(file_backup_path, filename);

        pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0)
        {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", file_path, file_backup_path, NULL };
            execv(cmd, argv);
            exit(EXIT_SUCCESS); 
        }

        else
        {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void runRestore() {
    char path[1024];
    strcpy(path, pathToFile);
    strcat(path, "/library/");
    char backup_path[1024];
    strcpy(backup_path, path);
    strcat(backup_path, "backup/");

    DIR *dir = opendir(backup_path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;

        if (strstr(filename, "m0V3") == NULL) {
            continue;
        }

        if (filename[0] == '.') {
            continue;
        }

        FILE *file = fopen(pathToLogFile, "a"); 
        if (file != NULL) {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully restored from backup.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }

        char file_path[1024];
        strcpy(file_path, path);
        strcat(file_path, filename);
        char file_backup_path[1024];
        strcpy(file_backup_path, backup_path);
        strcat(file_backup_path, filename);

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0) {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", file_backup_path, file_path, NULL };
            execvp(cmd, argv);
            return;
        }

        else {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void fileReset() {
    char folder_path[1024];
    strcpy(folder_path, pathToFile);
    strcat(folder_path, "/library");
    char zip_path[1024];
    strcpy(zip_path, pathToFile);
    strcat(zip_path, "/library.zip");

    pid_t pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    else if (pid == 0) {
        char *cmd = "/bin/rm";
        char *argv[4] = { "rm", "-rf", folder_path, NULL };
        execvp(cmd, argv);
    }

    else {
        int status;
        wait(&status);

        pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0) {
            char *cmd = "/bin/rm";
            char *argv[3] = { "rm", zip_path, NULL };
            execvp(cmd, argv);
        }

        else {
            int status;
            wait(&status);
        }
    }
}

void fileDownload() {
    pid_t pid = fork();

    char download_path[1024];
    strcpy(download_path, pathToFile);

    if (pid < 0) exit(EXIT_FAILURE);

    else if (pid == 0) {
        char *cmd = "/bin/wget";
        char *argv[7] = { "wget", "--content-disposition", "--no-check-certificate", "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup", "-P", download_path, NULL };
        execvp(cmd, argv);
    }

    else {
        int status;
        wait(&status);
    }
}

void fileUnzip() {
    pid_t pid = fork();

    char unzip_path[1024];
    strcpy(unzip_path, pathToFile);
    strcat(unzip_path, "/library.zip");

    if (pid < 0) exit(EXIT_FAILURE);

    else if (pid == 0) {
        char *cmd = "/bin/unzip";
        char *argv[5] = { "unzip", unzip_path, "-d", pathToFile, NULL };
        execvp(cmd, argv);
    }

    else {
        int status;
        wait(&status);
    }
}

char rot19Algorithm(char c) {
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A';
        return (c - base + 7) % 26 + base;
    } return c;
}

void fileDecrypt() {
    char path[1024];
    strcpy(path, pathToFile);
    strcat(path, "/library/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        char new_filename[1024];

        if (isdigit(filename[0])) continue;

        if (filename[0] == '.') continue;

        int nullterminator;
        for (int i = 0; i < strlen(filename); i++) {
            new_filename[i] = rot19Algorithm(filename[i]);
            nullterminator = i + 1;
        } new_filename[nullterminator] = '\0';
        
        char old_path[1024];
        strcpy(old_path, path);
        strcat(old_path, filename);
        char new_path[1024];
        strcpy(new_path, path);
        strcat(new_path, new_filename);

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0) {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", old_path, new_path, NULL };
            execvp(cmd, argv);
            return;
        }

        else {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void fileRename() {
    char path[1024];
    strcpy(path, pathToFile);
    strcat(path, "/library/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;

        if (strstr(filename, "r3N4mE") == NULL) continue;

        if (filename[0] == '.') continue;

        FILE *file = fopen(pathToLogFile, "a"); 
        if (file != NULL) {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully renamed.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }

        char old_path[1024];
        strcpy(old_path, path);
        strcat(old_path, filename);
        char new_path[1024];

        if (strstr(filename, ".ts") != NULL) {
            strcpy(new_path, path);
            strcat(new_path, "helper.ts");
        }

        else if (strstr(filename, ".py") != NULL) {
            strcpy(new_path, path);
            strcat(new_path, "calculator.py");
        }

        else if (strstr(filename, ".go") != NULL) {
            strcpy(new_path, path);
            strcat(new_path, "server.go");
        }

        else {
            strcpy(new_path, path);
            strcat(new_path, "renamed.file");
        }

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0) {
            char *cmd = "/bin/mv";
            char *argv[4] = { "mv", old_path, new_path, NULL };
            execvp(cmd, argv);
            return;
        }

        else {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void fileDelete() {
    char path[1024];
    strcpy(path, pathToFile);
    strcat(path, "/library/");

    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;

        if (strstr(filename, "d3Let3") == NULL) continue;

        if (filename[0] == '.') continue;

        FILE *file = fopen(pathToLogFile, "a"); 
        if (file != NULL) {  
            fprintf(file, "[%s][%02d:%02d:%02d] - %s - Successfully deleted.\n", username, digitime->tm_hour, digitime->tm_min, digitime->tm_sec, filename); 
            fclose(file); 
        }

        char file_path[1024];
        strcpy(file_path, path);
        strcat(file_path, filename);

        pid_t pid = fork();

        if (pid < 0) exit(EXIT_FAILURE);

        else if (pid == 0) {
            char *cmd = "/bin/rm";
            char *argv[3] = { "rm", file_path, NULL };
            execvp(cmd, argv);
            return;
        }

        else {
            int status;
            wait(&status);
        }
    } closedir(dir);
}

void runDefault() {
    fileReset();
    fileDownload();
    fileUnzip();
    fileDecrypt();
    fileRename();
    fileDelete();
}

int main(int argc, char **argv) 
{
    getcwd(workingDirectory, sizeof(workingDirectory));
    strcpy(pathToFile, workingDirectory);
    strcpy(pathToLogFile, pathToFile);
    strcat(pathToLogFile, "/history.log");

    username = (char*)malloc(1024);
    username = getlogin();
    runtime = time(NULL);
    digitime = localtime(&runtime);

    signal(SIGRTMIN, set_runDefault);
    signal(SIGUSR1, initiate_backup_process);
    signal(SIGUSR2, start_restore_process);
    signal(SIGKILL, terminate_program);

    if (argc == 3 && strcmp(argv[1], "-m") == 0) {
        if (strcmp(argv[2], "backup") == 0) mode = 1;
        else if (strcmp(argv[2], "restore") == 0) mode = 2;
    }

    pid_t pid, sid;

    pid = fork();

    if (pid < 0) exit(EXIT_FAILURE);

    if (pid > 0) return 0;

    umask(0);

    if (setsid() < 0) perror("Error: setsid() failed");
    if ((chdir("/")) < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1) {
        switch(mode) {
            case 0:
                runDefault();
                break;

            case 1:
                runBackup();
                break;

            case 2:
                runRestore();
                break;
        }
        sleep(100); 
    }
    return 0;
}
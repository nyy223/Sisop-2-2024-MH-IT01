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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_APPS 20
#define MAX_NAME_LENGTH 100

typedef struct {
    char config_name[MAX_NAME_LENGTH];
    int pids[MAX_APPS * 3];
    int pid_count;
} AppRecord;

void launch_applications(char *apps[], int counts[], int n, const char* source) {
    pid_t pid;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < counts[i]; j++) {
            pid = fork();
            if (pid == 0) {
                execlp(apps[i], apps[i], NULL);
                perror("Failed to start application");
                exit(1);
            }
        }
    }
}

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

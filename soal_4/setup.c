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
            records[i].pid_count = 0;  
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
            records[i].pid_count = 0;  
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

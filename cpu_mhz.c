#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/cpuinfo");
        exit(EXIT_FAILURE);
    }
    
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "cpu MHz") != NULL) {
            char* token = strtok(line, ":");
            token = strtok(NULL, ":");
            printf("CPU MHz: %s", token);
            // break; // Exit the loop after finding the CPU MHz
        }
    }
    fclose(fp); 

    return 0;
}

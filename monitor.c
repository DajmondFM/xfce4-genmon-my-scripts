#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Read cpu information
void read_cpu_times(long long *total_time, long long *idle_time, long long *user, long long *system, long long *iowait) {
    FILE *fp;
    char buffer[1024];
    long long nice, idle, irq, softirq, steal;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }

    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    sscanf(buffer, "cpu %lld %lld %lld %lld %lld %lld %lld %lld", user, &nice, system, &idle, iowait, &irq, &softirq, &steal);
    *total_time = *user + nice + *system + idle + *iowait + irq + softirq + steal;
    *idle_time = idle + *iowait;
}

// Read memory information
void mem(int* EMemTotal, int* EMemAvailable) {
    FILE* file = fopen("/proc/meminfo", "r");
    if (file == NULL) {
        perror("Failed to open /proc/meminfo");
        // return 1;
        exit(EXIT_FAILURE);
    }

    // char buffer[100];
    char MemTotal[50], MemFree[50], MemAvailable[50];

    fgets(MemTotal, sizeof(MemTotal), file); // Read the first line
    fgets(MemFree, sizeof(MemFree), file); // Read the second line
    fgets(MemAvailable, sizeof(MemAvailable), file); // Read the third line

    // Extract values from the lines
    int value_MemTotal, value_MemFree, value_MemAvailable;
    sscanf(MemTotal, "%*s %d", &value_MemTotal);
    // sscanf(MemFree, "%*s %d", &value_MemFree);
    sscanf(MemAvailable, "%*s %d", &value_MemAvailable);
    *EMemTotal = value_MemTotal;
    *EMemAvailable = value_MemAvailable;
    fclose(file);
}

int main() {
    // Functions
    long long total_time1, total_time2, idle_time1, idle_time2, user1, user2, system1, system2, iowait1, iowait2;
    int value_MemTotal, value_MemAvailable = 0;

    mem(&value_MemTotal, &value_MemAvailable);
    // First snapshot
    read_cpu_times(&total_time1, &idle_time1, &user1, &system1, &iowait1);
    usleep(500000); // Sleep for 100 milliseconds
    // Second snapshot
    read_cpu_times(&total_time2, &idle_time2, &user2, &system2, &iowait2);
    
    // CPU
    long long total_diff = total_time2 - total_time1;
    long long idle_diff = idle_time2 - idle_time1;

    long long user_diff = user2 - user1;
    long long system_diff = system2 - system1;
    long long iowait_diff = iowait2 - iowait1;

    double user_usage = 100.0 * (user_diff) / total_diff;
    double sys_usage = 100.0 * (system_diff) / total_diff;
    double iowait_usage = 100.0 * (iowait_diff) / total_diff;
    double cpu_usage = 100.0 * (total_diff - idle_diff) / total_diff;

    char* cpu_color = "lightgray";
    if (cpu_usage > 80) {
        cpu_color = "red";
    } else if (cpu_usage > 60) {
        cpu_color = "orange";
    } else if (cpu_usage > 40) {
        cpu_color = "yellow";
    } else if (cpu_usage > 20) {
        cpu_color = "lightgreen";
    }

    // MEMORY

    float KB_value_MemTotal = value_MemTotal * 1.024;
    float KB_value_MemAvailable = value_MemAvailable * 1.024;

    // Convert values from KB to MB
    float MB_value_MemTotal = KB_value_MemTotal /= 1000;
    float MB_value_MemAvailable = KB_value_MemAvailable /= 1000;

    // Calculate used memory
    float MB_MemUsed = MB_value_MemTotal - MB_value_MemAvailable;
    int MemUsed = value_MemTotal - value_MemAvailable;

    int MemPercent = MemUsed * 100 / value_MemTotal;

    char* mem_color = "lightgray";
    if (MemPercent > 80) {
        mem_color = "red";
    } else if (MemPercent > 60) {
        mem_color = "orange";
    } else if (MemPercent > 40) {
        mem_color = "yellow";
    } else if (MemPercent > 20) {
        mem_color = "lightgreen";
    }

    // Convert values from MB to GB
    float GB_value_MemTotal = MB_value_MemTotal /= 1000;
    float GB_value_MemAvailable = MB_value_MemAvailable /= 1000;
    float GB_MemUsed = MB_MemUsed /= 1000;
    GB_value_MemTotal = (int)(GB_value_MemTotal + 0.5);
    GB_value_MemAvailable = (int)(GB_value_MemAvailable + 0.5);
    GB_MemUsed = (int)(GB_MemUsed + 0.5);

    // SHOW

    printf("<txt><span color='lightgray'>Cpu: <span color='%s'>%.2f%%</span>\nMem: <span color='%s'>%d%%</span></span></txt>",cpu_color, cpu_usage, mem_color,  MemPercent);

    printf("<tool>"
           "┌ MEMORY %d%%\n"
           "├─ Total:      %.0fGB\n"
           "├─ Used:       %.0fGB\n"
           "└─ Available:  %.0fGB"
           "\n\n"
           "┌ CPU: %.2f%%\n"
           "├─ User:    %.2f%%\n"
           "├─ System:  %.2f%%\n"
           "└─ IOWait:  %.2f%%"
           "</tool>",MemPercent , GB_value_MemTotal, GB_MemUsed, GB_value_MemAvailable, cpu_usage, user_usage, sys_usage, iowait_usage);

    return 0;
}



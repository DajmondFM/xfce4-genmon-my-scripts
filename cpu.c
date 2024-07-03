#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void read_cpu_times(long long *total_time, long long *idle_time) {
    FILE *fp;
    char buffer[1024];
    long long user, nice, system, idle, iowait, irq, softirq, steal;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }

    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    sscanf(buffer, "cpu %lld %lld %lld %lld %lld %lld %lld %lld", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    *total_time = user + nice + system + idle + iowait + irq + softirq + steal;
    *idle_time = idle + iowait;
}

int main() {
    long long total_time1, total_time2, idle_time1, idle_time2;

    // First snapshot
    read_cpu_times(&total_time1, &idle_time1);
    usleep(500000); // Sleep for 100 milliseconds
    // Second snapshot
    read_cpu_times(&total_time2, &idle_time2);

    long long total_diff = total_time2 - total_time1;
    long long idle_diff = idle_time2 - idle_time1;

    double cpu_usage = 100.0 * (total_diff - idle_diff) / total_diff;

    printf("CPU Usage: %.2f%%\n", cpu_usage);

    return 0;
}

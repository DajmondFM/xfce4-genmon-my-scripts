#include <glibtop.h>
#include <glibtop/cpu.h>
#include <stdio.h>
#include <unistd.h> // For sleep

int main() {
  glibtop_init();

  glibtop_cpu cpu1, cpu2;

  // First measurement
  glibtop_get_cpu(&cpu1);
  usleep(500000); // Sleep for 100 milliseconds
  // Second measurement
  glibtop_get_cpu(&cpu2);

  unsigned long long int total_diff = cpu2.total - cpu1.total;
  unsigned long long int idle_diff = cpu2.idle - cpu1.idle;

  // Calculate the CPU usage as a percentage over the interval
  double cpu_usage = 100.0 * (total_diff - idle_diff) / total_diff;

  unsigned long long int user_diff = cpu2.user - cpu1.user;
  unsigned long long int sys_diff = cpu2.sys - cpu1.sys;
  unsigned long long int iowait_diff = cpu2.iowait - cpu1.iowait;

  double user_usage = 100.0 * (user_diff) / total_diff;
  double sys_usage = 100.0 * (sys_diff) / total_diff;
  double iowait_usage = 100.0 * (iowait_diff) / total_diff;

  // printf("CPU Usage: %.2f%%\n", cpu_usage);
  // printf("User CPU Usage: %.2f%%\n", user_usage);
  // printf("System CPU Usage: %.2f%%\n", sys_usage);
  // printf("IOWait CPU Usage: %.2f%%\n", iowait_usage);
  

  // cpu_usage = 80.5;
  char* color = "lightgray";
  
  printf("\n%.2f%\n", cpu_usage);
  if (cpu_usage > 80) {
    color = "red";
  } else if (cpu_usage > 60) {
    color = "orange";
  } else if (cpu_usage > 40) {
    color = "yellow";
  } else if (cpu_usage > 20) {
    color = "lightgreen";
  }

  printf("<txt><span foreground='%s'>CPU: %.2f%%</span></txt>", color, cpu_usage);
  // printf("<bar>%.2f%</bar>", cpu_usage);
  printf("<tool>User CPU Usage: %.2f%%\nSystem CPU Usage: %.2f%%\nIOWait CPU Usage: %.2f%%</tool>", user_usage, sys_usage, iowait_usage);

  glibtop_close();

  return 0;
}

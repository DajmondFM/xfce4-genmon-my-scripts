#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE* file = fopen("/proc/meminfo", "r");
  if (file == NULL) {
    perror("Error opening file");
    return 1;
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

  fclose(file);
  // Convert values from KiB to KB
  float KB_value_MemTotal = value_MemTotal * 1.024;
  float KB_value_MemAvailable = value_MemAvailable * 1.024;

  // Convert values from KB to MB
  float MB_value_MemTotal = KB_value_MemTotal /= 1000;
  float MB_value_MemAvailable = KB_value_MemAvailable /= 1000;


  // Calculate used memory
  float MB_MemUsed = MB_value_MemTotal - MB_value_MemAvailable;
  int MemUsed = value_MemTotal - value_MemAvailable;

  int Percent = MemUsed * 100 / value_MemTotal;

  printf("Percent: %d\n", Percent);
  printf("MemUsed: %.2f MB\n", MB_MemUsed);
  printf("MemTotal: %.2f MB\n", MB_value_MemTotal);
  printf("MemAvailable: %.2f MB\n", MB_value_MemAvailable);
  
  // Set the color based on the percentage
  char* color = "lightgray"; 

  if (Percent > 80) {
    color = "red";
  } else if (Percent > 60) {
    color = "orange";
  } else if (Percent > 40) {
    color = "yellow";
  } else if (Percent > 20) {
    color = "lightgreen";
  }
  printf("<txt><span color='%s'>Mem: %d%</span></txt>", color, Percent);

  printf("<tool>Used: %.2f MB\nTotal: %.2f MB\nAvailable: %.2f MB</tool>", MB_MemUsed, MB_value_MemTotal, MB_value_MemAvailable);

  return 0;
}

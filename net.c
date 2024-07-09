#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void get_network_usage(const char *interface, unsigned long long *rx_bytes, unsigned long long *tx_bytes) {
    FILE *fp = fopen("/proc/net/dev", "r");
    if (!fp) {
        perror("Failed to open /proc/net/dev");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        unsigned long long int dummy;
        char ifname[20];
        if (sscanf(line, " %[^:]: %llu %llu %llu %llu %llu %llu %llu %llu %llu",
                   ifname, rx_bytes, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, &dummy, tx_bytes) == 10) {
            if (strcmp(ifname, interface) == 0) {
                break;
            }
        }
    }

    fclose(fp);
}

int main() {
    const char *interface = "wlan0";  // Change this to your network interface
    unsigned long long rx_bytes_before, tx_bytes_before;
    unsigned long long rx_bytes_after, tx_bytes_after;

    // Measure the network usage
    get_network_usage(interface, &rx_bytes_before, &tx_bytes_before);
    usleep(500000);  // Wait for a 500ms
    get_network_usage(interface, &rx_bytes_after, &tx_bytes_after);

    // Calculate the difference
    unsigned long long rx_diff = rx_bytes_after - rx_bytes_before;
    unsigned long long tx_diff = tx_bytes_after - tx_bytes_before;

    printf("Received: %llu bytes\n", rx_diff);
    printf("Transmitted: %llu bytes\n", tx_diff);
    return 0;
}

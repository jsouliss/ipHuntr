#include "stdio.h"
#include "unistd.h"
#include "string.h"
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    char banner[] =
        ".__         ___ ___               __\n"
    "|__|_____  /   |   \\ __ __  _____/  |________\n"
    "|  \\____ \\/    ~    \\  |  \\/    \\   __\\_  __ \\\n"
    "|  |  |_> >    Y    /  |  /   |  \\  |  |  | \\/\n"
    "|__|   __/ \\___|_  /|____/|___|  /__|  |__|\n"
    "   |__|          \\/            \\/\n";

    printf("%s", banner);
    char ipaddr[16];
    FILE *fptr;

    fptr = fopen("huntedIps.txt", "w");

    printf("[i] Please enter the first three octets of the IP address (e.g., 192.168.1): ");
    scanf("%15s", ipaddr);
    printf("[+] Beginning scan...\n\n");
    printf("%s", "Happy Hunting!");

    for(int i = 0; i < 256; ++i) {
        // Store full length of IP Address
        char full_ip[16];
        snprintf(full_ip, sizeof(full_ip), "%s.%d", ipaddr, i);
        int flag = fork();
        if (flag == 0) {
            // This is the child process
            execl("/sbin/ping", "ping", "-c 1", full_ip, (char*)NULL);
            _exit(0); // Exit child process if execl fails
        }
        else if (flag > 0) {
            wait(NULL); // Tell the parent process to wait for the child process to finish
        }
        else {
            printf("[!] Fork failed for IP %s\n", full_ip);
        }

        if (flag != -1) {
            printf("[+] Pinged Host %s!\n", full_ip);
            // Write text to the file
            fprintf(fptr, "%s\n", full_ip);
        }
    }

    // Close the file
    fclose(fptr);
    return 0;
}
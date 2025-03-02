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
    int status;

    fptr = fopen("huntedIps.txt", "w");
    if (fptr == NULL) {
        perror("[!] Error opening file");
        return 1;
    }

    printf("[i] Please enter the first three octets of the IP address (e.g., 192.168.1): ");
    scanf("%15s", ipaddr);
    printf("[+] Beginning scan...\n\n");
    printf("%s", "Happy Hunting!\n");

    for(int i = 0; i < 256; ++i) {
        // Store full length of IP Address
        char full_ip[16];
        snprintf(full_ip, sizeof(full_ip), "%s.%d", ipaddr, i);
        pid_t pid = fork();
        if (pid == 0) {
            // This is the child process
            execlp("ping", "ping", "-c", "1", full_ip, (char*)NULL);
            _exit(1); // Exit child process if execl fails
        }
        // 0 means we are still in the child process
        else if (pid > 0) {
            wait(&status); // Tell the parent process to wait for the child process to finish
            /*
            * WIFEXITED(status): to ensure the child exited normally.
            * WEXITSTATUS(status): to check if the exit code is 0 (indicating a successful ping).
            */
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                printf("[+] Pinged Host %s!\n", full_ip);
                // Write text to the file
                fprintf(fptr, "%s\n", full_ip);
            }
        }
        else {
            printf("[!] Fork failed for IP %s\n", full_ip);
        }
    }

    // Close the file
    fclose(fptr);
    return 0;
}
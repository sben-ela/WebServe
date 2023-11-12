#include <stdio.h>
#include <netdb.h>  // For getaddrinfo
#include <string.h> // For strerror

int main() {
    const char *hostname = "aybiouss"; // Replace with your hostname
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        printf("Family: %s\n",
               rp->ai_family == AF_INET ? "IPv4" : "IPv6");
        printf("Socket Type: %s\n",
               rp->ai_socktype == SOCK_STREAM ? "TCP" : "UDP");
        // You can access other fields and details here as needed
        printf("\n");
    }

    freeaddrinfo(result); // Free the memory allocated by getaddrinfo

    return 0;
}
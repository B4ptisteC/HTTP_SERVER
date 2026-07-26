#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void banner() {
    printf("\n");
    printf(" _   _ _____ _____ ____    ____                           \n");
    printf("| | | |_   _|_   _|  _ \\  / ___|  ___ _ ____   _____ _ __ \n");
    printf("| |_| | | |   | | | |_) | \\___ \\ / _ \\ '__\\ \\ / / _ \\ '__|\n");
    printf("|  _  | | |   | | |  __/   ___) |  __/ |   \\ V /  __/ |   \n");
    printf("|_| |_| |_|   |_| |_|     |____/ \\___|_|    \\_/ \\___|_|   \n");
    printf("\n");
}

void launchHttpServer() {
    int server, client;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char buffer[1024] = {0};

    server = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(server, (struct sockaddr *)&addr, sizeof(addr));
    listen(server, 5);

    while (1) {
        printf("Listening on port 8080...\n");
        client = accept(server, (struct sockaddr *)&addr, &addr_len);

        int n = read(client, buffer, sizeof(buffer) - 1);
        if (n > 0) buffer[n] = '\0';
        printf("Received: %s\n", buffer);

        FILE *file = fopen("file.txt", "r");
        char body[4096] = {0};

        size_t total = fread(body, 1, sizeof(body) - 1, file);
        body[total] = '\0';
        fclose(file);
     
        char response[4500];
        int len = snprintf(response, sizeof(response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            strlen(body), body);

        write(client, response, len);
        close(client);
    }

    close(server);
}

int main() {
    banner();
    launchHttpServer();

    return 0;
}

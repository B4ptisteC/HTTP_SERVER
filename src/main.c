#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
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

    printf("Listening on port 8080...\n");

    client = accept(server, (struct sockaddr *)&addr, &addr_len);

    int n = read(client, buffer, sizeof(buffer) - 1);
    buffer[n] = '\0';
    printf("Received: %s\n", buffer);

    FILE *file;
    file = fopen("file.txt", "r");

    char response[100];

    // fgets isnt the good way to do this coz it'll only return the 1st line..
    fgets(response, 100, file);

    write(client, response, strlen(response));

    close(client);
    close(server);
    return 0;
}

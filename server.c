#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char server_reply[2000];
    char buffer[1024] = {0};
    char message[1024];
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1) {
    valread = read(new_socket, buffer, 1024);
    if (valread == 0) {
        printf("Client disconnected\n");
        break;
    } else if (valread < 0) {
        perror("read failed");
        break;
    } else {
        printf("Received message: %s\n", buffer);
        // process the message here
        //send(new_socket, "Message received", strlen("Message received"), 0);

        printf("Enter message to send to client: ");
        if (scanf("%[^\n]%*c", message) != 1) {
            perror("scanf failed");
            break;
        }
        if (send(new_socket, message, strlen(message), 0) < 0) {
            perror("send failed");
            break;
        }
    }
}
}



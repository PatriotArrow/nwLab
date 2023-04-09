#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr

int main(int argc, char *argv[]) {
    int socket_desc;
    struct sockaddr_in server;
    char server_reply[2000];
    char message[1024];

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    // Connect to remote server
    if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        puts("connect error");
        return 1;
    }

    puts("Connected");

    // Loop to read input from user and send to server
    while (1) {
        printf("Enter message: ");
        scanf("%[^\n]%*c", message);

        // Send message to server
        if (send(socket_desc, message, strlen(message), 0) < 0) {
            puts("Send failed");
            return 1;
        }

        // Receive reply from server
        if (recv(socket_desc, server_reply, 2000, 0) < 0) {
            puts("recv failed");
        }
        
        // Check if server has closed the connection
	if (strlen(server_reply) == 0) {
	    puts("Server closed the connection");
	    break;
	}


        puts("Server reply:");
        puts(server_reply);

        // Check if user wants to quit
        if (strcmp(message, "quit") == 0) {
            break;
        }
        
        //clear the buffer
        memset(server_reply, 0, sizeof(server_reply));
    }

    return 0;
}

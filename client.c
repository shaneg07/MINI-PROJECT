#include <stdio.h>
#include <errno.h>      
#include <fcntl.h>      
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/socket.h> 
#include <netinet/ip.h>
#include <string.h>

void connection_handler(int sockFD) {

    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;

    char tempBuffer[1000];

    do {
        bzero(readBuffer, sizeof(readBuffer)); //Empty the read buffer
        bzero(tempBuffer, sizeof(tempBuffer));

        readBytes = read(sockFD, readBuffer, sizeof(readBuffer));

        if(readBytes == -1)
            perror("Error while reading from client");
        else if (readBytes == 0)
            printf("No error recieved, closing connection\n");     
        
        else if(strchr(readBuffer, '^') != NULL) {//search for ^

            strncpy(tempBuffer, readBuffer, strnlen(readBuffer) - 1);
            printf("%s\n", tempBuffer);
            writeBytes = write(sockFD, "^", strlen("^"));
            if (writeBytes == -1) {
                perror("Error while writing to client socket\n");
                break;
            }
        }
        else if (strchr(readBuffer, '$') != NULL)
        {
            // Server sent an error message and is now closing it's end of the connection
            strncpy(tempBuffer, readBuffer, strlen(readBuffer) - 2);
            printf("%s\n", tempBuffer);
            printf("Closing the connection to the server now!\n");
            break;
        }
        else
        {
            bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer

            if (strchr(readBuffer, '#') != NULL)
                strcpy(writeBuffer, getpass(readBuffer));
            else
            {
                printf("%s\n", readBuffer);
                scanf("%[^\n]%*c", writeBuffer); // Take user input!
            }

            writeBytes = write(sockFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing to client socket!");
                printf("Closing the connection to the server now!\n");
                break;
            }
        }
    } while (readBytes > 0);

    close(sockFD);
}

void main()
{
    int socketFileDescriptor, connectStatus;
    struct sockaddr_in serverAddress;
    struct sockaddr server;

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor == -1)
    {
        perror("Error while creating server socket!");
        _exit(0);
    }

    serverAddress.sin_family = AF_INET;                // IPv4
    serverAddress.sin_port = htons(8081);              // Server will listen to port 8080
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    connectStatus = connect(socketFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (connectStatus == -1)
    {
        perror("Error while connecting to server!");
        close(socketFileDescriptor);
        _exit(0);
    }

    connection_handler(socketFileDescriptor);

    close(socketFileDescriptor);
}
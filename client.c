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
        
        else if(strchr(readBuffer, '^') != NULL) //search for ^ {

            strncpy(tempBuffer, readBuffer, strnlen(readBuffer) - 1);
            printf("%s\n", tempBuffer);
            writeBytes = write(sockFD, )

        }
    }
}
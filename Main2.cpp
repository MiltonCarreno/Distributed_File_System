// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Client.h"

void func(int sockfd)
{
    char buff[1024];
    int n;
    for (;;) {
        bzero(buff, sizeof(buff));
        printf("Enter the string : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');
        
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Controller : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main(int argc, char const* argv[]) {
  int valread;
  const char* hello = "Hello from client";
  char buffer[1024] = { 0 };

  // Create new client
  Client client;

  // Creat new client socket
  int sock = client.createSocket();
  // Request connection with client socket
  client.requestConnection();
  // Chat with controller
  send(sock, hello, strlen(hello), 0);
  printf("Hello message sent\n");
  valread = read(sock, buffer, 1024);
  printf("%s\n", buffer);
  // Chat
  func(sock);
  // Close connection
  client.closeConnection();
  return 0;
}
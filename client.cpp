// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
const char* LOCAL_HOST = "127.0.0.1";

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
        printf("From Server : %s", buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

int main(int argc, char const* argv[]) {
  int sock = 0, valread, client_fd;
  struct sockaddr_in serv_addr;
  char* hello = "Hello from client";
  char buffer[1024] = { 0 };
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // serv_addr.sin_addr.s_addr = INADDR_ANY;

  // Convert IPv4 and IPv6 addresses from text to binary
  // form
  if (inet_pton(AF_INET, LOCAL_HOST, &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }
  send(sock, hello, strlen(hello), 0);
  printf("Hello message sent\n");
  valread = read(sock, buffer, 1024);
  printf("%s\n", buffer);

  func(sock);

  // closing the connected socket
  close(client_fd);
  return 0;
}
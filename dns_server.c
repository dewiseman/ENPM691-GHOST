#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

const unsigned short PORT = 5300;
const unsigned BUF_SIZE = 4096; 

int main(int argc, char **argv) {
  //set up a socket to listen to UDP connections
  int sockfd;
  struct sockaddr_in server, client;
  int addrlen = sizeof(server);
  
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("Socket Creation Failed\n");
    exit(EXIT_FAILURE);
  }
  memset(&server, sizeof(server), 0);
  memset(&client, sizeof(client), 0);
  
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);
  
  if (bind(sockfd, (const struct sockaddr*) &server, addrlen) < 0) {
    perror("Socket Bind Failed\n");
    exit(EXIT_FAILURE);
  }
   int len = sizeof(client);
   int p_size = 0; // payload size
   char name[BUF_SIZE];
   while(1) {
     p_size = recvfrom(sockfd, (char*)name, BUF_SIZE, 0, (struct sockaddr*)
      &client, &len);
     
     if (p_size > 0 && p_size < BUF_SIZE)
       name[p_size - 1] = '\0';
     
     else
       continue;
     
     //expecting a hostname in name
     //printf("DEBUG: Client sent: %s\n", name);
     //get an ip address for the host
     struct hostent *result;
     printf("Resolving hostname %s\n", name);
     result = gethostbyname(name);
     char *ip;
     if (!result) {
       printf("Could not resolve host %s\n", name);
       continue;
     }
     else {
       ip = inet_ntoa(*((struct in_addr*) result->h_addr_list[0]));
       if (ip) {
         printf("IP: %s\n", ip);
         strcat(ip, "\n");
         sendto(sockfd, ip, strlen(ip), 0, (const struct sockaddr*)&client, len);
       }
     }
   }
   return 0;
 }


#include "../incl/webserv.hpp"

// int main(void)
// {
// 	int domain = AF_INET;
// 	int type = SOCK_STREAM;
// 	int protocol = 0;
// 	int serverSocket = socket(domain, type, protocol);
// 	std::cout << serverSocket << std::endl;
// 	sockaddr serverAddress;
// 	serverAddress.sa_family = AF_INET;
// 	bind(serverSocket, 0, 0);
// 	// while (1)
// 	// {

// 	// }
// 	return 0;
// }
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fstream>
#include <vector>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char **argv)
{
	int sockfd;
	int portno;
	int n = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	std::string input = "";

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	server = gethostbyname(argv[1]);
	if (server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
	serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
	char buffer[256];
	bzero(buffer, 256);
	n = read(sockfd, buffer, 256);
	if (n < 0) 
         error("ERROR reading from socket");
	std::cout << buffer << std::endl;
	while (1)
	{
    	printf("Please enter the message: ");
		std::cin.clear();
		std::getline(std::cin, input);
		if (strcmp(input.data(), "quit") == 0)
			break;
		n = send(sockfd, input.data(), input.size(), 0);
		//testing
		// std::cout << "reading" << std::endl;
		bzero(buffer, 256);
		n = recv(sockfd, buffer, 256, 0);
		if (n < 0) 
        	error("ERROR reading from socket");
		std::cout << "Other clients sent message:" << std::endl << buffer << std::endl;
	}
}


// int main(int argc, char *argv[])
// {
//     int sockfd, portno, n;
//     struct sockaddr_in serv_addr;
//     struct hostent *server;

//     char buffer[16];
//     if (argc < 3) {
//        fprintf(stderr,"usage %s hostname port\n", argv[0]);
//        exit(0);
//     }
//     portno = atoi(argv[2]);
//     sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd < 0) 
//         error("ERROR opening socket");
//     server = gethostbyname(argv[1]);
//     if (server == NULL) {
//         fprintf(stderr,"ERROR, no such host\n");
//         exit(0);
//     }
//     bzero((char *) &serv_addr, sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     bcopy((char *)server->h_addr, 
//          (char *)&serv_addr.sin_addr.s_addr,
//          server->h_length);
//     serv_addr.sin_port = htons(portno);
//     if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
//         error("ERROR connecting");
//     printf("Please enter the message: ");
//     bzero(buffer,16);
//     fgets(buffer,15,stdin);
//     n = write(sockfd,buffer,strlen(buffer));
//     if (n < 0) 
//          error("ERROR writing to socket");
//     bzero(buffer,16);
//     n = read(sockfd,buffer,15);
//     if (n < 0) 
//          error("ERROR reading from socket");
//     printf("%s\n",buffer);
//     close(sockfd);
//     return 0;
// }
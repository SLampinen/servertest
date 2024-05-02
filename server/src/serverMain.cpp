
#include "../incl/webserv.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char **argv)
{
    // int sockfd, newsockfd, portno;
	// socklen_t clilen;
	// int n = 1;

	int opt = true;
	int masterSocket;
	int maxClients = 30;
	int clientSocket[maxClients];
	int maxSd;
	int newSocket;
	int addrlen;
	int valread;
	int sd;
	int activity;
	

	fd_set readfds;

    char buffer[256];
    struct sockaddr_in serv_addr;
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	int PORT = std::atoi(argv[1]);
	for (int i = 0; i < maxClients; i++)
	{
		clientSocket[i] = 0;
	}
	// std::cout << "clients made" << std::endl;
	if ((masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		error("ERROR, Socket failed");
	}
	// std::cout << "masterSocket made" << std::endl;
	if (setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0)
	{
		error("ERROR, setsockopt failed");
	}
	// std::cout << "setsockopt made" << std::endl;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);

	if (bind(masterSocket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		error("ERROR, bind failed");
	}

	// std::cout << "masterSocket binded" << std::endl;
	if (listen(masterSocket, 3) < 0)
	{
		error("ERROR, listen failed");
	}

	// std::cout << "masterSocket listen" << std::endl;
	addrlen = sizeof(serv_addr);

	while (1)
	{
		FD_ZERO(&readfds);
		FD_SET(masterSocket, &readfds);
		maxSd = masterSocket;

		for (int i = 0; i < maxClients; i++)
		{
			sd = clientSocket[i];
			if (sd > 0)
			{
				FD_SET(sd, &readfds);
			}
			if (sd > maxSd)
			{
				maxSd = sd;
			}
		}
		// std::cout << "clients set to readfds" << std::endl;
		// std::cout << "waiting for client" << std::endl;
		activity = select(maxSd + 1, &readfds, NULL, NULL, NULL);

		if (activity < 0 && (errno != EINTR))
		{
			error("ERROR, select failed");
		}

		if (FD_ISSET(masterSocket, &readfds))
		{
			if ((newSocket = accept(masterSocket, (struct sockaddr *)&serv_addr, (socklen_t *)&addrlen)) < 0)
			{
				error("ERROR, accept failed");
			}
			// std::cout << "New connection, socket fd = " << newSocket << " ip = " << inet_ntoa(serv_addr.sin_addr) << 
			// " port = " << ntohs(serv_addr.sin_port) << std::endl;

		std::string message = "You are now connected\n";
		if (send(newSocket, message.data(), strlen(message.data()), 0) != strlen(message.data()))
		{
			error("ERROR, send failed");
		}
		std::cout << "messaged client" << std::endl;

			for (int i = 0; i < maxClients; i++)
			{
				if (clientSocket[i] == 0)
				{
					clientSocket[i] = newSocket;
					std::cout << "added to clientSocket[" << i << "]" << std::endl;
					break;
				}
			}
		}

		for (int i = 0; i < maxClients; i++)
		{
			sd = clientSocket[i];

			if (FD_ISSET(sd, &readfds))
			{
				if ((valread = recv(sd, buffer, 256, 0)) == 0)
				{
					getpeername(sd, (struct sockaddr*)&serv_addr, (socklen_t *)&addrlen);
					std::cout << "Host disconnected, ip = " << inet_ntoa(serv_addr.sin_addr) << std::endl;

					close(sd);
					clientSocket[i] = 0;
				}
				else
				{
					buffer[valread] = '\0';
					// send(sd, buffer, strlen(buffer), 0);
					std::cout << "This is message from the client:" << std::endl << buffer << std::endl;
					for (int j = 0; j < maxClients; j++)
					{
						if (j != i)
						{
							send(clientSocket[j], buffer, strlen(buffer), 0);
						}
					}
					
				}
			}
			
		}
		
	}
	
	// struct sockaddr_in cli_addr; 
	// sockfd = socket(AF_INET, SOCK_STREAM, 0);
	// if (sockfd < 0) 
	// error("ERROR opening socket");
	// bzero((char *) &serv_addr, sizeof(serv_addr));
	// portno = atoi(argv[1]);
	// serv_addr.sin_family = AF_INET;
	// serv_addr.sin_addr.s_addr = INADDR_ANY;
	// serv_addr.sin_port = htons(portno);
	// if (bind(sockfd, (struct sockaddr *) &serv_addr,
	// 		sizeof(serv_addr)) < 0) 
	// 		error("ERROR on binding");


	// while (1)
	// {
	// 	listen(sockfd,5);
	// 	clilen = sizeof(cli_addr);
	// 	newsockfd = accept(sockfd, 
	// 				(struct sockaddr *) &cli_addr, 
	// 				&clilen);
	// 	if (newsockfd < 0) 
	// 		error("ERROR on accept");
	// 	bzero(buffer,256);
	// 	n = read(newsockfd,buffer,255);
	// 	if (n < 0) error("ERROR reading from socket");
	// 	printf("Here is the message: %s\n",buffer);
	// 	n = write(newsockfd,"I got your message",18);
	// 	if (n < 0) error("ERROR writing to socket");
	// }
	// close(newsockfd);
	// close(sockfd);
	return 0; 
}
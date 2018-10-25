#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char* argv[])
{
	printf("Creating new socket...\n");
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);

	if(socketfd>0)
		printf("Socket created successfully! file descriptor: %d\n", socketfd);
	else
		printf("Error socket was not created!\n");

	const struct sockaddr_in server = {.sin_addr.s_addr = inet_addr("127.0.0.1"), .sin_family = AF_INET, .sin_port = htons(2000)};

	printf("Binding socket...\n");
	if(!bind(socketfd, (const struct sockaddr *)(&server), sizeof(server)))
		printf("Socket bound successfully!\n");

	printf("Listening...\n");
	listen(socketfd, 3);
	printf("Listening...\n");

	struct sockaddr_in client;

	int connectionsocketfd = accept(socketfd, NULL, NULL);
	printf("Accepting request... Connection socket: %d\n", connectionsocketfd);

	char message[512];
	int receivedMessage = recv(connectionsocketfd, message, 512, MSG_PEEK);
	if(receivedMessage == -1)
		printf("Receiving error: %d", errno);
	printf("Received message: %d\n", receivedMessage);

	printf("%s", message);

	char messageToSend[] = "HTTP/1.0 200 OK\nServer: myserver\nContent-Type: text/html\r\n\r\n<html><head><title>Web Server Test</title><style>.centered{text-align: center;margin: auto; color: blue; font-size: 20px;</style></head><body><div class=\"centered\">test</div></body></html>";
	
	int resultOfSend = send(connectionsocketfd, messageToSend, sizeof(messageToSend), MSG_NOSIGNAL);

	printf("Sent: %d\n", resultOfSend);

	shutdown(connectionsocketfd, SHUT_RDWR);
	close(connectionsocketfd);

	shutdown(socketfd, SHUT_RDWR);
	close(socketfd);

	return 0;
}

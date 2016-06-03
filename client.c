//
// COEN 146L lab3
// Yuya Oguchi
// 4/19/2016
// Client.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//arg input filename hostname
int main(int argc, char *argv[]){
    ////////// same for UDP TCP //////////////
    struct hostent *server;
    //info about IP itself
    struct sockaddr_in si_server;
    //local host name
    server = gethostbyname(argv[1]);
    bzero((char*)&si_server, sizeof(si_server));
    si_server.sin_family = AF_INET;
    //copy server content to si_server to avoid changing pointer
    bcopy((char*)server->h_addr,(char*)&si_server.sin_addr,server->h_length);
    //address of the local host port
    si_server.sin_port = htons(5555);
    //////////////////////////////////////////
    //////////// UDP Specified ///////////////
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    socklen_t socklength = sizeof(si_server);

	//init buffer and sizes client will get from server
	char otheruser[256];
	char username[256];
	int name;
	bool end = false;

	////////////Start Chat //////////////
	//username declare
	printf("What is your username?\n");
	fgets(username,100,stdin);
	//send username to other computer
	sendto(sockfd,username,sizeof(username),0,(struct sockaddr *)&si_server,socklength);
	//receive other username
	name = recvfrom(sockfd,otheruser,sizeof(otheruser),0,(struct sockaddr *)&si_server, &socklength);
    printf("You are now connected to %s\n\n",otheruser);



	// Start chatting //
	while(end == false){
		//talk//
		char message[100];
		printf("\n%s:",username);
		//read in message
		fgets(message,100,stdin);
		sendto(sockfd,message,sizeof(message),0,(struct sockaddr *)&si_server,socklength);
		printf("\n");
		//quit if i typed quit
		if(strcmp(message,"quit\n")==0){
			end = true;
			break;
		}

		//Listen//
		int string;
		char buff[256];
		//receive new string 
		string = recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr *)&si_server, &socklength);
		printf("%s:%s",otheruser, buff);
		//quit if other user typed quit
		if(strcmp(buff,"quit\n")==0){
			end = true;
		}
	}	
	printf("Connection closed\n");

return 1;
}

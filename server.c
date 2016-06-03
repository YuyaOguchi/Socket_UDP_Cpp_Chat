//
// COEN 146L lab3
// Yuya Oguchi
// 4/19/2016
// server.c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[]){
    ////////// same for UDP TCP //////////////
    //info about IP itself
    struct sockaddr_in si_server;
    bzero((char*)&si_server, sizeof(si_server));
    si_server.sin_family = AF_INET;
    //copy server content to si_server to avoid changing pointer
    si_server.sin_addr.s_addr = INADDR_ANY;
    si_server.sin_port = htons(5555);
    //////////////////////////////////////////
    //////////// UDP Specified ///////////////
    int sockfd = socket(PF_INET,SOCK_DGRAM,0);
    bind(sockfd,(struct sockaddr *)&si_server, sizeof(si_server));

    //init buffer and sizes server will get from client
    char otheruser[256];
	char username[256];
    int name;
	bool end = false;

    //length of name of socket addr
    socklen_t l = sizeof(si_server);

	//////////////Start Chat///////////////
	//get username of client
	name  = recvfrom(sockfd, otheruser, sizeof(otheruser), 0, (struct sockaddr*)&si_server,&l);
	//username declare
	printf("What is your username?\n");
	fgets(username,100,stdin);
	printf("You are now connected to %s\n", otheruser);
	//send username to other computer
	sendto(sockfd,username,sizeof(username),0,(struct sockaddr *)&si_server,l);

	//Start chatting//
	while(end == false){
		//listen//
		int string;
		char buff[256];
		//receive new string
		string = recvfrom(sockfd, buff, sizeof(buff), 0, (struct sockaddr*)&si_server, &l);
   		printf("%s:%s",otheruser, buff);
		//quit if other user typed quit
		if (strcmp(buff,"quit\n") == 0){
			end = true;
			break;
		}

		//talk//
		char message[100];
		printf("\n%s:",username);
		//read in message
		fgets(message,100,stdin);
		sendto(sockfd,message,sizeof(message),0,(struct sockaddr *)&si_server,l);
		printf("\n");
		//quit if i type quit
		if(strcmp(message,"quit\n") == 0){
			end = true;
		}
	}
	printf("Connection closed\n");
return 1;
}


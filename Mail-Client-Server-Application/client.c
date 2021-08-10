#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

void listusers(int sockfd){
	char msg[4] = "LSTU";
	char buff[MAX];
	write(sockfd, msg, sizeof(msg));
    bzero(buff,sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s",buff);
    //printf("\n");
}

void adduser(int sockfd,char buff[MAX]){
	//printf("hello\n");
	char buff2[MAX];
	bzero(buff2,sizeof(buff2));
	buff2[0]='A';
	buff2[1]='D';
	buff2[2]='D';
	buff2[3]='U';
	for(int i=7;i<MAX;i++){
		buff2[i-3]=buff[i];
	}
	//printf("%s",buff2);
	write(sockfd, buff2, sizeof(buff2));
	bzero(buff2,MAX);
	read(sockfd, buff2, sizeof(buff2));
	if(buff2[0]=='s'){
		return;
	}
	else{
		printf("%s\n",buff2);
	}
}

void readcomm(int sockfd){
	char msg[4] = "READ";
	char buff[MAX];
	bzero(buff,sizeof(buff));
    write(sockfd, msg, sizeof(msg));
    read(sockfd, buff, sizeof(buff));
    //printf("%s",buff);
    if(buff[0]=='N'){
    	printf("%s",buff);
    	return;
    }
    //printf("%s\n",buff);
    bzero(buff,sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s",buff);
    bzero(buff,sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s",buff);
    bzero(buff,sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s",buff);
    bzero(buff,sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s",buff);
    bzero(buff,sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("%s",buff);
    bzero(buff,sizeof(buff));
    
}

void delete(int sockfd){
	//move to next item
	char msg[4] = "DELM";
	char buff[MAX];
	bzero(buff,MAX);
    write(sockfd, msg, sizeof(msg));
    read(sockfd, buff, sizeof(buff));
    if(buff[0]=='N'){
    	printf("%s\n",buff);
    	return;
    }
    bzero(buff,MAX);
    read(sockfd, buff, MAX);
    printf("%s\n",buff);
}

void sendcomm(int sockfd,char buff[MAX]){
	//verify again
	char buff2[MAX];
	bzero(buff2,sizeof(buff2));
	buff2[0]='S';
	buff2[1]='E';
	buff2[2]='N';
	buff2[3]='D';
	for(int i=4;i<MAX;i++){
		buff2[i]=buff[i];
	}

	write(sockfd, buff2, sizeof(buff2));
	bzero(buff, MAX); 
	read(sockfd, buff, MAX);

	//printf("%s\n",buff);
	//return;
	// check if users exists and give / dont give error msg
	if(buff[0]=='U'){
		printf("%s\n",buff);
		return;
	}
	int n; 
	//sends server to SEND func,we wait for further input here
	printf("Type Subject: ");
		bzero(buff, MAX); 
		//printf("Main-Prompt>"); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, MAX);

	printf("Type Message: ");
		bzero(buff, MAX); 
		//printf("Main-Prompt>"); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		write(sockfd, buff, MAX);	
    // printf("finish");
}

void done(int sockfd){
    char msg[4] = "DONE";
    //RESET THE CURRENT USER POINTER//server side.
    write(sockfd, msg, sizeof(msg));
}

void setuser(int sockfd,char buff3[MAX]){
	char buff2[MAX],buff[MAX];
	int n;
	bzero(buff2,sizeof(buff2));
	buff2[0]='U';
	buff2[1]='S';
	buff2[2]='E';
	buff2[3]='R';
	for(int i=7;i<MAX;i++){
		buff2[i-3]=buff3[i];
	}
	write(sockfd, buff2, sizeof(buff2));
	bzero(buff2,MAX);
	read(sockfd, buff2, sizeof(buff2));
    
    if(buff2[0]=='U'){
    	printf("%s",buff2);
    	return;
    }
    bzero(buff2,MAX);
    read(sockfd, buff2, sizeof(buff2));
    printf("%s\n",buff2);
        //construct subprompt string...
    char net[MAX]="Sub-Prompt-";
    for(int i=8;i<MAX-3;i++){
    	net[i+3] = buff3[i];
    }
    net[strcspn(net, "\n")] = 0;
    int len = strlen(net);
    net[len] = '>';
	//subprompt starts here
	for (;;) { 

		bzero(buff, sizeof(buff)); 
		printf("%s",net); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		if(buff[0]=='R'){
			readcomm(sockfd);
		}
		else if(buff[0]=='D'&&buff[1]=='e'){
			delete(sockfd);
		}
		else if(buff[0]=='S'){
			sendcomm(sockfd,buff);
		}
	    else if(buff[0]=='D'&&buff[1]=='o'){
	    	done(sockfd);
            return;
	    }
		//write(sockfd, buff, sizeof(buff)); 
		//bzero(buff, sizeof(buff)); 
		//read(sockfd, buff, sizeof(buff)); 
		//printf("From Server : %s", buff); 
		//if ((strncmp(buff, "exit", 4)) == 0) { 
		//	printf("Client Exit...\n"); 
		//	break; 
		//} 
	} 
}

void quit(int sockfd){
	char msg[4] = "QUIT";
	char buff[MAX];
	write(sockfd, msg, sizeof(msg));
    bzero(buff,sizeof(buff));
    //read(sockfd, buff, sizeof(buff));
    return;
    //printf("%s",buff);
}

void func(int sockfd) 
// divide ito functions to send the appropriate data
{ 
	char buff[MAX]; 
	int n; 
	for (;;) {

		bzero(buff, sizeof(buff)); 
		printf("Main-Prompt>"); 
		n = 0; 
		while ((buff[n++] = getchar()) != '\n') 
			; 
		if(buff[0]=='L'){
			listusers(sockfd);
		}
		else if(buff[0]=='A'){
			adduser(sockfd,buff);
		}
		else if(buff[0]=='S'){
			setuser(sockfd,buff);
		}
	    else if(buff[0]=='Q'){
	    	quit(sockfd);
            return;
	    }
		//write(sockfd, buff, sizeof(buff)); 
		//bzero(buff, sizeof(buff)); 
		//read(sockfd, buff, sizeof(buff)); 
		//printf("From Server : %s", buff); 
		//if ((strncmp(buff, "exit", 4)) == 0) { 
		//	printf("Client Exit...\n"); 
		//	break; 
		//} 
	} 
} 



int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	func(sockfd); 

	// close the socket 
	close(sockfd); 
} 

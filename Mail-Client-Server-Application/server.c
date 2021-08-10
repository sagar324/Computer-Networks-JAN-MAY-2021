#include <assert.h>
#include <stdio.h> 
#include <time.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 


struct Node{
	char name[MAX];
	int total;
	int current;
	struct Node* next;
};

struct Node* currentuserptr = NULL;
struct Node* Toptr = NULL;
struct Node* listhead = NULL;

// Function designed for chat between client and server. 
void LSTU(int sockfd){
	char completelist[MAX] = "";
	struct Node* temp = listhead;
	while(temp!=NULL){
		strcat(completelist,temp->name);
		strcat(completelist," ");
		temp = temp->next;
	}
	strcat(completelist,"\n");
	write(sockfd, completelist, sizeof(completelist));
}

void ADDU(int sockfd,char buff[MAX]){
	//printf("hello\n");
	char nameuser[MAX];
	bzero(nameuser, MAX);
	for(int i=5;i<MAX;i++){
		nameuser[i-5] = buff[i];
	}
	nameuser[strcspn(nameuser, "\n")] = 0;
	//write(sockfd, nameuser, sizeof(nameuser));
	//printf("%s",nameuser);
	if(listhead==NULL){
		listhead = (struct Node*)malloc(sizeof(struct Node));
		listhead->total = 0;
		listhead->current = 0;
		strcpy(listhead->name,nameuser);
		char error[MAX] = "success";
		write(sockfd, error , sizeof(error));
	}
	else{
		int flag = 0;	
		struct Node* temp = listhead;
		while(temp->next!=NULL){
            if(strcmp(temp->name,nameuser)==0){
            	flag=1;
            	break;
            }
            temp=temp->next;
		}
		if(flag==1){
            char error[MAX] = "Userid already present";
            write(sockfd, error , sizeof(error));
            return;
		}
		else{
			struct Node* newnode;
			newnode = (struct Node*)malloc(sizeof(struct Node));
			strcpy(newnode->name,nameuser);
			newnode->total = 0;
			newnode->current = 0;

			temp->next = newnode;

			char error[MAX] = "success";
			write(sockfd,error,sizeof(error));
		}
	}
	FILE *spoof;

	int len = strlen(nameuser);
	nameuser[len]='.';
	nameuser[len+1]='t';
	nameuser[len+2]='x';
	nameuser[len+3]='t';

    spoof = fopen(nameuser,"w");
    fclose(spoof);
}

void USER(int sockfd,char buff[MAX]){
	char nameuser[MAX];
	bzero(nameuser, MAX);
	for(int i=5;i<MAX;i++){
		nameuser[i-5] = buff[i];
	}
	nameuser[strcspn(nameuser, "\n")] = 0;
	//find user
	struct Node* temp = listhead;
	while(temp!=NULL){
		if(strcmp(temp->name,nameuser)==0){
			break;
		}
		temp = temp->next;
	}
	if(temp==NULL){
		char error[MAX] = "Userid does not exist";
		write(sockfd, error, sizeof(error));
		return;
	}
	currentuserptr = (struct Node*)malloc(sizeof(struct Node));
	currentuserptr = temp;
	char error[MAX] = "success";
	write(sockfd, error, sizeof(error));
    // open file and count number of ### and give output
	int num = currentuserptr->total;
	char result[MAX];
	sprintf(result,"%d",num);
	char msg[MAX] = "Number of mails in this user's spoof file: ";
	int len = strlen(result);
	int len2 = strlen(msg);
	for(int i=0;i<len;i++){
		msg[len2+i] = result[i];
	}
	write(sockfd,msg,sizeof(msg));
}

void READM(int sockfd){
	//currentuserptr = globaltemp;
	if(currentuserptr==NULL){
		printf("BAD\n");
	}
	else{
	//	printf("GOOD\n");
	}
	if(currentuserptr->total == 0){
		char error[MAX] = "No More Mail";
		write(sockfd, error , sizeof(error));
		return;
	}
	char error[MAX] = "success";
	write(sockfd, error , sizeof(error));
	bzero(error,MAX);
	if(currentuserptr->current >= currentuserptr->total){
		currentuserptr->current = 0;
	}
	char nameuser[MAX];
	char nameuser2[MAX];
	int len;
	len = strlen(currentuserptr->name);
	for(int i=0;i<MAX;i++){
        nameuser[i] = (currentuserptr->name)[i];
        //nameuser2[i] = (currentuserptr->name)[i];
	}
	int posn = currentuserptr->current;
	nameuser[strcspn(nameuser, "\n")] = 0;
	//nameuser2[strcspn(nameuser2, "\n")] = 0;
	//5k+0,1,2,3,4 lines
	int counter = 0;
	FILE *spoof;
	len = strlen(nameuser);
    nameuser[len]='.';
	nameuser[len+1]='t';
	nameuser[len+2]='x';
	nameuser[len+3]='t';
	//open the file now
	spoof = fopen(nameuser,"r");
	printf("%s",nameuser);
	if(spoof==NULL){
		printf("ERROR1\n");
	}
	else{
	//	printf("GOD\n");
	}
	fseek(spoof, 0, SEEK_SET);

	char buff[MAX];
	bzero(buff, MAX);
	//code
	while( fgets(buff, 80, spoof) != NULL ) 
   	{ 
        if(counter<6*posn){
        	counter++;
        	continue;
        } 
        write(sockfd, buff, sizeof(buff));
        bzero(buff, MAX);
        counter++;
        if(counter==(6*posn)+5){
        	break;
        }
    } 
	//code
	currentuserptr->current = (currentuserptr->current)+1;
	
	fclose(spoof);
}

void DELM(int sockfd){
	int posn = currentuserptr->current;
	int maxi = currentuserptr->total;
	if(maxi==0){
		char error[MAX] = "No More Mail";
		write(sockfd, error, sizeof(error));
	}
	char error[MAX] = "success";
	write(sockfd, error, sizeof(error));

	if(posn>=maxi){
		posn=0;
		currentuserptr->current = 0;
	}
	char nameuser[MAX];
	int len;
	len = strlen(currentuserptr->name);
	for(int i=0;i<len;i++){
        nameuser[i] = (currentuserptr->name)[i];
	}
	//int posn = currentuserptr->current;
	nameuser[strcspn(nameuser, "\n")] = 0;
	//5k+0,1,2,3,4 lines
	int counter = 0;
	FILE *spoof,*temporary;
	len = strlen(nameuser);
    nameuser[len]='.';
	nameuser[len+1]='t';
	nameuser[len+2]='x';
	nameuser[len+3]='t';
	//open the file now
	spoof = fopen(nameuser,"r");
	temporary = fopen("temp.txt","a");
    fseek(temporary,0,SEEK_SET);
    fseek(spoof,0,SEEK_SET);

	char buff[MAX];
    bzero(buff,MAX);
	while( fgets(buff, 80, spoof) != NULL ) 
   	{ 
       if(counter>=6*posn&&counter<=(6*posn)+5){
       		counter++;
       		continue;
       }
       fputs(buff,temporary);
       counter++;
    }
    fclose(spoof); 
    char msg[MAX] = "Message Deleted";
    if(remove(nameuser)==0){
    	write(sockfd, msg, sizeof(msg));
    }
    //else{
    //	char error2[MAX] = "error,file not deleted";
    //	 write(sockfd, msg, sizeof(msg));
   // }
    rename("temp.txt",nameuser);
    //fseek(temporary, 0, SEEK_SET);
    fclose(temporary);

}

void SEND(int sockfd,char buff[MAX])
{
	char nameuser[MAX],buff2[MAX],buff3[MAX];
	bzero(nameuser, MAX);
	for(int i=5;i<MAX;i++){
		nameuser[i-5] = buff[i];
	}
	nameuser[strcspn(nameuser, "\n")] = 0;
	struct Node* temp = listhead;
	while(temp!=NULL){
		if(strcmp(temp->name,nameuser)==0){
			break;
		}
		temp = temp->next;
	}
	if(temp==NULL){
		char error[MAX] = "Userid does not exist";
		write(sockfd, error, sizeof(error));
		return;
	}
	Toptr = (struct Node*)malloc(sizeof(struct Node));
	Toptr = temp;
	char error[MAX] = "success";
	write(sockfd, error, sizeof(error));

	//wait for one line inpt and write in the file
	bzero(buff2,sizeof(buff2));
	bzero(buff3,sizeof(buff3));

	read(sockfd, buff2, sizeof(buff2));
	buff2[strcspn(buff2, "\n")] = 0;
	// this is our SUBJECT to write in toptr's spoof file.
    read(sockfd, buff3, sizeof(buff3));
    buff3[strcspn(buff3, "\n")] = 0;
    //this is our mail.
   	 char FROM[MAX],TO[MAX],DATE[MAX]="Date: ";
   	 //int i=0;
    FROM[0] = 'F';
    FROM[1] = 'r';
    FROM[2] = 'o';
    FROM[3] = 'm';
    FROM[4] = ':';
    FROM[5] = ' ';
    char nextline[MAX] = "\n";
    int len = strlen(currentuserptr->name);

    //for(int i=0;i<len;i++){
    //	FROM[i+6] = currentuserptr->name[i];
    //}
    for(int i=0;i<MAX-6;i++){
    	FROM[i+6] = (currentuserptr->name)[i];
    }
    strcat(FROM,nextline);
    //FROM STRING DONE
    TO[0] = 'T';
    TO[1] = 'o';
    TO[2] = ':';
    TO[3] = ' ';

    len = strlen(Toptr->name);

    for(int i=0;i<MAX-4;i++){
    	TO[i+4] = Toptr->name[i];
    }
    strcat(TO,nextline);
    //TO STRING DONE
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    assert(strftime(s, sizeof(s), "%c", tm));
        int length = strlen(s);
    //char DATE[80];
    int itr;
    for(itr=0;itr<length-4;itr++){
    	DATE[itr+6] = s[itr];
    }
    DATE[itr+6] = 'I';itr++;
    DATE[itr+6] = 'S';itr++;
    DATE[itr+6] = 'T';itr++;
    DATE[itr+6] = ' ';itr++;
    int temporary = 0;
    while(temporary<4){
    	DATE[itr+temporary+6] = s[itr+temporary-4]; 
    	temporary++;
    }
    strcat(DATE,nextline);
    //DATE IS DONE
    char SUBJECT[MAX] = "Subject: ";
    len = strlen(buff2);
    for(int i=0;i<len;i++){
    	SUBJECT[i+9] = buff2[i];
    }
    strcat(SUBJECT,nextline);
    //SUBJECT DONE
    char CONTENTS[MAX] = "Contents: ";
    len = strlen(buff3);
    for(int i=0;i<len;i++){
    	CONTENTS[i+10] = buff3[i];
    }
    strcat(CONTENTS,nextline);
    //CONTENTS DONE
 	
    len = strlen(nameuser);
    nameuser[len]='.';
	nameuser[len+1]='t';
	nameuser[len+2]='x';
	nameuser[len+3]='t';

    FILE *spoof;

    spoof = fopen(nameuser,"a");
    //fseek(spoof, 0, SEEK_SET);

    fputs(FROM,spoof);
    //fputs("\n",spoof);
    fputs(TO,spoof);
    //fputs("\n",spoof);
    fputs(DATE,spoof);
    //fputs("\n",spoof);
    fputs(SUBJECT,spoof);
    //fputs("\n",spoof);
    fputs(CONTENTS,spoof);
    //fputs("\n",spoof);
    fputs("###\n",spoof);
    //fputs("\n",spoof);

    Toptr->total = (Toptr->total)+1;
    
    fclose(spoof);
	return;
}

void DONEU(int sockfd){
	currentuserptr = NULL;
}

void QUIT(int sockfd){
	//printf("ahola");
	struct Node* temp = listhead;
	while(temp!=NULL){
		char nameuser[MAX];
		for(int i=0;i<MAX;i++){
			nameuser[i] = (temp->name)[i];
		}
		int len = strlen(nameuser);
		nameuser[len]='.';
		nameuser[len+1]='t';
		nameuser[len+2]='x';
		nameuser[len+3]='t';

		//code
		int garbage = remove(nameuser);
		temp = temp->next;
	}

}

void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		//printf("From client: %s", buff); 
		//printf(" To client : ");		
		if(buff[0]=='L'){
			LSTU(sockfd);
		}
		else if(buff[0]=='A'){
			ADDU(sockfd,buff);
		}
		else if(buff[0]=='U'){
			USER(sockfd,buff);
		}
		else if(buff[0]=='R'){
            READM(sockfd);
		}
        else if(buff[0]=='D'&&buff[1]=='E'){
        	DELM(sockfd);
        }
        else if(buff[0]=='S'){
        	SEND(sockfd,buff);
        }
        else if(buff[0]=='D'&&buff[1]=='O'){
            DONEU(sockfd);
        }
        else if(buff[0]=='Q'){
        	QUIT(sockfd);
        	return;
        }
		bzero(buff, MAX); 
		//n = 0; 
		// copy server message in the buffer 
		//while ((buff[n++] = getchar()) != '\n') 
		//	; 

		// and send that buffer to client 
		//write(sockfd, buff, sizeof(buff)); 

		// if msg contains "Exit" then server exit and chat ended. 
		//if (strncmp("exit", buff, 4) == 0) { 
		//	printf("Server Exit...\n"); 
		//	break; 
		//} 
	} 
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

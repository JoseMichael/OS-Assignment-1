#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");
     
    int counter = 0;

    //keep communicating with server
    while(1)
    {

 if(counter==0)
	{
	int sizeOfArray=0;
        printf("Enter the size of the array : ");
        scanf("%d" , &sizeOfArray);
	int integerArray[sizeOfArray];
	printf("Please enter the various array elements \n");
	int j = 0;
	for(j=0; j<sizeOfArray; j++)
	{
	scanf("%d",&integerArray[j]);
	}
	int valToTestAndDiscard = 15;	
	
	//sending size first
	printf("Checkpoint Inside Send ka Size ka if \n");
	//int statusOfSend = send(sock , &sizeOfArray , sizeof(sizeOfArray) , 0);
	int statusOfSend = send(sock , &valToTestAndDiscard , sizeof(valToTestAndDiscard) , 0);
		if( statusOfSend < 0)
		{
		    puts("Send failed");
		    return 1;
		}
	/*
	printf("Checkpoint Inside Send ka Array ka if \n");
	//int statusOfSend = send(sock , message , strlen(message) , 0);
	int statusOfSend2 = send(sock , integerArray , sizeOfArray , 0);
		if( statusOfSend2 < 0)
		{
		    puts("Send failed");
		    return 1;
		}*/
		else if(statusOfSend>0)
		{
			counter = counter + 1;
		}
	}
        

         

    }
     
    close(sock);
    return 0;
}
/*

Operating Systems Project 1
Server Code
Jose Michael Joseph
Bhavin Modi

*/
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include "functions.h"
 
//the thread function
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
	int testvar=0;
	int **testarray;

//Counter is a depricated variable that is being kept for future use
int counter=0;
     
    //Receive a message from client
while(1)
{
	//code to read the size of the array that will be sent
	read_size = recv(sock , &testvar , sizeof(testvar) , 0);
	if( read_size > 0 )
	{
		//Send the message back to client

		//adding an integer array with size got from before
		int intArray[testvar];


		//adding a while that will look for the array
		//printf used to check stuff -- printf("Here the recv is gonna look for an array with size of %d \n", (int)sizeof(intArray));
		while(1)
		//this while is used to receive the array from the client
		{

			read_size = recv(sock , intArray , sizeof(intArray) , 0);
			if(read_size > 0)
			{
				printf("Array received \n");

				int maxValueInTheArray = max(testvar,intArray);
				//printf used to test code -- printf("The max value in that array was %d \n", maxValueInTheArray);
				//writing the result back to the client
				write(sock , &maxValueInTheArray , sizeof(maxValueInTheArray));

				break;
				//the break is supposed to ideally break the internal while
			}//end of if that checks if inside second while
		}//end of while that looks for array

		counter = counter + 1;

	}//end of if
	else if(read_size == 0)
	{
		puts("Client disconnected");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("recv failed");
	}

}//end of while

//Free the socket pointer
free(socket_desc);

return 0;
}

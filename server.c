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

int counter=0;
     
    //Receive a message from client
while(1)
{
/* trying to restructure code to keep nested whiles
if(counter==0)
{
read_size = recv(sock , &testvar , sizeof(testvar) , 0);
}
else
{
read_size = recv(sock , testarray , sizeof(testarray) , 0);
}
*/

	//step 1 : read the size message
	read_size = recv(sock , &testvar , sizeof(testvar) , 0);
    	if( read_size > 0 )
    	{
        //Send the message back to client
        	printf("yup got the message for counter %d \n",counter);

//dummy code to print array
/*	if(counter==1)
	{
		int j=0;
		printf("Values are \n");
		for(j=0; j<testvar; j++)
		{
			printf("%d ", &testarray[j]);
		}
	}

*/
//adding an integer array with size got from before
		int intArray[testvar];
		//trying with fixed size
		//int intArray[10];

//adding a while that will look for the array
		printf("Here the recv is gonna look for an array with size of %d \n", (int)sizeof(intArray));
		while(1)
		{
			//printf("Looking for array now \n");
			
			read_size = recv(sock , intArray , sizeof(intArray) , 0);
			//read_size = recv(sock , &testvar , sizeof(testvar) , 0);
			if(read_size > 0)
			{
				printf("Array received \n");

				int maxValueInTheArray = max(testvar,intArray);
				printf("The max value in that array was %d \n", maxValueInTheArray);

/*
all below code was used to check if array is being received properly
				printf("Array's 1st element is %d \n",intArray[0]);
				printf("Array's 3rd element is %d \n",intArray[2]);
printf("Printing array to test \n");
		int j=0;
printf("Size of testvar is %d \n",testvar);
//printf("Size of one elemeant of intarr is %d \n",(int)sizeof(intArray[0]));
//printf("No of elements in intArray are %d \n",(int)(testvar/sizeof(intArray[0])));
		printf("Values are \n");
		for(j=0; j<(testvar); j++)
		{
			printf("%d \n", intArray[j]);
		}

*/
	



				break;
				//the break is supposed to ideally break the internal while
			}
		}

		counter = counter + 1;



    	}
    else if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
}
         
    //Free the socket pointer
    free(socket_desc);
     
    return 0;
}
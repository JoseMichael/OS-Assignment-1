//server copy before indent that had the menu implemented

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

void recv2DArrays(int , int *, int *, int **);
int recvFromClient(int , int , int *, int *, char *, int);
 
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
    
while(1)
{
	
	//initializing new while to look for counter value
	int statusOfCounterRecv = recvFromClient(sock, 1, NULL, &counter, NULL, (int)sizeof(int));
	if(statusOfCounterRecv>0)
	{
		printf("Counter value successfully received and is %d \n",counter);
		break; //breaks from outer while loop
	}
	
}
	
	
if(counter==0)
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
}//end of if counter==0
else if(counter==1)
{

int rowSize1, rowSize2, rowSize3;
int colSize1, colSize2, colSize3;

int *matrix1, *matrix2, *matrix3;


recv2DArrays(sock, &rowSize1, &colSize1, &matrix1);

recv2DArrays(sock, &rowSize2, &colSize2, &matrix2);

recv2DArrays(sock, &rowSize3, &colSize3, &matrix3);


printf("The values of the array are \n");
int rows,cols;
printf("rowSize1 and colSize1 are %d %d \n",rowSize1,colSize1);

printf("Matrix 1 \n");

//Code used to display the matrix

	for(rows=0;rows<rowSize1;rows++)
	{
	for(cols=0;cols<colSize1;cols++)
	{
	//printf("Test");
	printf("%d",*(matrix1+((rows * colSize1)+cols)));
	}
	printf("\n");
	}

printf("Matrix 2 \n");

//Code used to display the matrix
/* */
	for(rows=0;rows<rowSize2;rows++)
	{
	for(cols=0;cols<colSize2;cols++)
	{
	//printf("Test");
	printf("%d",*(matrix2+((rows * colSize2)+cols)));
	}
	printf("\n");
	}
	


printf("Matrix 3 \n");

//Code used to display the matrix

	for(rows=0;rows<rowSize3;rows++)
	{
	for(cols=0;cols<colSize3;cols++)
	{
	//printf("Test");
	printf("%d",*(matrix3+((rows * colSize3)+cols)));
	}
	printf("\n");
	}
	
	
	
	//printf("Babe just gonna get into multiply \n");

multiply(matrix1, matrix2, rowSize1, colSize1, colSize2, matrix3);

printf("See matrix 3 after calculation \n");
//Code used to display the matrix
/* */
	for(rows=0;rows<rowSize3;rows++)
	{
	for(cols=0;cols<colSize3;cols++)
	{
	//printf("Test");
	printf("%d",*(matrix3+((rows * colSize3)+cols)));
	}
	printf("\n");
	}

int sendStatusOfMultipliedMatrix = send2DMatrixToClient(sock, rowSize3, colSize3, matrix3);

if(sendStatusOfMultipliedMatrix==1)
{
	printf("Muliplied Array sent successfully \n");
}


}//end of counter==2
else if(counter==2)
{
	char string[2048];
	int status = recvFromClient(sock, 3, NULL, 0, string, 2048);
	if(status==1)
	{
		//printf("Yo got that string shit \n");
		printf("The string received is %s \n",string);
		
		int wordCountResult = wc(string);
		printf("The number of words in that sentence are %d \n",wordCountResult);
		
		int statusOfWordCountToClient =  sendToClient( sock, 1, NULL, wordCountResult, NULL, sizeof(int));
		if(statusOfWordCountToClient>0)
		{
			printf("Word count sent \n");
		}
		
	}
}
}//end of while

//Free the socket pointer
free(socket_desc);

return 0;
}


int send2DMatrixToClient(int sock, int row, int column, int *integerArray)
{
	int statusOfRowSizeSend = sendToClient(sock, 1, NULL, row, NULL, 0);
	int statusOfColSizeSend = sendToClient(sock, 1, NULL, column, NULL, 0);
	int serializedSize = row*column;


	printf("Serialized size is %d \n",serializedSize);
	
	printf("Size of array is %d \n",(int)(sizeof(integerArray)));


	if(statusOfRowSizeSend>0 && statusOfColSizeSend>0)
	{

		//size has been successfully sent
		int statusOfArraySend = sendToClient(sock, 2, integerArray, -999, NULL, sizeof(int)*serializedSize);
		if(statusOfArraySend > 0)
		{
		//array also has been sent successfully
		printf("Array has been sent successfully \n");
		
		return 1;
		}
		else if(statusOfArraySend < 0)
		{
			printf("Array send failed \n");
			return 0;
		}

	}//end of if with row and col size
	else
	{
		printf("Error in sending row and col size \n");
	}

}//end of function



int recvFromClient(int sock, int typeOfData, int *intArray, int *integerToRecv, char *stringToRecv, int sizeOfData)
{
/*
basic type assumptions for this code
1. Integer
2. 1D Array
3. String

Also note any value that is not needed for sending that type would be set to null

*/

switch(typeOfData)
{

case 1:
	while(1)
	{
		int statusOfSend = recv(sock , integerToRecv , sizeof(integerToRecv) , 0);
		if(statusOfSend > 0)
		{
			printf("Got an integer %d \n",*integerToRecv);
			return 1;
		}
		else if(statusOfSend < 0)
		{
			break; //breaks from the while
		}
	}//end of while
break;

case 2:
	while(1)
	{
		int statusOfSend = recv(sock , intArray , sizeOfData , 0);
		if(statusOfSend > 0)
		{
			return 1;
		}
		else if(statusOfSend < 0)
		{
			break; //breaks from the while
		}
	}//end of while
break;

case 3:
	while(1)
	{	
		//printf("Size of string being received is %d \n",(int)sizeof(stringToRecv));
		int statusOfSend = recv(sock , stringToRecv , sizeof(char)*2048 , 0);
		if(statusOfSend > 0)
		{
			//printf("Yo got that string bae \n");
			return 1;
		}
		else if(statusOfSend < 0)
		{
			break; //breaks from the while
		}
	}//end of while
break;
}//end of switch

return 0; //this shows that there was an error in the send
}//end of function


void recv2DArrays(int sock, int *rowSize, int *colSize, int **integerArray)
{

int recvRowSizeStatus = recvFromClient(sock, 1, NULL, rowSize, NULL, 0);
printf("The rowSize got is %d \n", *rowSize);
if(recvRowSizeStatus<0)
{
	printf("Error in sending row size \n");
}
int recvColSizeStatus = recvFromClient(sock, 1, NULL, colSize, NULL, 0);
if(recvRowSizeStatus<0)
{
	printf("Error in sending col size \n");
}

printf("The colSize got is %d \n", *colSize);

if(recvRowSizeStatus > 0 && recvColSizeStatus > 0)
{
//this loop is entered if both row and col size have been accepted
	int serializedSize = *rowSize * *colSize;
	//commenting for test int integerArray[serializedSize];
	printf("The serialized size we're getting is %d",serializedSize);
	*integerArray = (int*)malloc(serializedSize);
	printf("The sizeof(integerArray) we're getting is %d",(int)sizeof(integerArray));

	//int *resultPointer=integerArray;

	/** Dummy Pointer*/


	int dummy = -999;
	int *dummyPointer = &dummy;



		int statusOfArraySend = recvFromClient(sock, 2, *integerArray, dummyPointer, NULL, sizeof(int)*serializedSize);
		if(statusOfArraySend > 0)
		{
		//array also has been received successfully
		printf("Array received successfully \n");
		return;
		}
		else if(statusOfArraySend < 0)
		{
			printf("Array recv failed \n");
		}


}//end of if for row and col size sent



}//end of function

int sendToClient(int sock, int typeOfData, int *intArray, int integerToSend, char *stringToSend, int sizeOfData)
{
/*
basic type assumptions for this code
1. Integer
2. 1D Array
3. String

Also note any value that is not needed for sending that type would be set to null

*/

switch(typeOfData)
{

case 1:
	while(1)
	{
		int statusOfSend = send(sock , &integerToSend , sizeof(integerToSend) , 0);
		if(statusOfSend > 0)
		{
			return 1;
		}
		else if(statusOfSend < 0)
		{
			break; //breaks from the while
		}
	}//end of while
break;

case 2:
	while(1)
	{
		int statusOfSend = send(sock , intArray , sizeOfData , 0);
		if(statusOfSend > 0)
		{
			printf("Size of the array being sent is %d \n",sizeOfData);
			return 1;
		}
		else if(statusOfSend < 0)
		{
			break; //breaks from the while
		}
	}//end of while
break;

case 3:
	while(1)
	{
		int statusOfSend = send(sock , stringToSend , sizeof(stringToSend) , 0);
		if(statusOfSend > 0)
		{
			return 1;
		}
		else if(statusOfSend < 0)
		{
			break; //breaks from the while
		}
	}//end of while
break;
}//end of switch

return 0; //this shows that there was an error in the send
}//end of function
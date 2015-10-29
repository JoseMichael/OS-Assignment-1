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

#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
 
//the thread function
int sockToDirServer;
int programID=1;
int versionNumber=1;
int deRegistered=0;

int waitForAck(int sock)
{
	//Wait for Ack
	int statusOfReceive, ack;
	while(1)
	{
		statusOfReceive = recv(sock , &ack , sizeof(int),0);
		if(statusOfReceive < 0)
		{
			//Failure break
			return -1;
		}
		else if (statusOfReceive > 0)
		{
			puts("Got ACK");
			//Got ACK
			return 1;
		}
	}
}//end of waitForAck
		

//this function is used to connect to the dir server
int connectToDirServer()
{
	struct sockaddr_in server;
    
	//Create socket
    sockToDirServer = socket(AF_INET , SOCK_STREAM , 0);
    if (sockToDirServer == -1)
    {
        puts("Could not create socket");
		return 0;
    }
    puts("Socket created");

	//below should contain the ip address of the Directory Service
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8000 );
	
	//Connect to Server
	puts("Trying to connect\n");
	int connected = -1;
	while(connected == -1){
		//Connect to remote server
    	connected = connect(sockToDirServer , (struct sockaddr *)&server , sizeof(server));
	}
    
	//Connected to server
    puts("Connected \n");
    return 1;
}

int registerWithDirService()
{
	//this code is used to register the server with the directory service
	int statusOfConnect;
	statusOfConnect = connectToDirServer();
	printf("Status of connect is %d \n",statusOfConnect);
	if(statusOfConnect!=1)
	{
		printf("Connection to Directory Service failed \n");
		return 0;
	}
	
	//we first send '1' to the Dir Service which shows it that we are a server
	int statusOfServerSend = sendToClient(sockToDirServer, 1, NULL, 1, NULL, sizeof(int));
	if(statusOfServerSend!=1)
	{
		printf("Sending '1' to indicate Server to Dir Service failed \n");
		return 0;
	}
	
	int statusOfAck;
	statusOfAck = waitForAck(sockToDirServer);
	if(statusOfAck < 0)
	{
		printf("Ack from Dir Service failed \n");
		return 0;
	}
	
	printf("Going to send value to indicate I want to register \n");
	//we then send '1' to indicate that we want to Register
	int statusOfRegisterMsgSend = sendToClient(sockToDirServer, 1, NULL, 1, NULL, sizeof(int));
	if(statusOfRegisterMsgSend!=1)
	{
		printf("Sending '1' to indicate Server to Dir Service failed \n");
		return 0;
	}
	
	statusOfAck = waitForAck(sockToDirServer);
	if(statusOfAck < 0)
	{
		printf("Ack from Dir Service failed \n");
		return 0;
	}
	
	//now we send the port
	//here 8888 is the port that we are listening to for connections from the dir service
	int statusOfPortSend = sendToClient(sockToDirServer, 1, NULL, 8888, NULL, sizeof(int));
	if(statusOfPortSend!=1)
	{
		printf("Sending Port Number failed \n");
		return 0;
	}
	
	statusOfAck = waitForAck(sockToDirServer);
	if(statusOfAck < 0)
	{
		printf("Ack from Dir Service failed \n");
		return 0;
	}
	
	//now we send the program id
	int statusOfProgIDSend = sendToClient(sockToDirServer, 1, NULL, programID, NULL, sizeof(int));
	if(statusOfProgIDSend!=1)
	{
		printf("Sending Program ID failed \n");
		return 0;
	}
	
	statusOfAck = waitForAck(sockToDirServer);
	if(statusOfAck < 0)
	{
		printf("Ack from Dir Service failed \n");
		return 0;
	}
	
	//now we send the version number
	int statusOfVersionNoSend = sendToClient(sockToDirServer, 1, NULL, versionNumber, NULL, sizeof(int));
	if(statusOfVersionNoSend!=1)
	{
		printf("Sending Version No failed \n");
		return 0;
	}
	
	statusOfAck = waitForAck(sockToDirServer);
	if(statusOfAck < 0)
	{
		printf("Ack from Dir Service failed \n");
		return 0;
	}
	
	int registrationStatus=0;
	int statusOfRegistrationStatus = recvFromClient(sockToDirServer, 1, NULL, &registrationStatus, NULL, sizeof(int));
	
	if(registrationStatus==1)
	{
		printf("Service successfully registered \n");
		close(sockToDirServer); //closing the socket
		return 1;
	}
	else
	{
		printf("Registration failed \n");
		return 0;
	}
}



int deregisterWithDirService()
{
	//this code is used to register the server with the directory service
	int statusOfConnect;
	statusOfConnect = connectToDirServer();
	if(statusOfConnect!=1)
	{
		printf("Connection to Directory Service failed \n");
		return 0;
	}
	
	//we first send '1' to the Dir Service which shows it that we are a server
	int statusOfServerSend = sendToClient(sockToDirServer, 1, NULL, 1, NULL, sizeof(int));
	if(statusOfServerSend!=1)
	{
		printf("Sending '1' to indicate Server to Dir Service failed \n");
		return 0;
	}
	
	int statusOfAck;
	statusOfAck = waitForAck(sockToDirServer);
	if(statusOfAck < 0)
	{
		printf("Ack from Dir Service failed \n");
		return 0;
	}
	
	//we then send '0' to indicate that we want to deregister
	int statusOfRegisterMsgSend = sendToClient(sockToDirServer, 1, NULL, 0, NULL, sizeof(int));
	if(statusOfRegisterMsgSend!=1)
	{
		printf("Sending '0' to indicate Server to Dir Service failed \n");
		return 0;
	}
	
	statusOfAck = waitForAck(sockToDirServer);
	if(statusOfAck < 0)
	{
		printf("Ack from Dir Service failed \n");
		return 0;
	}
	
	int deregistrationStatus=0;
	int statusOfDeregistrationStatus = recvFromClient(sockToDirServer, 1, NULL, &deregistrationStatus, NULL, sizeof(int));
	
	if(statusOfDeregistrationStatus==1)
	{
		printf("Service successfully deregistered \n");
		close(sockToDirServer); //closing the socket
		return 1;
	}
	else
	{
		printf("Deregistration failed \n");
		return 0;
	}
	

}


void setDeregisteredTrue(void)
{
	deRegistered = 1;
	
	//printf("Value of deregistered is now %d \n",deRegistered);
	
	int statusOfDeregistration = deregisterWithDirService();
	if(statusOfDeregistration==1)
	{
		printf("Deregistration complete \n");
	}
	else
	{
		printf("Deregistration failed \n");
	}
	
	//write invocation to deregister function
	//TODO: Write deregister function
}

void* deRegisterMenu(void *args)
{
	printf("Please enter 1 to deregister \n");
	scanf("%d",&deRegistered);
	if(deRegistered!=1)
	{
		printf("Incorrect option \n");
		deRegisterMenu(&deRegistered);
	}
	setDeregisteredTrue();
}
 

/*void receiveCharArrayFromClient(int sock, char **charArray)
{
	
}*/
 
int receiveArrayFromClient(int sock, int *size, int **integerArray)
{
	int sizeOfArray=0;
	int statusOfSizeSend;
	statusOfSizeSend = recvFromClient(sock, 1, NULL, size, NULL, (int)sizeof(int));
	if( statusOfSizeSend < 0 )
	{
		printf("Receive failed. \n");
		return;
	}
	int size2 = *size;
	*integerArray = (int*)malloc(size2);
	//int intArray[sizeOfArray];
		int dummy = -999;
	int *dummyPointer = &dummy;

		int statusOfArraySend = recvFromClient(sock, 2, *integerArray, dummyPointer, NULL, sizeof(int)*size2);
		if(statusOfArraySend > 0)
		{
		//array also has been received successfully
		printf("Array received successfully \n");
		return 1;
		}
		else if(statusOfArraySend < 0)
		{
			printf("Array recv failed \n");
			return 0;
		}
}

void successOrFailedSend(int a)
{
	if(a==1)
	{
		printf("Value sent successsfully \n");
	}
	else
	{
		printf("Value NOT sent successsfully \n");
	}
}

int send1DArrayToClient(int sock, int sizeOfArray, int *sortedArray)
{
	int statusOfSizeSend = sendToClient(sock, 1, NULL, sizeOfArray, NULL, 0);
	successOrFailedSend(statusOfSizeSend);
	int statusOfArraySend = sendToClient(sock, 2, sortedArray, -999, NULL, sizeof(int)*sizeOfArray);
	successOrFailedSend(statusOfArraySend);
	if(statusOfSizeSend==1 && statusOfSizeSend==1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
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
			
			int statusOfAck = sendAck(sock);			
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
			int statusOfAck = sendAck(sock);
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
			int statusOfAck = sendAck(sock);
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


int recv2DArrays(int sock, int *rowSize, int *colSize, int **integerArray)
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
		return 1;
		}
		else if(statusOfArraySend < 0)
		{
			printf("Array recv failed \n");
		}


}//end of if for row and col size sent


return 0;
}//end of function

int sendAck(int sock)
{
	int ackValue = 1;
	int statusOfAck = sendToClient(sock, 1, NULL, ackValue, NULL, (int)(sizeof(int)));
	if(statusOfAck>0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

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
			printf("Integer %d was sent \n",integerToSend);
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


void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
	//int testvar=0;
	int **testarray;

//Counter is a depricated variable that is being kept for future use
int counter=0;
    
    //Receive a message from client
    
while(deRegistered==0)
{
	//printf("Value of deregistered is %d \n",deRegistered);
	//TODO: Add function to send client some message when the connection is terminated due to deRegistered=0
    
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
	
	
switch(counter)
{
	
case 1:
{
	//function for max
	int size;
	int *intArr;
	int statusOfRecvParam1 = receiveArrayFromClient(sock, &size, &intArr);
	if(statusOfRecvParam1==0)
		break;
	int result = max(size,intArr);
	int statusOfValSent = sendToClient(sock, 1, NULL, result, NULL, 0);
	successOrFailedSend(statusOfValSent);

}//end of case 1
break;
case 2:
{
	//function for min
	int size;
	int *intArr;
	int statusOfRecvParam1 = receiveArrayFromClient(sock, &size, &intArr);
	if(statusOfRecvParam1==0)
		break;
	int result = min(size,intArr);
	int statusOfValSent = sendToClient(sock, 1, NULL, result, NULL, 0);
	successOrFailedSend(statusOfValSent);

}//end of if counter == 2
break;
case 3:
{
/*
int rowSize1, rowSize2, rowSize3;
int colSize1, colSize2, colSize3;
int *matrix1, *matrix2, *matrix3;

int statusOfParam1 =recv2DArrays(sock, &rowSize1, &colSize1, &matrix1);
int statusOfParam2 =recv2DArrays(sock, &rowSize2, &colSize2, &matrix2);
int statusOfParam3 =recv2DArrays(sock, &rowSize3, &colSize3, &matrix3);

if(!(statusOfParam1==1 && statusOfParam2==1 && statusOfParam3==1))
{
	break;
}
*/

int rowSize1,colSize1;
int *matrix1;

int statusOfParam1 =recv2DArrays(sock, &rowSize1, &colSize1, &matrix1);

if(!(statusOfParam1==1))
{
	break;
}

int rowSize2,colSize2;
int *matrix2;

int statusOfParam2 =recv2DArrays(sock, &rowSize2, &colSize2, &matrix2);

if(!(statusOfParam2==1))
{
	break;
}

int rowSize3,colSize3;
int *matrix3;

int statusOfParam3 =recv2DArrays(sock, &rowSize3, &colSize3, &matrix3);

if(!(statusOfParam3==1))
{
	break;
}


int* result = multiply(rowSize1, colSize1,matrix1,rowSize2,colSize2, matrix2,rowSize3,colSize3,matrix3);

int statusOfValSent = send2DMatrixToClient(sock, rowSize3, colSize3, result);
successOrFailedSend(statusOfValSent);

}//end of case 3
break;
case 4:
{//function for sorting

	int size;
	int *intArr;
	int statusOfRecvParam1 = receiveArrayFromClient(sock, &size, &intArr);
	if(statusOfRecvParam1==0)
		break;
	int *result = sort(size,intArr);				
	int statusOfValSent = send1DArrayToClient(sock, size, result);
	successOrFailedSend(statusOfValSent);

}//end of case 4
break;
case 5:
{ 
	//function for word count
	char string[2048];
	//char *string;
	int status = recvFromClient(sock, 3, NULL, 0, string, 2048*sizeof(char));
	if(status==0)
		break;
	int result = wc(string);
	int statusOfValSent =  sendToClient( sock, 1, NULL, result, NULL, sizeof(int));
	successOrFailedSend(statusOfValSent);
		
}//end of case 5
break;
}//end of switch

}//end of while above that checks for deregistered and maintains menu

//Free the socket pointer
free(socket_desc);

return 0;
}


int main(int argc , char *argv[])
{
	//code to register with dir service
	int statusOfRegistry = registerWithDirService();
	if(statusOfRegistry==1)
	{
		printf("Dir Registry Successful \n");
	}
	
	
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
    //server.sin_addr.s_addr = inet_addr("127.0.0.1");
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
     
     
     
	//adding code for thread that is supposed to send deregister info to directory
	
	pthread_t deregisterThread;
	int statusOfDeregisterThread = pthread_create(&deregisterThread, NULL, deRegisterMenu, &deRegistered);
	if(statusOfDeregisterThread==0)
	{
		 printf("Deregister Thread created successfully \n");
	}
	else
	{
		printf("Deregister Thread creation failed \n");
	}
	
	
	
	//*************************************************************************
     
     
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




#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<stdio.h>
#include "functions.h"

//Globals
int sock;

int sendAck(int sock)
{
	int ackValue = 1, statusOfAck;
	statusOfAck = send(sock, &ackValue, sizeof(int), 0);
	if(statusOfAck > 0){
		return 1;
	}else{
		return -1;
	}
}

int sendToServer(int sock, int typeOfData, int *intArray, int integerToSend, char *stringToSend, int sizeOfData){
	/*
	basic type assumptions for this code
	1. Integer
	2. 1D Array
	3. String

	Also note any value that is not needed for sending that type would be set to null

	*/

	switch(typeOfData){
		case 1:
			while(1){
				int statusOfSend = send(sock , &integerToSend , sizeof(integerToSend) , 0);
				if(statusOfSend > 0){
					break;
				}
				else if(statusOfSend < 0){
					return -1;
				}
			}//end of while
		break;

		case 2:
			while(1){
				int statusOfSend = send(sock , intArray , sizeOfData , 0);
				if(statusOfSend > 0){
					printf("Inside receiver for int array and gonna break now \n");
					break;
				}
				else if(statusOfSend < 0){
					printf("Inside receiver for int array and failure and gonna return \n");
					return -1;
				}
			}//end of while
		break;

		case 3:
			while(1){
				int statusOfSend = send(sock , stringToSend , sizeOfData , 0);

				if(statusOfSend > 0){
					break;
				}
				else if(statusOfSend < 0){
					return -1;
				}
			}//end of while
		break;
	}//end of switch

	//Wait for Ack
	int statusOfReceive, ack;
	while(1){
		statusOfReceive = recv(sock , &ack , sizeof(int),0);
		if(!(statusOfReceive > 0)){
			//Failure break
			return -1;
		}else{
			printf("Got an ACK babe \n");
			//Got ACK
			break;
		}
	}
	
	//Check Ack
	if(ack == 0){
		//Failure break
		return -1;
	}

	return 1; //this shows that send was a success
}//end of function

int sendFunctionIdentifier(int identity){
	int statusOfSend;

	//Send the unique function identifier
	puts("Sending function identifier");
	statusOfSend = sendToServer(sock, 1, NULL, identity, NULL, 0);
	if(statusOfSend < 0){
		puts("Send failed");
		return -1;
	}
	
	return 0;
}

int send2DMatrixToServer(int sock, int integerArray[],int row, int column)
{
	//Declare variables
	int statusOfSend;

	//Send number of rows
	statusOfSend = sendToServer(sock, 1, NULL, row, NULL, 0);
	if(statusOfSend < 0){
		//Failure break
		return 0;
	}

	//Send number of columns
	statusOfSend = sendToServer(sock, 1, NULL, column, NULL, 0);
	if(statusOfSend < 0){
		//Failure break
		return 0;
	}
	
	//size has been successfully sent
	statusOfSend = sendToServer(sock, 2, integerArray, -999, NULL, (sizeof(int)*row*column));
	if(statusOfSend < 0){
		//Failure break
		return 0;
	}
	
	//Successful
	return 1;
}

void multiply(int *a, int *b, int n, int m, int l, int *c){
	
	if(sendFunctionIdentifier(3) == -1){
		puts("Sending Function Identifier Failed");
		return;
	}

	if(send2DMatrixToServer(sock, a, n, m) == 0){
		puts("Mulitplication failed");
		return;
	}
		
	if(send2DMatrixToServer(sock, b, m, l) == 0){
		puts("Mulitplication failed");
		return;
	}
	
	if(send2DMatrixToServer(sock, c, n, l) == 0){
		puts("Mulitplication failed");
		return;
	}

	//adding code here to receive the result array from the server
	while(1){
		//this while loop exists to receive the result array from the server
		int resultRow = 0, resultColumn = 0;

		int statusOfRead = recv(sock , &resultRow , sizeof(resultRow),0);
		if(statusOfRead > 0){
			printf("The row size of result array is = %d",resultRow);
		}else{
			puts("Error receiving result row.");
			break;
		}

		statusOfRead = recv(sock , &resultColumn , sizeof(resultColumn),0);
		if(statusOfRead > 0){
			printf("The column size of result array is = %d",resultColumn);
		}else{
			puts("Error receiving result column.");
			puts("The result array is:");
			break;
		}
		
		statusOfRead = recv(sock , c , sizeof(int)*resultRow*resultColumn,0);
		if(statusOfRead > 0){	
			return;	
		}//end of if(statusOfRead > 0)
		else{
			puts("Error receiving result array.");
			break;
		}
	}//end of while

}

int wc(char string[]){

	if(sendFunctionIdentifier(5) == -1){
		puts("Sending Function Identifier Failed");
		return -1;
	}

	int status = sendToServer(sock, 3, NULL, 0, string, sizeof(char)*2048);
	if(status < 0){
		printf("\nFailed");
		return -1;
	}else{
		int numWords = 0;
		while(1){
			int statusOfStringRead = recv(sock , &numWords , sizeof(numWords),0);
			if(statusOfStringRead > 0){
				printf("\nNumber of words = %d",numWords);
				return numWords;
			}else{
				printf("\nError receiving number of words.");
				return -1;
			}
		}
	}
}

int max(int size, int *intarray){
	int statusOfSend, statusOfRead, maxVal;

	if(sendFunctionIdentifier(1) == -1){
		puts("Sending Function Identifier Failed");
		return 0;
	}

	//Sending size
	puts("Sending Array Size");
	statusOfSend = sendToServer(sock, 1, NULL, size, NULL, 0);
	if(statusOfSend < 0){
		puts("Send failed");
		return 0;
	}
	
	puts("Sending array.");
	statusOfSend = sendToServer(sock , 2, intarray , -999 , NULL, sizeof(int)*size);
	if(statusOfSend < 0){
		puts("Send failed");
		return 0;
	}else{
		//Wait to receive max value
		puts("Waiting to recieve Max result.");
		while(1){
			statusOfRead = recv(sock , &maxVal , sizeof(maxVal),0);
			if(statusOfRead > 0){
				printf("Got result = %d",maxVal);
				return maxVal;
			}else{
				puts("Receive failed");
				return -1;
			}
		}
	}
}

int min(int size, int *intarray){
	int statusOfSend, statusOfRead, minVal;

	if(sendFunctionIdentifier(2) == -1){
		puts("Sending Function Identifier Failed");
		return 0;
	}
	
	//Sending size
	statusOfSend = sendToServer(sock, 1, NULL, size, NULL, 0);
	if(statusOfSend < 0){
		puts("Send failed");
		return 0;
	}

	statusOfSend = sendToServer(sock , 2, intarray , -999 , NULL, sizeof(int)*size);
	if(statusOfSend < 0){
		puts("Send failed");
		return 0;
	}else{
		//Wait to receive min value
		while(1){
			statusOfRead = recv(sock , &minVal , sizeof(minVal),0);
			if(statusOfRead > 0){
				return minVal;
			}else{
				puts("Receive failed");
				return -1;
			}
		}
	}
}

int * sort(int size, int* intarray){
	int statusOfSend, statusOfRead;

	if(sendFunctionIdentifier(4) == -1){
		puts("Sending Function Identifier Failed");
		return NULL;
	}

	//Sending size
	statusOfSend = sendToServer(sock, 1, NULL, size, NULL, 0);
	if(statusOfSend < 0){
		puts("Send failed");
		return NULL;
	}

	statusOfSend = sendToServer(sock , 2, intarray , -999 , NULL, sizeof(int)*size);
	if(statusOfSend < 0){
		puts("Send failed");
		return NULL;
	}else{
		//Accept result array and assign a pointer
		while(1){
			//this while loop exists to receive the result array from the server
			int resultSize = 0, statusOfRead;

			statusOfRead = recv(sock , &resultSize , sizeof(resultSize),0);
			if(statusOfRead > 0){
				printf("The size of the result array is = %d",resultSize);
			}else{
				puts("Error receiving result row.");
				return NULL;
			}

			statusOfRead = recv(sock , intarray, sizeof(int)*resultSize,0);
			if(statusOfRead > 0){	
				return intarray;	
			}
			else{
				puts("Error receiving result array.");
				return NULL;
			}
		}
	}
}

int main(){

	int statusOfSend, statusOfRead, queryResult;
    struct sockaddr_in server;
    
	//Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        puts("Could not create socket");
		return 1;
    }
    puts("Socket created");

	//Connect to Directory Register and get ip and port for service
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8000 );
	
	//Connect to Directory Service
	puts("Trying to connect to Directory Service");
	int connected = -1;
	while(connected == -1){
    	connected = connect(sock , (struct sockaddr *)&server , sizeof(server));
	}
    
	//Connected to Directory Service
    puts("Connected");
	
	//Let Directory know you are the client
	statusOfSend = sendToServer(sock, 1, NULL, 0, NULL, 0);
	if(statusOfSend < 0){
		puts("Send programID failed");
		close(sock);
		return 0;
	}
	
	//Send Lookup Request
	int ip[4];
	int port;
	
	//Send Program ID
	statusOfSend = sendToServer(sock, 1, NULL, programID, NULL, 0);
	if(statusOfSend < 0){
		puts("Send programID failed");
		close(sock);
		return 0;
	}
	
	//Send Version
	statusOfSend = sendToServer(sock, 1, NULL, version, NULL, 0);
	if(statusOfSend < 0){
		puts("Send Version failed");
		close(sock);
		return 0;
	}
	
	//Get Status of Server Found or Not
	statusOfRead = recv(sock , &queryResult , sizeof(queryResult),0);
	if(statusOfRead < 0){
		puts("Receive Query Result failed");
		close(sock);
		return 0;
	}else{
		if(queryResult == 0){
			puts("Server Not Found");
			close(sock);
			return 0;
		}
	}

	//Send the ACK
	if(sendAck(sock) < 0){
		puts("Send ACK failed");
		close(sock);
		return 0;
	}

	//Get IP
	statusOfRead = recv(sock , &ip , sizeof(int)*4,0);
	if(statusOfRead < 0){
		puts("Receive IP failed");
		close(sock);
		return 0;
	}
	
	//Send the ACK
	if(sendAck(sock) < 0){
		puts("Send ACK failed");
		close(sock);
		return 0;
	}

	//Get Port
	statusOfRead = recv(sock , &port , sizeof(port),0);
	if(statusOfRead < 0){
		puts("Receive Port failed");
		close(sock);
		return 0;
	}

	//Convert ip to string
	char address[50];
	sprintf(address, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

	//Trim the char array
	
	//Close connection to Directory Service
	close(sock);
	
	//Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        puts("Could not create socket");
		return 1;
    }
    puts("Socket created");
	
	//Now setup Server connection
	server.sin_addr.s_addr = inet_addr(address);
    server.sin_family = AF_INET;
    server.sin_port = htons( port );
	
	//Connect to Remote Server
	puts("Trying to connect to Remote Server");
	connected = -1;
	while(connected == -1){
    	connected = connect(sock , (struct sockaddr *)&server , sizeof(server));
	}
    
	//Connected to Remote Server
    puts("Connected");

	while(1){
		//Call input in client file
		if(input() == -1){
			break;
		}
	}
	
	puts("Connection terminated.");
	close(sock);
	return 0;
}



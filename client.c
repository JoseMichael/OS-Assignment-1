/*

Operating Systems Project 1
Client Code
Jose Michael Joseph
Bhavin Modi

*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

int sendToServer(int, int, int *, int, char *, int); 
 
int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
    int maxValue; int sizeOfArray=0;
     
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
     
//--------------Quick Note/Self Reminder----------------
//"counter" (below) is an old variable I used to toggle between different sections
//Its use has since been depricated but I am still considering if there is any benefits
//That can be achieved by having it in the code and hence keeping it in for the time being
//In my mind this could be something useful that helps us toggle between different input
//methods when we take in different data types. Will have to think more on it
//---------------------------------------------------------
    int counter = 1;

    //keep communicating with server



    while(1){
		//this is the first while, it's job was to initially direct the flow to different sections of the code
	    //Now its sort of useless. Until further brain storming that is.
		
		if(counter==0){

		    printf("Enter the size of the array : ");
		    scanf("%d" , &sizeOfArray);
		    //we now create an array based on that size.
		    //Please note the scope of that array would be limited to the scope of the above "if"
			int integerArray[sizeOfArray];
			printf("Please enter the various array elements \n");
			int j = 0;
		
			for(j=0; j<sizeOfArray; j++){
				//this for loop is used to take in the various elements for the array
				scanf("%d",&integerArray[j]);
			}

	
			//sending size first
			//printf("Checkpoint Inside Send ka Size ka if \n");
			//int statusOfSend = send(sock , &sizeOfArray , sizeof(sizeOfArray) , 0);
			//testing new sending function with send to server function
			int statusOfSend = sendToServer(sock, 1, NULL, sizeOfArray, NULL, 0);

			//int statusOfSend = send(sock , &valToTestAndDiscard , sizeof(valToTestAndDiscard) , 0);
			if( statusOfSend < 0){
				puts("Send failed");
				return 1;
			}
			else if(statusOfSend>0){
				while(1){
					//this while is used to send the array to the server

					//adding code here to send array
					int statusOfSend2 = send(sock , integerArray , sizeof(integerArray) , 0);
					if( statusOfSend2 < 0){
						puts("Send failed");
						return 1;
					}
					else if(statusOfSend2>0){
						//this section is entered when the array has been sent successfully
						//we will now have to look for the result of that operation

						printf("Array has been sent \n");
						counter = counter + 1;

						//adding code here to receive the max value from the server
						while(1){
							//this while loop exists to receive the max value from the server
							int statusOfRead = recv(sock , &maxValue , sizeof(maxValue),0);
							if(statusOfRead > 0){
								//this means the max value has been returned to the client
								printf("The max value in the passed array was %d \n",maxValue);
								break;
								//this breaks from the second internal while
							}//end of if
						}//end of while


						break;
						//this is supposed to break from the internal while
					}

				}//end of while
			}//end of else if
		}
		else if(counter==1){
			//this counter is going to be used for taking input for matrix multiplication
			//that requires three matrices; initially coding for serializing and sending one
			int rowA=0,columnA=0,rowB=0,columnB=0;

			printf("\nPerforming Matrix Multiplication AxB");
			printf("\nEnter Matrix A:");
			printf("\n\tPlease enter the number for rows: ");
			scanf("%d",&rowA);
			printf("\n\tPlease enter the number for columns: ");
			scanf("%d",&columnA);
			if(send2DMatrixToServer(sock, rowA, columnA) == 0){
				printf("\nSend A Failed");
				return 0;
			}else{
				printf("\nEnter Matrix B:");
				printf("\n\tPlease enter the number for rows: ");
				scanf("%d",&rowB);
				printf("\n\tPlease enter the number for columns: ");
				scanf("%d",&columnB);

				if(columnA != rowB){
					printf("\nInvalid Parameters");
					return 0;
				}

				if(send2DMatrixToServer(sock, rowB, columnB) == 0){
					printf("\nSend B Failed");
					return 0;
				}else{
					//Initilaize result array to zeroes and send it over.
					int resultArray[rowA][columnB];
					int rowC = 0, columnC = 0;
					for(rowC = 0; rowC < rowA; rowC++){
						for(columnC = 0; columnC < columnB; columnC++){
							resultArray[rowC][columnC] = 0;
						}
					}

					if(send2DMatrixToServer(sock, rowC, columnC) == 0){
						printf("\nSend C Failed");
						return 0;
					}else{
						//adding code here to receive the result array from the server
						while(1){
							//this while loop exists to receive the result array from the server
							int resultRow = 0, resultColumn = 0;

							int statusOfRead = recv(sock , &resultRow , sizeof(resultRow),0);
							if(statusOfRead > 0){
								printf("\nThe row size of result array is = %d",resultRow);
							}else{
								printf("\nError receiving result row.");
								break;
							}

							statusOfRead = recv(sock , &resultColumn , sizeof(resultColumn),0);
							if(statusOfRead > 0){
								printf("\nThe column size of result array is = %d",resultColumn);
							}else{
								printf("\nError receiving result column.");
								break;
							}
							
							int matrixC[resultRow][resultColumn];
							statusOfRead = recv(sock , &matrixC , (sizeof(matrixC)/sizeof(resultRow)),0);
							if(statusOfRead > 0){
								printf("\nThe result array is:");
								for(rowC = 0; rowC < resultRow; rowC++){
									printf("\n");
									for(columnC = 0; columnC < resultColumn; columnC++){
										printf("%d\t",matrixC[rowC][columnC]);
									}
								}
							}else{
								printf("\nError receiving result array.");
								break;
							}
						}//end of while
					}
				}
			}

			
		}//end of else if
      
	}//end of while

    close(sock);
    return 0;
}

int send2DMatrixToServer(int sock, int row, int column)
{
	int statusOfRowSizeSend = sendToServer(sock, 1, NULL, row, NULL, 0);
	int statusOfColSizeSend = sendToServer(sock, 1, NULL, column, NULL, 0);
	int serializedSize = row*column;
	int integerArray[serializedSize];

	printf("Please enter the values for the array");
	int num=0;
	for(num=0; num<serializedSize; num++)
	{
		scanf("%d",&integerArray[num]);
	}

	printf("Serialized size is %d \n",serializedSize);
	
	printf("Size of array is %d \n",(int)(sizeof(integerArray)));


	if(statusOfRowSizeSend>0 && statusOfColSizeSend>0)
	{


	
		//size has been successfully sent
		int statusOfArraySend = sendToServer(sock, 2, integerArray, -999, NULL, sizeof(integerArray));
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

int sendToServer(int sock, int typeOfData, int *intArray, int integerToSend, char *stringToSend, int sizeOfData)
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

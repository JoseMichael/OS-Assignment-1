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

//int **integerArray;
int sizeOfArray=0;
    while(1)
    {

 if(counter==0)
	{

        printf("Enter the size of the array : ");
        scanf("%d" , &sizeOfArray);
	//int integerArray[sizeOfArray];
	//integerArray = malloc(sizeof(int)*sizeOfArray);
	//int integerArray[sizeOfArray];
	//trying fixed size; experimentation only; need to delete====================================
	int integerArray[10];
	printf("Please enter the various array elements \n");
	int j = 0;
	for(j=0; j<sizeOfArray; j++)
	{
	scanf("%d",&integerArray[j]);
	}
	int valToTestAndDiscard = 15;	
	
	//sending size first
	printf("Checkpoint Inside Send ka Size ka if \n");
	int statusOfSend = send(sock , &sizeOfArray , sizeof(sizeOfArray) , 0);
	//int statusOfSend = send(sock , &valToTestAndDiscard , sizeof(valToTestAndDiscard) , 0);
		if( statusOfSend < 0)
		{
		    puts("Send failed");
		    return 1;
		}
		else if(statusOfSend>0)
		{
			while(1)
			{

				//adding code here to send array
				printf("Here the send is gonna send an array with size of %d \n", (int)sizeof(integerArray));
				int statusOfSend2 = send(sock , integerArray , sizeof(integerArray) , 0);
				if( statusOfSend2 < 0)
				{
				    puts("Send failed");
				    return 1;
				}
				else if(statusOfSend2>0)
				{
					printf("Array has been sent \n");
					counter = counter + 1;
					break;
					//this is supposed to break from the internal while
				}

			}//end of while
		}
	}
	else if(counter==1)
	{/*
	
		printf("Checkpoint Inside Send ka Old Array ka if \n");
		//int statusOfSend = send(sock , message , strlen(message) , 0);
		//int statusOfSend2 = send(sock , integerArray , sizeOfArray , 0);
		int statusOfSend2 = send(sock , &sizeOfArray , sizeof(sizeOfArray) , 0);
			if( statusOfSend2 < 0)
			{
			    puts("Send failed");
			    return 1;
			}
			else if(statusOfSend2>0)
			{
				counter = counter + 1;
			}
*/
	}//end of else if
      }//end of while

         

    
     
    close(sock);
    return 0;
}
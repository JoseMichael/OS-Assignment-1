#include "ezxml.h"
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct 
{
	char value[100];
}charHolder;

struct remember
{
	char dataType[20];
	struct remember *next;
}*head, *curr;

int serviceNo, versionNo;

void addToRemember(char dataType[]){
	struct remember *ptr = (struct remember*)malloc(sizeof(struct remember));

	if(NULL == ptr){
		puts("Node Creation Failed");
		return;
	}

	strcpy(ptr->dataType, dataType);
	ptr->next = NULL;
	
	if(NULL == head){
		head = curr = ptr;
	}else{
		curr->next = ptr;
		curr = ptr;
	}
}

int searchRemember(char dataType[]){
	int found = 0;

	struct remember *ptr = head;
	
	while(ptr != NULL){
		if(strcmp(ptr->dataType,dataType) == 0){
			found = 1;
			break;
		}
		ptr = ptr->next;
	}

	if(found == 0){
		return -1;
	}else{
		return 1;
	}
}

void emptyRemember(){
	struct remember *ptr = head;
	struct remember *prev = head;
	
	while(ptr != NULL){
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}

	head = NULL;
}

int write(char input[], char output[]){
	char buff[512];
	//open files
	FILE *fp1, *fp2;

	fp1 = fopen(input,"r");
	fp2 = fopen(output,"a");
	
	fputs("\n",fp2);
	
	//read a line from input file
	while(fgets(buff,512,fp1) != NULL){
		//write the line to output file
		fputs(buff,fp2);
	}
	
	//close
	fclose(fp1);
	fclose(fp2);
	
	return 1;
}

int writeToFile(int type, char dataType[]){
	switch(type){
		case 0:
			//Depending on datatype write the function
			if(strcmp(dataType,"int") == 0){
				//Integer
				if(write("sendint.c","client_stub.c") < 0){
					puts("Write Failed");
					return -1;
				}
			}else if(strcmp(dataType,"int*") == 0){
				//integer Array
				if(write("sendintarray.c","client_stub.c") < 0){
					puts("Write Failed");
					return -1;
				}
			}else if(strcmp(dataType,"char*") == 0){
				if(write("sendstring.c","client_stub.c") < 0){
					puts("Write Failed");
					return -1;
				}
			}else{
				//Data Type Not supported
				puts("Unsupported Data Type");
				return -1;
			}
		break;
		case 1:
			//Depending on datatype write the function
			if(strcmp(dataType,"int") == 0){
				//Integer
				if(write("readint.c","client_stub.c") < 0){
					puts("Write Failed");
					return -1;
				}
			}else if(strcmp(dataType,"int*") == 0){
				//Check if read int is needed
				if(searchRemember("int") < 0){
					//Not Found Integer
					addToRemember("int");
					if(write("readint.c","client_stub.c") < 0){
						puts("Write Failed");
						return -1;
					}
				}
				
				//integer Array
				if(write("readintarray.c","client_stub.c") < 0){
					puts("Write Failed");
					return -1;
				}
			}else if(strcmp(dataType,"char*") == 0){
				if(write("readstring.c","client_stub.c") < 0){
					puts("Write Failed");
					return -1;
				}
			}else{
				//Data Type Not supported
				puts("Unsupported Data Type");
				return -1;
			}
		break;
	}
	
	//Successful
	return 1;
}

void restructure(char *saver){
	const char delim[1] = "*";
	char temp[100];
	char *token;
	int i = 0;
	
	token = strtok(saver, delim);
	while(token != NULL){
		if(i == 0){
			strcpy(temp,token);
		}
		token = strtok(NULL, delim);
	}
	strcpy(saver,temp);
}

void writeFunc(char def[], int fid){
	FILE *fp;
	const char delim1[3] = "(,)";
	const char delim2[1] = " ";
	int saveCount = 0;
	char intSaver[100];
	char receive[100], functionId[100], temp[50], trackerInt[50], trackerChar[50];
	char *token, *sepInt, *endstr1, *endstr2;
	int i;
	
	//Delete the file (if exists)
	remove("temp.c");
	
	//Parse definition and write to file
	fp = fopen("temp.c","a");

	//Write definition
	fputs(def,fp);
	fputs("{\n",fp);
	
	//Split into send and receive
	token = strtok_r(def, delim1,&endstr1);
	i = 0;
	while(token != NULL){
		if(i == 0){
			//Save receive token
			strcpy(receive,token);

			//Write function Indentifier send
			if(strstr(receive,"int*") != NULL || strstr(receive,"char*") != NULL){
				sprintf(functionId,"%s(%d)%s","if(sendInt",fid,"<0){\nputs(\"Send Failure\");\nreturn NULL;\n}\n");
			}else{
				sprintf(functionId,"%s(%d)%s","if(sendInt",fid,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
			}
			fputs(functionId,fp);
			fputs("\n",fp);
		}else{
			//Add send code				
			if(strstr(token,"int*") != NULL){
				//Break the token
				sepInt = strtok_r(token,delim2,&endstr2);
				while(sepInt != NULL){
					strcpy(temp,sepInt);
					sepInt = strtok_r(NULL, delim2,&endstr2);
				}				
		
				//remeber the variable
				strcpy(trackerInt,temp);
				
				if(strstr(receive,"int*") != NULL || strstr(receive,"char*") != NULL){
					sprintf(functionId,"%s(%s,%s)%s","if(sendIntArray",intSaver,temp,"<0){\nputs(\"Send Failure\");\nreturn NULL;\n}\n");
				}else{
					sprintf(functionId,"%s(%s,%s)%s","if(sendIntArray",intSaver,temp,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
				}
				
				fputs(functionId,fp);
				fputs("\n",fp);
				
				//Clear buff
				saveCount = 0;
				strcpy(intSaver,"");
				
			}else if(strstr(token,"int") != NULL){
				//Break the token
				sepInt = strtok_r(token,delim2,&endstr2);
				while(sepInt != NULL){
					strcpy(temp,sepInt);
					sepInt = strtok_r(NULL, delim2,&endstr2);
				}

				//Add to a buff
				if(saveCount < 2){
					if(saveCount == 0){
						strcpy(intSaver,"");
						strcat(intSaver,temp);
						saveCount++;
					}else{
						strcat(intSaver,"*");
						strcat(intSaver,temp);
						saveCount++;
					}
				}else{
					restructure(intSaver);
					strcat(intSaver,"*");
					strcat(intSaver,temp);	
				}
				
				if(strstr(receive,"int*") != NULL || strstr(receive,"char*") != NULL){
					sprintf(functionId,"%s(%s)%s","if(sendInt",temp,"<0){\nputs(\"Send Failure\");\nreturn NULL;\n}\n");
				}else{
					sprintf(functionId,"%s(%s)%s","if(sendInt",temp,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
				}
				
				fputs(functionId,fp);
				fputs("\n",fp);
			}else if(strstr(token,"char*") != NULL){
				//Break the token
				sepInt = strtok_r(token,delim2,&endstr2);
				while(sepInt != NULL){
					strcpy(temp,sepInt);
					sepInt = strtok_r(NULL, delim2,&endstr2);
				}

				//remeber the variable
				strcpy(trackerChar,temp);

				if(strstr(receive,"int*") != NULL || strstr(receive,"char*") != NULL){
					sprintf(functionId,"%s(%d,%s)%s","if(sendString",2048,temp,"<0){\nputs(\"Send Failure\");\nreturn NULL;\n}\n");
				}else{
					sprintf(functionId,"%s(%d,%s)%s","if(sendString",2048,temp,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
				}
				
				fputs(functionId,fp);
				fputs("\n",fp);
			}
		}
		i++;
		token = strtok_r(NULL, delim1,&endstr1);
	}
	
	//Write Receive code
	if(strstr(receive,"int*") != NULL){
		//get array size, declare new integer
		fputs("int num1;\n",fp);
		sprintf(functionId,"%s(%s)%s","if(readInt","num1","<0){\nputs(\"Receive Failure\");\nreturn NULL;\n}\n");
		fputs(functionId,fp);		
	
		//declare new array
		//fputs("int tempArray[num1];\n",fp);
		sprintf(functionId,"%s(%s,%s)%s","if(readIntArray","num1",trackerInt,"<0){\nputs(\"Receive Failure\");\nreturn NULL;\n}\n");
		fputs(functionId,fp);
		sprintf(functionId,"%s %s;\n","return", trackerInt);
		fputs(functionId,fp);
	}else if(strstr(receive,"int") != NULL){
		//declare new integer
		fputs("int num1;\n",fp);
		sprintf(functionId,"%s(%s)%s","if(readInt","num1","<0){\nputs(\"Receive Failure\");\nreturn -1;\n}\n");
		fputs(functionId,fp);
		fputs("return num1;\n",fp);
	}else if(strstr(receive,"char*") != NULL){
		//fputs("char tempString[100];\n",fp);
		sprintf(functionId,"%s(%d,%s)%s","if(readstring",2048,trackerChar,"<0){\nputs(\"Receive Failure\");\nreturn NULL;\n}\n");
		fputs(functionId,fp);
		sprintf(functionId,"%s %s;\n","return", trackerChar);
		fputs(functionId,fp);
		//fputs("return tempString;\n",fp);
	}

	fputs("}",fp);
	
	//close file
	fclose(fp);
	
	//Write this file to the main stub
	write("temp.c","client_stub.c");

	//Remove temp file after use
	remove("temp.c");
}

int createClientStub(charHolder c[][6], int numRows){
	//has 6 columns

	//Write include files
	if(write("includeclientfiles.c","client_stub.c") < 0){
		puts("Header Files Write failed");
		return -1;
	}	

	//Write global variables
	FILE *fp;
	char tempBuff[100];
	remove("temp.c");
	fp = fopen("temp.c","a");
	sprintf(tempBuff,"%s %d%s\n","int programID = ",serviceNo,";");
	fputs(tempBuff,fp);
	sprintf(tempBuff,"%s %d%s\n","int version = ",versionNo,";");
	fputs(tempBuff,fp);
	fputs("int sock;\n",fp);
	fclose(fp);
	if(write("temp.c","client_stub.c") < 0){
		puts("Global Variables Write failed");
		return -1;
	}
	//Remove temp file after use
	remove("temp.c");
	
	puts("Check Point 1");
	//Check Data Types: For all Data types found, add the respective send functions
	int counter = 0;
	while(counter < numRows){		
		if(searchRemember(c[counter][5].value) > 0){
			//Found
			counter++;
			continue;
		}else{
			//Not Found
			addToRemember(c[counter][5].value);
			if(writeToFile(0, c[counter][5].value) < 0){
				puts("Client Stub creation failed");
				return -1;
			}
		}
		counter++;
	}

	puts("Check Point 2");

	emptyRemember();
	counter = 0;
	//Check Return Types and add the respective receive functions
	while(counter < numRows){		
		if(searchRemember(c[counter][2].value) > 0){
			//Found
			counter++;
			continue;
		}else{
			//Not Found
			addToRemember(c[counter][2].value);
			if(writeToFile(1, c[counter][2].value) < 0){
				puts("Client Stub creation failed");
				return -1;
			}
		}
		counter++;
	}
	
	//Write the function procedures, create definitions first
	counter = 0;
	int funcid, temp;
	char def[100], tempdef[50];
	while(counter < numRows){
		if(counter == 0){
			sscanf(c[counter][0].value,"%d",&funcid);

			//Add return type name ( param
			sprintf(def,"%s %s\(%s %s",c[counter][2].value,c[counter][1].value,c[counter][5].value,c[counter][3].value);
		}else{
			sscanf(c[counter][0].value,"%d",&temp);
			if(temp == funcid){
				//Still the same ID, add to definition
				sprintf(tempdef,",%s %s",c[counter][5].value,c[counter][3].value);
				strcat(def,tempdef);
			}else{
				//Finish Old function
				strcat(def,")");
				writeFunc(def,funcid);
				
				//Next function 
				funcid = temp;
				//Add return type name ( param
			sprintf(def,"%s %s\(%s %s",c[counter][2].value,c[counter][1].value,c[counter][5].value,c[counter][3].value);
			}
		}		
		counter++;
		if(counter == numRows){
			//Finish Old function
			strcat(def,")");
			writeFunc(def,funcid);
		}
	}	
	
	//Write the main
	if(write("clientmain.c","client_stub.c") < 0){
		puts("Main Function Write failed");
		return -1;
	}
	
	//Successful
	return 1;
}

int createHeaderFile(charHolder c[][6], int numRows){
	//Create definitions to put in header file
	int counter = 0, funcid, temp;
	char def[100], tempdef[50];
	
	remove("temp.c");
	FILE *fp;
	fp = fopen("temp.c","a");
	
	fputs("#ifndef RPCHEADER_H_\n",fp);
	fputs("#define RPCHEADER_H_\n",fp);
	
	while(counter < numRows){
		if(counter == 0){
			sscanf(c[counter][0].value,"%d",&funcid);

			//Add return type name ( param
			sprintf(def,"%s %s\(%s %s",c[counter][2].value,c[counter][1].value,c[counter][5].value,c[counter][3].value);
		}else{
			sscanf(c[counter][0].value,"%d",&temp);
			if(temp == funcid){
				//Still the same ID, add to definition
				sprintf(tempdef,",%s %s",c[counter][5].value,c[counter][3].value);
				strcat(def,tempdef);
			}else{
				//Finish Old function
				strcat(def,");\n");
				fputs(def,fp);
				
				//Next function 
				funcid = temp;
				//Add return type name ( param
			sprintf(def,"%s %s\(%s %s",c[counter][2].value,c[counter][1].value,c[counter][5].value,c[counter][3].value);
			}
		}		
		counter++;
		if(counter == numRows){
			//Finish Old function
			strcat(def,");\n");
			fputs(def,fp);
		}
	}

	//Write our client input function definition
	fputs("int input();\n",fp);
	fputs("#endif",fp);
	
	fclose(fp);
	
	//Write Header
	if(write("temp.c","rpcheader.h") < 0){
		puts("Main Function Write failed");
		return -1;
	}
}

int createServerStub(charHolder c[][6], int numRows){
	printf("Checkpoint 0 \n");

	//Write include files
	if(write("includeserverfiles.c","server_stub.c") < 0){
		puts("Header Files Write failed");
		return -1;
	}
	
	
	printf("Checkpoint 1 \n");

	//Write global variables
	FILE *fp;
	char tempBuff[100];
	remove("temp.c");
	fp = fopen("temp.c","a");
	sprintf(tempBuff,"%s %d\n","#define programID",serviceNo);
	fputs(tempBuff,fp);
	sprintf(tempBuff,"%s %d\n","#define version",versionNo);
	fputs(tempBuff,fp);
	fputs("int sock;\n",fp);
	fclose(fp);
	if(write("temp.c","server_stub.c") < 0){
		puts("Global Variables Write failed");
		return -1;
	}
	//Remove temp file after use
	remove("temp.c");
	
	//Write the Before switch
	if(write("beforeswitch.c","server_stub.c") < 0){
		puts("beforeswitch Write failed");
		return -1;
	}
	
	puts("Server Check Point 1");

	//Temp file to write cases
	fp = fopen("temp.c","a");		

	//Write Switch Cases
	int counter = 0;
	int funcid = -1;
	int lastFunctId = -1;
	
	printf("Numrows is %d \n",numRows);
	
	while(counter < numRows)
	{
		sscanf(c[counter][0].value,"%d",&funcid);
			//puts("Server Check Point 2");

		if((funcid!=lastFunctId) && (lastFunctId!=-1))
		{
			puts("Server Check Point 3");
			//End old case and start a new case
			sprintf(tempBuff,"break; \n");
			fputs(tempBuff,fp);
			sprintf(tempBuff,"%s %d%s\n","case",funcid,":\n");
			fputs(tempBuff,fp);
			lastFunctId=funcid;
		}
		else if(funcid!=lastFunctId)
		{
			puts("Server Check Point 4");
			//start new case
			sprintf(tempBuff,"%s %d%s\n","case",funcid,":\n");
			fputs(tempBuff,fp);
			lastFunctId=funcid;
		}

		//get param
		if(strcmp(c[counter][5].value,"int") == 0)
		{
			puts("Server Check Point 5");
			int tempfuncid1, tempfuncid2;
			sscanf(c[counter+1][0].value,"%d",&tempfuncid1);
			sscanf(c[counter+2][0].value,"%d",&tempfuncid2);
			if(tempfuncid1==funcid)
			{
				if(strcmp(c[counter+1][5].value,"int*") == 0)
				{
					puts("Server Check Point 6");
					//add 1d array functions
					/*
					 * 	int size;
						int *intArr;
						int statusOfRecvParam1 = receiveArrayFromClient(sock, &size, &intArr);
						if(statusOfRecvParam1==0)
							break;
					 */
					 sprintf(tempBuff,"%s %s%s\n","int",c[counter][3].value,"; \n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s %s%s\n","int *",c[counter+1][3].value,"; \n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s %s%s %s %s%s%s%s %s\n","int","statusOfRecv",c[counter+1][3].value,"=","receiveArrayFromClient(sock, &",c[counter][3].value,", &",c[counter+1][3].value,");\n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s%s%s\n","if(statusOfRecv",c[counter+1][3].value,"==0)\n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s\n","break;\n");
					 fputs(tempBuff,fp);
					 
					 //check condition for when next funct id is not the same
					 int funcidToTest;
					 sscanf(c[counter+2][0].value,"%d",&funcidToTest);
					 if(funcidToTest!=funcid)
					 {
					 	
					 	
					 	/*
					 	 * 	int result = max(size,intArr);
							int statusOfValSent = sendToClient(sock, 1, NULL, result, NULL, 0);
							successOrFailedSend(statusOfValSent);
					 	 */
					 	//initiate procedures to return the value
					 	int counter2=0;
					 	char tempstr[100], deststr[100], testarr[100];
					 	int tfuncid;
					 	strcpy(deststr,"");
					 	sprintf(deststr,"%s(",c[counter][1].value);
					 	int someIndex = 0;
					 	while(counter2<numRows)
					 	{
					 		
					 		sscanf(c[counter2][0].value,"%d",&tfuncid);
					 		if(tfuncid==funcid)
					 		{
					 			if(someIndex == 0){
					 				someIndex++;
					 				sprintf(tempstr,"%s",c[counter2][3].value);
					 				strcat(deststr,tempstr);
					 			}else{
					 				sprintf(tempstr,",%s",c[counter2][3].value);
					 				strcat(deststr,tempstr);
					 			}
					 		}
					 		//strcat(testarr,deststr);
					 		counter2++;
					 	}
					 	strcat(deststr,")");
					 	printf("The final values for it are %s \n",deststr);
					 	
					 	//printing to file
					 	sprintf(tempBuff,"%s %s%s%s\n",c[counter+1][2].value,"result = ",deststr,"; \n");
					 	fputs(tempBuff,fp);
 	
					 }
					counter = counter + 2;
				}
				else if(strcmp(c[counter+1][5].value,"int") == 0)
				{
					puts("Server Check Point 7");
					//add code to check if next variable is array
					if((strcmp(c[counter+2][5].value,"int*") == 0)&&(tempfuncid2==funcid))
					{
						puts("Server Check Point 77");
					//this means next param is an int* and belongs to same function
					//add code to set up functions for 3d array
					
					/*
					 *  int rowSize1,colSize1;
						int *matrix1;

						int statusOfParam1 =recv2DArrays(sock, &rowSize1, &colSize1, &matrix1);

						if(!(statusOfParam1==1))
						{
							break;
						}
					 */
					 
					 sprintf(tempBuff,"%s %s%s","int",c[counter][3].value,"; \n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s %s%s","int",c[counter+1][3].value,";\n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s%s%s","int *",c[counter+2][3].value,";\n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s %s%s %s %s%s%s%s%s","int","statusOf",c[counter+2][3].value,"=","recv2DArrays(sock, &",c[counter][3].value," &",c[counter+1][3].value,"); \n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s%s%s","if(statusOf",c[counter+2][3].value," ==0) \n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s\n","break;\n");
					 fputs(tempBuff,fp);
					 sprintf(tempBuff,"%s%s%s","successOrFailedSend(statusOf",c[counter+2][3].value,"); \n");
					 //successOrFailedSend(statusOfValSent);
					
					
					 //check condition for when next funct id is not the same
					 int funcidToTest;
					 sscanf(c[counter+3][0].value,"%d",&funcidToTest);
					 if(funcidToTest!=funcid)
					 {
					 	
					 	
					 	/*
					 	 * 	int result = max(size,intArr);
							int statusOfValSent = sendToClient(sock, 1, NULL, result, NULL, 0);
							successOrFailedSend(statusOfValSent);
					 	 */
					 	//initiate procedures to return the value
					 	int counter2=0;
					 	char tempstr[100], deststr[100], testarr[100];
					 	int tfuncid;
					 	strcpy(deststr,"");
					 	sprintf(deststr,"%s(",c[counter][1].value);
					 	int someIndex = 0;
					 	while(counter2<numRows)
					 	{
					 		
					 		sscanf(c[counter2][0].value,"%d",&tfuncid);
					 		if(tfuncid==funcid)
					 		{
					 			if(someIndex == 0){
					 				someIndex++;
					 				sprintf(tempstr,"%s",c[counter2][3].value);
					 				strcat(deststr,tempstr);
					 			}else{
					 				sprintf(tempstr,",%s",c[counter2][3].value);
					 				strcat(deststr,tempstr);
					 			}
					 		}
					 		//strcat(testarr,deststr);
					 		counter2++;
					 	}
					 	strcat(deststr,")");
					 	printf("The final values for it are %s \n",deststr);
					 	
					 	//printing to file
					 	sprintf(tempBuff,"%s %s%s%s\n",c[counter+1][2].value,"result = ",deststr,"; \n");
					 	fputs(tempBuff,fp);
 	
					 }
					
					
					counter = counter + 3;
					}

				}
				else
				{
					puts("Server Check Point 8");
					//add code to put in int functions
					counter = counter + 1;
					/*
					 * int statusOfCounterRecv = recvFromClient(sock, 1, NULL, &counter, NULL, (int)sizeof(int));
					 */
					 sprintf(tempBuff,"%s %s%s%s%s%s\n","int","statusOf",c[counter][3].value,"Recv = recvFromClient(sock, 1, NULL, &",c[counter][3].value,", NULL, 0);\n");
					 fputs(tempBuff,fp);
					 int funcidToTest;
					 sscanf(c[counter+2][0].value,"%d",&funcidToTest);
					 if(funcidToTest!=funcid)
					 {
					 	//initiate procedures to return the value
					 }
				}
			}
			else
			{
				puts("Server Check Point 9");
			//add code to put in int functions since next row is another function
			
			/*
			 * int recvRowSizeStatus = recvFromClient(sock, 1, NULL, rowSize, NULL, 0);
			 * int result = funct(int dsfdsf);		
			 * int statusOfSizeSend = sendToClient(sock, 1, NULL, sizeOfArray, NULL, 0);
					successOrFailedSend(statusOfSizeSend);
						 */
				sprintf(tempBuff,"%s%s%s%s%s","int statusOfRecv",c[counter][3].value," = recvFromClient(sock, 1, NULL, ",c[counter][3].value,", NULL, 0); \n");
				fputs(tempBuff,fp);
				sprintf(tempBuff,"%s%s%s%s%s%s",c[counter][2].value," result = ",c[counter][1].value,"(",c[counter][3].value,"); \n");
				fputs(tempBuff,fp);
				sprintf(tempBuff,"%s%s%s","int statusOfSend",c[counter][3].value," = sendToClient(sock, 1, NULL, result, NULL, 0); \n");
				fputs(tempBuff,fp);
				sprintf(tempBuff,"%s%s%s","successOrFailedSend(statusOfSend",c[counter][3].value,"); \n");
				fputs(tempBuff,fp);
				
				
			
			
			
			counter = counter + 1;
			}
		}//end of if it was int
		else if(strcmp(c[counter][5].value,"char*") == 0)
		{
			puts("Server Check Point 10");
			//add code to put in string functions
			/*
			 * 	char string[2048];
				int status = recvFromClient(sock, 3, NULL, 0, string, 2048*sizeof(char));
				if(status==0)
					break;
				int result = wc(string);
				int statusOfValSent =  sendToClient( sock, 1, NULL, result, NULL, sizeof(int));
				successOrFailedSend(statusOfValSent);
			 */
			sprintf(tempBuff,"%s%s%s","char ",c[counter][3].value,"[2048]; \n");
			fputs(tempBuff,fp);
			sprintf(tempBuff,"%s%s%s%s%s","int statusOf ",c[counter][3].value," = recvFromClient(sock, 3, NULL, 0, ",c[counter][3].value,", 2048*sizeof(char)); \n");
			fputs(tempBuff,fp);
			sprintf(tempBuff,"%s%s%s","if (statusOf",c[counter][3].value,"==0)\n");
			fputs(tempBuff,fp);
			sprintf(tempBuff,"%s","break;\n");
			fputs(tempBuff,fp);
			
			
			 int funcidToTest;
					 sscanf(c[counter+1][0].value,"%d",&funcidToTest);
					 if(funcidToTest!=funcid)
					 {
					 	
					 	
					 	/*
					 	 * 	int result = max(size,intArr);
							int statusOfValSent = sendToClient(sock, 1, NULL, result, NULL, 0);
							successOrFailedSend(statusOfValSent);
					 	 */
					 	//initiate procedures to return the value
					 	int counter2=0;
					 	char tempstr[100], deststr[100], testarr[100];
					 	int tfuncid;
					 	strcpy(deststr,"");
					 	sprintf(deststr,"%s(",c[counter][1].value);
					 	int someIndex = 0;
					 	while(counter2<numRows)
					 	{
					 		
					 		sscanf(c[counter2][0].value,"%d",&tfuncid);
					 		if(tfuncid==funcid)
					 		{
					 			if(someIndex == 0){
					 				someIndex++;
					 				sprintf(tempstr,"%s",c[counter2][3].value);
					 				strcat(deststr,tempstr);
					 			}else{
					 				sprintf(tempstr,",%s",c[counter2][3].value);
					 				strcat(deststr,tempstr);
					 			}
					 		}
					 		//strcat(testarr,deststr);
					 		counter2++;
					 	}
					 	strcat(deststr,")");
					 	printf("The final values for it are %s \n",deststr);
					 	
					 	//printing to file
					 	sprintf(tempBuff,"%s %s%s%s\n",c[counter][2].value,"result = ",deststr,"; \n");
					 	fputs(tempBuff,fp);
					 	
					 	
					 	/*
					 	 * 			int result = wc(string);
									int statusOfValSent =  sendToClient( sock, 1, NULL, result, NULL, sizeof(int));
									successOrFailedSend(statusOfValSent);
					 	 */
					 	 sprintf(tempBuff,"%s%s%s","int statusOfSent",c[counter][3].value," = sendToClient( sock, 1, NULL, result, NULL, sizeof(int));); \n");
					 	 fputs(tempBuff,fp);
					 	 sprintf(tempBuff,"%s%s%s","successOrFailedSend(statusOfSent",c[counter][3].value,"); \n");
					 	 fputs(tempBuff,fp); 
 	
					 }
					
			
			
			
			counter = counter + 1;
		}
		else
		{
			//***************INCORRECT IDL SPECIFICATION****************
			printf("testtttt \n");
		}
		
	}//end of while
puts("Server Check Point 4s");
	
	//Write the after switch
	if(write("afterswitch.c","server_stub.c") < 0){
		puts("afterswitch Write failed");
		return -1;
	}
	
	//Successful
	return 1;
}

int main()
{
	
//code to count the number of params
	ezxml_t f2 = ezxml_parse_file("progidl.xml"),proc,parame,srNo,vNo;

srNo = ezxml_child(f2, "id");
vNo = ezxml_child(f2, "version");
sscanf(srNo->txt,"%d",&serviceNo);
sscanf(vNo->txt,"%d",&versionNo);

printf("Service Number is %d \n",serviceNo);
printf("Version Number is %d \n",versionNo);

int countOfParams=0;
//printf("Initial value of countOfParams is %d \n",countOfParams);

for (proc = ezxml_child(f2, "procedure"); proc; proc = proc->next) {
    for (parame = ezxml_child(proc, "param"); parame; parame = parame->next) {
        countOfParams = countOfParams + 1;
    }
}
ezxml_free(f2);

printf("Total number of params are %d \n",countOfParams);


//building struct to hold table of prog info
charHolder c[countOfParams][6];

//Jose implementation
	ezxml_t f1 = ezxml_parse_file("progidl.xml"),procedure,param;

 int row=-1;
for (procedure = ezxml_child(f1, "procedure"); procedure; procedure = procedure->next) {

    for (param = ezxml_child(procedure, "param"); param; param = param->next) {
    	row = row + 1;
        //printf("%s: %s: %s: %s: %s: %s:\n", ezxml_child(procedure, "id")->txt,ezxml_child(procedure, "name")->txt, ezxml_child(procedure, "return")->txt, ezxml_child(param, "name")->txt, ezxml_child(param, "index")->txt,ezxml_child(param, "type")->txt);
               strncpy(c[row][0].value,ezxml_child(procedure, "id")->txt,99);
               strncpy(c[row][1].value,ezxml_child(procedure, "name")->txt,99);
               strncpy(c[row][2].value,ezxml_child(procedure, "return")->txt,99);
               strncpy(c[row][3].value,ezxml_child(param, "name")->txt,99);
               strncpy(c[row][4].value,ezxml_child(param, "index")->txt,99);
               strncpy(c[row][5].value,ezxml_child(param, "type")->txt,99);
    }
}
ezxml_free(f1);


//function to display the table
printf("Printing table \n");
int i=0;
int j=0;
for(i=0; i<countOfParams; i++)
{
	for(j=0; j<6; j++)
	{
		printf("%s ",c[i][j].value);
	}
	printf("\n");
}
	
	//Remove an existing header file
	remove("rpcheader.h");
	//Create header file
	if(createHeaderFile(c, countOfParams) < 0){
		puts("Header File Generation Failed");
		return 1;
	}

	//Set Linked List head to Null
	head = curr = NULL;
	//Remove an existing client stub
	remove("client_stub.c");
	//Create Client Stub
	if(createClientStub(c, countOfParams) < 0){
		puts("Client Stub Generation Failed");
		return 1;
	}

	//Remove existing server stub
	remove("server_stub.c");
	puts("Here");
	//Create Server stub
	if(createServerStub(c, countOfParams) < 0){
		puts("Server Stub Generation Failed");
		return 1;
	}
	
	return 0;
}

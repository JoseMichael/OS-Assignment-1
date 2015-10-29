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
	if(head == NULL){
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
				//integer Array
				if(write("readintarray.c","client_stub.c") < 0){
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

void writeFunc(char def[], int fid){
	puts(def);
	FILE *fp;
	const char delim1[3] = "(,)";
	const char delim2[1] = " ";
	int saveCount = 0;
	char intSaver[100];
	char receive[100], functionId[100], temp[50];
	char *token, *sepInt, *endstr1, *endstr2;
	int i;
	
	//Delete the file (if exists)
	remove("temp.c");
	
	//Parse definition and write to file
	fp = fopen("temp.c","a");

	//Write definition
	fputs(def,fp);
	fputs("{\n",fp);
	
	//Write function Indentifier send
	sprintf(functionId,"%s(%d)%s","if(sendInt",fid,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
	fputs(functionId,fp);
	fputs("\n",fp);
	
	//Split into send and receive
	token = strtok_r(def, delim1,&endstr1);
	i = 0;
	while(token != NULL){
		if(i == 0){
			//Save receive token
			strcpy(receive,token);
		}else{
			//Add send code				
			if(strstr(token,"int*") != NULL){
				//Break the token
				sepInt = strtok_r(token,delim2,&endstr2);
				while(sepInt != NULL){
					strcpy(temp,sepInt);
					sepInt = strtok_r(NULL, delim2,&endstr2);
				}				
				
				sprintf(functionId,"%s(%s,%s)%s","if(sendIntArray",intSaver,temp,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
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
				if(saveCount < 3){
					if(saveCount == 0){
						strcpy(intSaver,"");
						strcat(intSaver,temp);
						saveCount++;
					}else{
						strcat(intSaver,"*");
						strcat(intSaver,temp);
						saveCount++;
					}
				}
				
				sprintf(functionId,"%s(%s)%s","if(sendInt",temp,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
				fputs(functionId,fp);
				fputs("\n",fp);
			}else if(strstr(token,"char*") != NULL){
				//Break the token
				sepInt = strtok_r(token,delim2,&endstr2);
				while(sepInt != NULL){
					strcpy(temp,sepInt);
					sepInt = strtok_r(NULL, delim2,&endstr2);
				}

				sprintf(functionId,"%s(%d,%s)%s","if(sendString",2048,temp,"<0){\nputs(\"Send Failure\");\nreturn -1;\n}\n");
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
		sprintf(functionId,"%s(%s)%s","if(readInt","num1","<0){\nputs(\"Receive Failure\");\nreturn -1;\n}\n");
		fputs(functionId,fp);		
	
		//declare new array
		fputs("int tempArray[num1];\n",fp);
		sprintf(functionId,"%s(%s,%s)%s","if(readIntArray","num1","tempArray","<0){\nputs(\"Receive Failure\");\nreturn -1;\n}\n");
		fputs(functionId,fp);
		fputs("return tempArray;\n",fp);
	}else{
		//declare new integer
		fputs("int num1;\n",fp);
		sprintf(functionId,"%s(%s)%s","if(readInt","num1","<0){\nputs(\"Receive Failure\");\nreturn -1;\n}\n");
		fputs(functionId,fp);
		fputs("return num1;\n",fp);
	}

	fputs("}",fp);
	
	//close file
	fclose(fp);
	
	//Write this file to the main stub
	write("temp.c","client_stub.c");
}

int createClientStub(charHolder c[][6], int numRows){
	//has 6 columns

	//TODO:Write include files
	
	//TODO:Write global variables
	
	puts("Check Point 1");
	//Check Data Types: For all Data types found, add the respective send functions
	int counter = 0;
	while(counter < numRows){		
		if(searchRemember(c[counter][5].value) < 0){
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
		if(searchRemember(c[counter][2].value) < 0){
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

	//Set Linked List head to Null
	head = NULL;
	//Remove an existing client stub
	remove("client_stub.c");
	//Create Client Stub
	if(createClientStub(c, countOfParams) < 0){
		return 1;
	}

	
	return 0;
}

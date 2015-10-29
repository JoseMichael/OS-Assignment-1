#include "ezxml.h"
#include <stdio.h>
#include <string.h>

typedef struct 
{
	char value[100];
}charHolder;


int serviceNo, versionNo;

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









return 0;
}

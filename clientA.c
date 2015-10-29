#include<stdio.h>
#include "functions.h"

int input(){
	
	int choice;

	puts("\n\tMENU");
	puts("1. MAX");
	puts("2. MIN");
	puts("3. MULTIPLICATION");
	puts("4. SORTING");
	puts("5. WORD COUNT");
	puts("6. EXIT");
	puts("\tEnter your choice:");
	scanf("%d",&choice);
	
	switch(choice){
		case 1:
			{
				int size, i, maxVal;

				puts("Performing Max operation");
				puts("\tEnter the array size:");
				scanf("%d" , &size);

				int arr[size];
				puts("\tEnter the array elements:\n");
				for(i = 0; i < size; i++){
					scanf("%d",&arr[i]);
				}
				
				puts("Calling Stub Max");

				maxVal = max(size, arr);
				printf("\nThe maximum value in the array is: %d",maxVal);
			}
			break;
		case 2:
			{
				int size, i, minVal;

				puts("Performing Min operation");
				puts("\tEnter the array size:");
				scanf("%d" , &size);

				int array[size];
				puts("\tEnter the array elements:\n");
				for(i = 0; i < size; i++){
					scanf("%d",&array[i]);
				}
				
				minVal = min(size, array);
				printf("\nThe minimum value in the array is: %d",minVal);
			}
			break;
		case 3:
			{
				int rowA=0,columnA=0,rowB=0,columnB=0;
				int serializedSize, num;

				puts("Performing Matrix Multiplication:");
				puts("\tEnter Matrix 1:");
				puts("\tPlease enter the number for rows: ");
				scanf("%d",&rowA);
				puts("\tPlease enter the number for columns: ");
				scanf("%d",&columnA);
			
				serializedSize = rowA*columnA;
				int matrix1[serializedSize];
				puts("\tPlease enter the values for the array:\n");
			
				num=0;
				for(num=0; num<serializedSize; num++){
					scanf("%d",&matrix1[num]);
				}
			
				puts("\tEnter Matrix 2:");
				puts("\tPlease enter the number for rows: ");
				scanf("%d",&rowB);
				puts("\tPlease enter the number for columns: ");
				scanf("%d",&columnB);

				serializedSize = rowB*columnB;
				int matrix2[serializedSize];
				puts("\tPlease enter the values for the array:\n");
			
				num=0;
				for(num=0; num<serializedSize; num++){
					scanf("%d",&matrix2[num]);
				}

				//Initialize the result matrix
				int matrix3[rowA*columnB];
				//adding Jose's code here for initializing array to 0
				for(num=0; num<rowA*columnB; num++){
					matrix3[num]=0;
				}
				
				int rowC = rowA;
				int columnC = columnB;

				int *p=multiply(rowA, columnA, matrix1, rowB, columnB, matrix2, rowC, columnC, matrix3);
	
				puts("Result is:");
				for(num = 0; num < rowA*columnB; num++){
					printf("\n%d",*(p+num));
				}
			}
			break;
		case 4:
			{
				int size, i;

				puts("Performing sorting operation");
				puts("\tEnter the array size:");
				scanf("%d" , &size);

				int array[size];
				puts("\tEnter the array elements:\n");
				for(i = 0; i < size; i++){
					scanf("%d",&array[i]);
				}
					
				puts("Calling Stub Sort");

				int *result = sort(size, array);
				if(result == NULL){
					return -1;
				}
				puts("The sorted array is:");
				for(i = 0; i < size; i++){
					printf("\t%d",*(result+i));
				}
			}
			break;
		case 5:
			{
				//Sending a string across
				char string[2048], words;
				puts("Enter anything:");

				//flushing input stream
				int c;
				while((c = getchar()) != '\n' && c != EOF);

				fgets(string, 2048, stdin);
				printf("%s",string);

				int index = 0;
				for(index = 0; index < 2048; index++){
					if(string[index] == '\n'){
						string[index] = '\0';
						break;
					}				
				}
	
				words = wc(string);
				if(words == -1){
					puts("\nFailed");
					return -1;
				}
				printf("\nNumber of Words: %d",words);
			}
			break;
		case 6:
			puts("Exiting");
			return -1;
			break;
		default:
			puts("Invalid Choice");
			return -1;
	}

	return 0;
}


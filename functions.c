#include <stdio.h>
#include "functions.h"

int max(int size, int *intarray)
{
	int comparer = 0;
	int i=0;

		for(i=0; i<size; i++)
		{
			if(comparer<intarray[i])
			{
				comparer = intarray[i];
			}
		}
return comparer;
}

int min(int size, int *arr){
	int minimum = *arr;
	int i;

	for(i = 1; i < size; i++){
		if(*(arr+i) < minimum){
			minimum = *(arr+i);
		}
	}
	
	return minimum;
}

int * multiply(int row1, int col1, int *a, int row2, int col2, int *b,int row3, int col3, int *c){
    //Multiply a and b, store in c
	int i, j, k;

	for(i = 0; i < row1; i++){
		for(j = 0; j < col1; j++){
			for(k = 0; k < col2; k++){
				*(c+((i*col2)+k)) = *(c+((i*col2)+k)) + (*(a+((i*col1)+j)) * *(b+((j*col2)+k)));
			}
		}
	}
	return c;
}

int wc(char* str){
	int i = 0, words = 0, poss = 1;
	
	while(*(str+i) != '\0'){
		if(*(str+i) == ' '){
			poss = 1;
		}else{
			if(poss == 1){
				words++;
				poss = 0;
			}
		}
		i++;
	}

	return words;
}

void quicksort(int* array, int low, int high){
	int i = low;
	int j = high;
	int temp;
	
	int pivot = *(array+((low+high)/2));
	
	/*partition*/
	while(i <= j){
		while(*(array+i) < pivot)
			i++;

		while (*(array+j) > pivot)
			j--;

		if(i <= j){
			temp = *(array+i);
			*(array+i) = *(array+j);
			*(array+j) = temp;
			i++;
			j--;
		}
	}

	if(low < j){
		quicksort(array,low,j);
	}

	if(i < high){
		quicksort(array,i,high);
	}
}

int * sort(int size, int* array){
	quicksort(array,0,size-1);
	return array;
}


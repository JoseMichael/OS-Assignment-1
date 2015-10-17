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

void multiply(int *a, int *b, int n, int m, int l, int *c){
    //Multiply a and b, store in c
	int i, j, k;

	for(i = 0; i < n; i++){
		for(j = 0; j < m; j++){
			for(k = 0; k < l; k++){
				*(c+((i*l)+k)) = *(c+((i*l)+k)) + (*(a+((i*m)+j)) * *(b+((j*l)+k)));
			}
		}
	}
}

int wc(char str[]){
	int i = 0, words = 0, poss = 1;
	
	while(str[i] != '\0'){
		if(str[i] == ' '){
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

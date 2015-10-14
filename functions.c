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

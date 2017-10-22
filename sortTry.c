#include <stdlib.h>
#include <stdio.h>
#include "Util.h"

void insertionSort(char *array, int n){
 int d, t;
  for (int c = 1 ; c <= n - 1; c++) {
    d = c;
 
    while ( d > 0 && array[d] < array[d-1]) {
      t          = array[d];
      array[d]   = array[d-1];
      array[d-1] = t;
 
      d--;
    }
  }
}

int main() {
	char array[5] = {(char)5, (char)98, (char)65, (char)1, (char)6};
	printf("Before\n");
	for(int i = 0; i < 5; i++) {
		printf("%d\n", (char)array[i]);
	}
	insertionSort(array, 5);

	printf("After\n");
	for(int i = 0; i < 5; i++) {
		printf("%d\n", (char)array[i]);
	}
}
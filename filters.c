#include <stdlib.h>
#include <stdio.h>
#include "Util.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


int filter3[9] = {	1, 2, 1, 
					2, 4, 2, 
					1, 2, 1};
					
int filter5[25] = {	1, 4, 7, 4, 1,
					4, 16, 26, 16, 4,
					7, 26, 41, 26, 7,
					4, 16, 26, 16, 4,
					1, 4, 7, 4, 1};
					
void insertionSort(gray *array, int n){
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

void medianFilter(gray* graymap, gray* result, int rows, int cols, int filterSize) {
	 
    int completeFilterSize = filterSize*filterSize;
	  for(int i=1; i < rows-1; i++){
	    for(int j=1; j < cols-1 ; j++){
	    	//float sum = 0;
	    	int filterMiddle = filterSize/2;
	    	gray *neighbors = malloc(completeFilterSize*sizeof(gray));
	    	for(int k = 0; k < filterSize; k++) {
	    		for(int t = 0; t < filterSize; t++) {
            int subCols = k - filterMiddle;
            int subRows = t - filterMiddle;

	    			neighbors[k*(filterSize) + t] = graymap[(i + subCols) * cols + (j + subRows)];
	    		}
	    	}

	    	insertionSort(neighbors, completeFilterSize);
	    	result[i * cols + j] = neighbors[(filterSize*filterSize)/2 + 1];
	    	
	    }
	  }

}


void gaussianFilter(gray* graymap, gray* result, int rows, int cols, int filterSize) {

	 int *filter = (filterSize == 3 ) ? filter3 : filter5;
	 float divider = (filterSize == 3 ) ? 1.0/16 : 1.0/273;
	 
	  for(int i=1; i < rows-1; i++){
	    for(int j=1; j < cols-1 ; j++){
	    	float sum = 0;
	    	int filterMiddle = filterSize/2;
	    	//fprintf(stderr,"Filter center %d for size %d\n", filterMiddle, filterSize);
	    	for(int k = 0; k < filterSize; k++) {
	    		for(int t = 0; t < filterSize; t++) {
	    			int subCols = k - filterMiddle;
	    			int subRows = t - filterMiddle;
	    			
	    			sum += (int)graymap[(i + subCols) * cols + (j + subRows)] * filter[ k * filterSize  + t];
	    		}
	    	}
	    	
	    	result[i * cols + j] = (int)(sum * divider) > 255 ? 255 : (int)(sum * divider);
	    }
	  }

}


int main(int argc, char* argv[])
    {
    FILE* ifp;
    gray* graymap;
    int ich1, ich2, rows, cols, maxval=255, pgmraw;
    int i, j;



    /* Arguments */
    if ( argc != 5 ){
      printf("\nUsage: %s file \n\n", argv[0]);
      exit(0);
    }

	int filterSize = atoi(argv[2]);
	int numberOfIterations = atoi(argv[3]);
    int filterType = atoi(argv[4]);

    if(filterSize != 5 && filterSize != 3) {
        pm_erreur("error argument 2: filterSize has to be 3(3x3) or 5(5x5)\n");
        exit(1);
    }

    if(filterType != 0 && filterType != 1) {
        pm_erreur("error argument 4: filterSize has to be 0(gaussian) or 1(median)\n");
        exit(1);
    }

	fprintf(stderr,"Filter type %d Number of iterations %d \n", filterType ,  numberOfIterations);
	
    /* Opening */
    ifp = fopen(argv[1],"r");
    if (ifp == NULL) {
      printf("error in opening file %s\n", argv[1]);
      exit(1);
    }

    /*  Magic number reading */
    ich1 = getc( ifp );
    if ( ich1 == EOF )
        pm_erreur( "EOF / read error / magic number" );
    ich2 = getc( ifp );
    if ( ich2 == EOF )
        pm_erreur( "EOF /read error / magic number" );
    if(ich2 != '2' && ich2 != '5')
      pm_erreur(" wrong file type ");
    else
      if(ich2 == '2')
	pgmraw = 0;
      else pgmraw = 1;

    /* Reading image dimensions */
    cols = pm_getint( ifp );
    rows = pm_getint( ifp );
    maxval = pm_getint( ifp );

    /* Memory allocation  */
    graymap = (gray *) malloc(cols * rows * sizeof(gray));

    /* Reading */
    for(i=0; i < rows; i++)
      for(j=0; j < cols ; j++)
        if(pgmraw)
          graymap[i * cols + j] = pm_getrawbyte(ifp) ;
        else
          graymap[i * cols + j] = pm_getint(ifp);
          
   	//Filtering
   	
    //We create a results array
   	gray* graymapResult = (gray *) malloc(cols * rows * sizeof(gray));
   	for(int i=1; i < rows; i++){
	    for(int j=1; j < cols ; j++){
	    	graymapResult[i * cols + j] = graymap[i * cols + j];
    	}
    }
   	
   	
    //Applying filters
    if(filterType) {
        for(int i = 0 ; i < numberOfIterations ; i++){
            fprintf(stderr,"Smooth number %d \n", i );
            medianFilter(graymap, graymapResult, rows, cols, filterSize);
        }
    } else {
        for(int i = 0 ; i < numberOfIterations ; i++){
            fprintf(stderr,"Smooth number %d \n", i );
            gaussianFilter(graymap, graymapResult, rows, cols, filterSize);
        }
    }
	
    /* Writing:Without conversion */
    if(pgmraw)
    	printf("P5\n");
      //printf("P2\n");
    else
    	printf("P2\n");
      //printf("P5\n");

    printf("%d %d \n", cols, rows);
    printf("%d\n",maxval);

    for(i=0; i < rows; i++)
      for(j=0; j < cols ; j++)
        if(pgmraw)
	        printf("%c",graymapResult[i * cols + j]);
          //printf("%d ", graymap[i * cols + j]);
        else
    	    printf("%d ", graymapResult[i * cols + j]);
          //printf("%c",graymap[i * cols + j]);
        /*putc(graymap[i * cols + j],stdout);*/


      /* Closing */
      fclose(ifp);
      return 0;
}

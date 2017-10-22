#include <stdlib.h>
#include <stdio.h>
#include "Util.h"

/* prints histogram for testing purpose*/
void printHistogram(int *histogram, int lengthOfHistogram){
  int i, j;
  for (i = 0; i < lengthOfHistogram; i++) {
    fprintf( stderr,"%d \t|",i);
    for (j = 0; j < histogram[i]; j+=50) {
      fprintf( stderr,"*");
    }
    fprintf( stderr,"\n");
  }
}

/* creates an intensity histogram */
int *getIntensityHistogram(gray* graymap, int rows, int cols, int maxval){
	int i, j;
	int *intensityHistogram = (int *)calloc(maxval , sizeof(int));
	for(i=0; i < rows; i++)
		for(j=0; j < cols ; j++)
				intensityHistogram[graymap[i * cols + j]] += 1;

	return intensityHistogram;
}

/* makes histogram equalization on a givin image */
void equalizeHistogram( gray* graymap, int rows, int cols, int maxval){
    int *histogram = getIntensityHistogram(graymap, rows, cols,maxval);
    float *cdf = (float *)calloc(maxval , sizeof(float));
    int *newHistogram = (int *)calloc(maxval , sizeof(int));
    float accum = 0, levels = maxval;
    int nPixels = rows * cols;

    for(int i = 0; i < maxval; i++){
        accum += histogram[i];
        cdf[i] = (accum/nPixels) * levels;
        newHistogram[(int)cdf[i]] += histogram[i];
    }

    for(int i=0; i < rows; i++)
        for(int j=0; j < cols ; j++){
                graymap[i * cols + j]  = cdf[graymap[i * cols + j]];
        }

    free(cdf);
    free(newHistogram);
}

/* makes histogram strecthing on a givin image */
void stretchHistogram( gray* graymap, int rows, int cols, int maxval){
	int *intensityHistogram = getIntensityHistogram(graymap, rows, cols,maxval);
	int oldMin = -1, oldMax = -1, i ,j;
	for ( i = 0; i < maxval; i++) {
		if(intensityHistogram[i] != 0 && oldMin == -1){
				oldMin = i;
		}else if(intensityHistogram[ maxval - i - 1 ] != 0 && oldMax == -1){
				oldMax =  maxval - i - 1;
		}
	}

	for(i=0; i < rows; i++)
		for(j=0; j < cols ; j++){
				graymap[i * cols + j]  = (gray)( (float)(graymap[i * cols + j] - oldMin)/(oldMax - oldMin) * maxval);
		}

  free(intensityHistogram);
}

int main(int argc, char* argv[])
    {
    FILE* ifp;
    gray* graymap;
    int ich1, ich2, rows, cols, maxval=255, pgmraw;
    int i, j;
    int type;

    /* Arguments */
    if ( argc != 3){
      printf("\nUsage: %s file \n\n", argv[0]);
      exit(0);
    }

    /* Opening */
    ifp = fopen(argv[1],"r");
    type = atoi(argv[2]);

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
      else
        pgmraw = 1;

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

    if(type) {
        equalizeHistogram(graymap,rows, cols, maxval);
    } else {
  		  stretchHistogram(graymap,rows, cols, maxval);
    }

    /* Writing */
    if(pgmraw)
      printf("P5\n");
    else
      printf("P2\n");

    printf("%d %d \n", cols, rows);
    printf("%d\n",maxval);

    for(i=0; i < rows; i++)
      for(j=0; j < cols ; j++)
        if(pgmraw)
          printf("%c",graymap[i * cols + j]);
        else
          printf("%d ", graymap[i * cols + j]);


    /* Closing */
    fclose(ifp);
    return 0;
}

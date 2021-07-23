/*======================================**
 *Name        : pi.c
 *Author      : Dong Ningning
 *======================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lpmti.h"

typedef struct args{
  long ninterval;
  double *sum;
}Params_t;
Params_t p;
double f(double x){
  return 4/(1+x*x);
}
int tf (int t) { return t; }

void *ThreadEntryPoint(){
  long ninterval=p.ninterval;
  int myRank;
  int size;
  long i;

  double sum = 0;
  double h;

  MTI_Comm_rank(MTI_COMM_WORLD, &myRank);
  MTI_Comm_size(MTI_COMM_WORLD, &size);

  h=1.0/ninterval;

  for(i=myRank; i<ninterval; i+=size) {
    sum+=f((i+0.5)*h);
  }

  p.sum[myRank]=sum*h;

  MTI_thread_exit (myRank);
}

int main(int argc, char *argv[]){
  int j;
  double pi=0;
  double sum[atoi(*(argv+2))];
  p.sum = sum;

  p.ninterval=2000000000;
  printf ("Computing......\n");
  MTI_Init(&argc, &argv);
  MTI_Init_thread(ThreadEntryPoint);
  MTI_Finalize();
  for(j=0; j<=atoi(*(argv+2)); j++) {
    pi=pi+p.sum[j];
  }
  printf("pi is %lf\nFinished.\n", pi);

  return EXIT_SUCCESS;
}

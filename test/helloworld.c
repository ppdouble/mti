/*======================================**
 *Name        : helloworld.c
 *Author      : Dong Ningning
 *======================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lpmti.h"

void *MTI_startroutine () {
  int myid, numthreads;

  MTI_Comm_rank (MTI_COMM_WORLD, &myid);
  MTI_Comm_size (MTI_COMM_WORLD, &numthreads);

  fprintf (stderr, "Hello World! Thread %d of %d.\n", myid, numthreads);

  MTI_thread_exit (myid);

}

int main (int argc, char *argv[] ) {

  MTI_Init (&argc, &argv);
  MTI_Init_thread (MTI_startroutine);
  MTI_Finalize ();

  return 0;
}

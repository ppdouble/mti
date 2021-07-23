/*
 * mtiimpl.c
 *
 *  Created on: May 28, 2012
 *      Author: hpc2
 */
#include "mti.h"
#include "mtiimpl.h"
#include <sys/sysinfo.h>


/*
 * MTI_Initialized ()
 */
int MTI_Initialized (int *flag) {

  *flag = 1;
  return MTI_SUCCESS;
}/* end of MTI_Initialized () */

/*
 * MTI_Finalized ()
 */
int MTI_Finalized (int *flag) {
  *flag = 1;
  return MTI_SUCCESS;
}/* end of MTI_Finalized () */

/*
 * MTI_geterr ()
 */
#define ERR_NUM 10
char *str_err[ERR_NUM] = {
    "MTI_Err(0): Recall MTI_Initialize ().",     /* errno 0 ERR_REINIT*/
    "MTI_Err(1): Recall MTI_Finalize ().",       /* errno 1 ERR_REFINALIZE*/
    "MTI_Err(2): MTI has not initialized.",      /* errno 2 ERR_NOTINIT*/
    "MTI_Err(3): MTI has not finalized.",        /* errno 3 ERR_NOTFINALIZE*/
    "MTI_Err(4): Too many threads.", /* errno 4*/
    "MTI_Err(5): MTI expected two arguments.\nUSAGE: yourtargetfile -n numberofthreads", /* errno 5*/
    "MTI_Err(6): MTI join thread error.", /* errno 6*/
    "", /* errno 7*/
    "", /* errno 8*/
    "", /* errno 9*/
};
int MTI_geterr (int errno) {
  fprintf (stderr, "\n%s\n", *(str_err+errno));
  return MTI_SUCCESS;
}/* end of MTI_geterr () */

/*
 * MTI_check_init ()
 */
int MTI_check_init (int *flag) {
  if (*flag == 0) {
      MTI_geterr (ERR_NOTINIT);
      exit (MTI_FAILURE);
  }
  return MTI_SUCCESS;
}

/*
 * MTI_get_ncores ()
 */
int MTI_get_ncores (void) {
  return get_nprocs ();
}/* end of MTI_get_ncores () */
/*
#define MTI_get_ncores (ncores) \
  do{   \
  ncores = get_nprocs (); \
  }while(0)
*/


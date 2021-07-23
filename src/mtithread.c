/*
 * mtithread.c
 *
 *  Created on: May 30, 2012
 *      Author: DONG NINGNING
 */

#include "mtithread.h"


/*
 * MTI_thread_self ()
 */
MTI_thread_t MTI_thread_self () {
  return pthread_self ();
}

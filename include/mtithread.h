/*
 * mtithread.h
 *
 *  Created on: May 23, 2012
 *      Author: DONG NINGNING
 */

#ifndef MTITHREAD_H_
#define MTITHREAD_H_

#include <pthread.h>

typedef pthread_t MTI_thread_t;
typedef pthread_mutex_t MTI_thread_mutex_t;
typedef pthread_attr_t MTI_threadattr_t;

/*
 * MTI_create_thread ()
 */
#define MTI_create_thread(thread, attr, start_routine, arg)    \
  do{                                                           \
      pthread_create(thread, attr, start_routine, arg);         \
  }while(0)/* end of MTI_Create_thread () */

MTI_thread_t MTI_thread_self (void);
#endif /* MTITHREAD_H_ */

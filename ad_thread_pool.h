#ifndef AD_THREAD_POOL_H
#define AD_THREAD_POOL_H

#include <pthread.h>

#include "ad_queue.h"
#include "ad_thread.h"

#define AD_THREAD_POOL_MIN_THREADS 5
#define AD_THREAD_POOL_MAX_THREADS 15

/* If we ever need to turn pthread_t into a 
 * struct containing info like tid, etc.
 */
typedef pthread_t ad_thread;
typedef pthread_attr_t ad_thread_attr;

typedef pthread_mutex_t ad_thread_pool_mutex;
typedef pthread_cond_t ad_thread_pool_cond;

typedef struct {
    ad_thread *pool_maintainer;
    ad_queue *thread_queue;
    ad_queue *request_queue;
}ad_thread_pool;

#define MAINTAINER(p)   ((p)->pool_maintainer)
#define THREAD_QUEUE(p) ((p)->thread_queue)

ad_thread_pool *ad_thread_pool_construct(ad_thread_pool_mutex *mutex, 
                                         ad_thread_pool_cond *cond_var,
                                         ad_queue *request_queue);

void ad_thread_pool_create_thread(ad_thread_pool *thread_pool);

int ad_thread_pool_delete_thread(ad_thread_pool *thread_pool);

int ad_thread_pool_get_thread_count(ad_thread_pool *thread_pool);

void ad_thread_pool_destruct(ad_thread_pool *thread_pool);

void *ad_thread_pool_maintain(void *thread_pool);

#endif

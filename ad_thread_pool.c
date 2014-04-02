#include <stdio.h>
#include <stdlib.h>

#include "ad_thread_pool.h"
#include "ad_server.h"

ad_thread_pool *ad_thread_pool_construct(ad_thread_pool_mutex *mutex, 
                                         ad_thread_pool_cond *cond_var, 
                                         ad_queue *request_queue)
{
    int i;
    ad_thread_attr attr;
    ad_thread_pool *thread_pool = malloc(sizeof(ad_thread_pool));

    THREAD_QUEUE(thread_pool) = ad_queue_construct(mutex, cond_var);
    REQUEST_QUEUE(thread_pool) = request_queue;

    /* Populate the pool */
    for (i = 0; i < AD_THREAD_POOL_MIN_THREADS; i++)
    {
        ad_thread_pool_create_thread(thread_pool);
    }

    return thread_pool;
}

void ad_thread_pool_create_thread(ad_thread_pool *thread_pool)
{
    ad_thread_attr attr;
    ad_thread *new_thread = malloc(sizeof(ad_thread));
    ad_thread_parameters *parameters = malloc(sizeof(ad_thread_parameters));

    MUTEX(parameters) = MUTEX(REQUEST_QUEUE(thread_pool));
    COND(parameters) = COND(REQUEST_QUEUE(thread_pool));
    REQUEST_QUEUE(parameters) = REQUEST_QUEUE(thread_pool);
    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(new_thread, &attr, ad_thread_handle_requests, (void *) parameters);

    ad_queue_push(THREAD_QUEUE(thread_pool), (void *) new_thread);
}

int ad_thread_pool_delete_thread(ad_thread_pool *thread_pool)
{
    ad_thread *thread = (ad_thread *) ad_queue_pop(THREAD_QUEUE(thread_pool));

    if(thread)
    {
        pthread_cancel(*thread);
        free(thread);
        return 1;
    }
    return 0;
}

int ad_thread_pool_get_thread_count(ad_thread_pool *thread_pool)
{
    return ad_queue_get_node_count(THREAD_QUEUE(thread_pool));
}

void ad_thread_pool_destruct(ad_thread_pool *thread_pool)
{

    while(ad_thread_pool_delete_thread(thread_pool));

    ad_queue_destruct(THREAD_QUEUE(thread_pool));
    free(thread_pool);
}

/* Experimental!
 * Resizes the thread pool according to the server load.
 */
void *ad_thread_pool_maintain(void *thread_pool_voidptr)
{
    int i, request_count, thread_count;
    ad_thread_pool *thread_pool = (ad_thread_pool *) thread_pool_voidptr;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    pthread_cleanup_push(ad_thread_cancel_cleanup, (void *) MUTEX(THREAD_QUEUE(thread_pool)));

    pthread_mutex_lock(MUTEX(THREAD_QUEUE(thread_pool)));
    
    for ( ; ; )
    {
        thread_count = ad_queue_get_node_count(THREAD_QUEUE(thread_pool));

        pthread_mutex_lock(MUTEX(REQUEST_QUEUE(thread_pool)));
        request_count = ad_queue_get_node_count(REQUEST_QUEUE(thread_pool));
        pthread_mutex_unlock(MUTEX(REQUEST_QUEUE(thread_pool)));

        if (request_count > AD_QUEUE_HIGH_LIMIT && thread_count < AD_THREAD_POOL_MAX_THREADS)
        {
            ad_thread_pool_create_thread(thread_pool);
        }
        else if (request_count < AD_QUEUE_LOW_LIMIT && thread_count > AD_THREAD_POOL_MIN_THREADS)
        {
            ad_thread_pool_delete_thread(thread_pool);
        }
        else
        {
            if(ad_server_is_terminating())
            {
                pthread_mutex_unlock(MUTEX(THREAD_QUEUE(thread_pool)));

                pthread_exit(NULL);
            }
            else
            {
                pthread_cond_wait(COND(THREAD_QUEUE(thread_pool)), MUTEX(THREAD_QUEUE(thread_pool)));
            }
        }
    }

    pthread_cleanup_pop(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "ad_thread.h"
#include "ad_server.h"

/* Makes the necessary cleaning operations of the thread
 * if the thread is canceled.
 *
 * @param mutex the mutex that is used by the thread
 */
void ad_thread_cancel_cleanup(void *mutex)
{
    if (mutex)
    {
        pthread_mutex_unlock((ad_queue_mutex *) mutex);
    }
}

/* Is used for extending the tasks during request handling.
 *
 * @param request the pointer of the request to be handled.
 */
void ad_thread_handle_requests_hook(ad_thread_request *request)
{
    jmp_buf error_jmp;
    int jmp_status;

    int client = VOIDPTR_TO_INT(request);
    free(request);

    /* Simulating try catch */
    jmp_status = setjmp(error_jmp);

    if(jmp_status != 1) {ad_server_answer(client, error_jmp);}
}

/* Locks the mutex of the given queue pops a request
 * and handles the request. Yields the CPU when there are no
 * requests in the request queue
 *
 * @param thread_parameters parameter structure for passing info to the thread.
 */
void *ad_thread_handle_requests(void *thread_parameters)
{
    void *request = NULL;
    ad_queue_mutex *queue_mutex = MUTEX((ad_thread_parameters *) thread_parameters);
    ad_queue_cond *queue_cond   = COND((ad_thread_parameters *) thread_parameters);
    ad_queue *request_queue     = REQUEST_QUEUE((ad_thread_parameters *) thread_parameters);
    free(thread_parameters);

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    pthread_cleanup_push(ad_thread_cancel_cleanup, (void *) queue_mutex);

    pthread_mutex_lock(queue_mutex);

    for ( ; ; )
    {
        request = ad_queue_pop(request_queue);

        if (request)
        {
            /* Let other threads access the request queue while handling a request */
            pthread_mutex_unlock(queue_mutex);

            /* Hook can be changed easily to be used in different setups */
            ad_thread_handle_requests_hook(request);

            pthread_mutex_lock(queue_mutex);
        }
        else
        {
            pthread_cond_wait(queue_cond, queue_mutex);
        }
    }

    pthread_cleanup_pop(0);
}

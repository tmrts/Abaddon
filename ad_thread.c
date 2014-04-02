#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#include "ad_thread.h"
#include "ad_server.h"

void ad_thread_cancel_cleanup(void *mutex)
{
    if (mutex)
    {
        pthread_mutex_unlock((ad_queue_mutex *) mutex);
    }
}

void ad_thread_handle_requests_hook(ad_thread_request *request)
{
    int jmp_status;
    jmp_buf error_jmp;

    /* Simulating try catch */
    jmp_status = setjmp(error_jmp);

    if(jmp_status != 1) {ad_server_answer(VOIDPTR_TO_INT(request), error_jmp);}

    free(request);
}

void *ad_thread_handle_requests(void *thread_parameters)
{
    void *request;
    ad_thread_parameters *parameters = (ad_thread_parameters *) thread_parameters;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    pthread_cleanup_push(ad_thread_cancel_cleanup, (void *) MUTEX(parameters));

    pthread_mutex_lock(MUTEX(parameters));

    for ( ; ; )
    {
        request = ad_queue_pop(REQUEST_QUEUE(parameters));

        if (request)
        {
            /* Let other threads access the request queue while handling a request */
            pthread_mutex_unlock(MUTEX(parameters));

            /* Hook can be changed easily to be used in different setups */
            ad_thread_handle_requests_hook(request);

            pthread_mutex_lock(MUTEX(parameters));
        }
        else
        {
            if (ad_server_is_terminating())
            {
                pthread_mutex_unlock(MUTEX(parameters));

                free(parameters);
                pthread_exit(NULL);
            }
            else 
            {
                pthread_cond_wait(COND(parameters), MUTEX(parameters));
            }
        }
    }

    pthread_cleanup_pop(0);
}

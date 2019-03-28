//
// Big shout out and thank you go to
// the Brian dude in this SO thread:
// https://stackoverflow.com/questions/641126/posix-semaphores-on-mac-os-x-sem-timedwait-alternative
//

#ifdef __APPLE__

#include "PSemaphore.h"
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

typedef struct
{
    pthread_mutex_t count_lock;
    pthread_cond_t  count_bump;
    unsigned count;
}
bosal_sem_t;

int _sem_init(_sem_t *psem, int flags, unsigned count)
{
    bosal_sem_t *pnewsem;
    int result;

    pnewsem = (bosal_sem_t *)malloc(sizeof(bosal_sem_t));
    if (! pnewsem)
    {
        return -1;
    }
    result = pthread_mutex_init(&pnewsem->count_lock, NULL);
    if (result)
    {
        free(pnewsem);
        return result;
    }
    result = pthread_cond_init(&pnewsem->count_bump, NULL);
    if (result)
    {
        pthread_mutex_destroy(&pnewsem->count_lock);
        free(pnewsem);
        return result;
    }
    pnewsem->count = count;
    *psem = (_sem_t)pnewsem;
    return 0;
}

int _sem_destroy(_sem_t *psem)
{
    bosal_sem_t *poldsem;

    if (! psem)
    {
        return EINVAL;
    }
    poldsem = (bosal_sem_t *)*psem;

    pthread_mutex_destroy(&poldsem->count_lock);
    pthread_cond_destroy(&poldsem->count_bump);
    free(poldsem);
    return 0;
}

int _sem_post(_sem_t *psem)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    pxsem->count = pxsem->count + 1;

    xresult = pthread_cond_signal(&pxsem->count_bump);

    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }

    return 0;
}

int _sem_trywait(_sem_t *psem)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count > 0)
    {
        pxsem->count--;
    }
    else
    {
        xresult = EAGAIN;
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int _sem_wait(_sem_t *psem)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count == 0)
    {
        xresult = pthread_cond_wait(&pxsem->count_bump, &pxsem->count_lock);
    }
    if (! xresult)
    {
        if (pxsem->count > 0)
        {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

int _sem_timedwait(_sem_t *psem, const struct timespec *abstim)
{
    bosal_sem_t *pxsem;
    int result, xresult;

    if (! psem)
    {
        return EINVAL;
    }
    pxsem = (bosal_sem_t *)*psem;

    result = pthread_mutex_lock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    xresult = 0;

    if (pxsem->count == 0)
    {
        xresult = pthread_cond_timedwait(&pxsem->count_bump, &pxsem->count_lock, abstim);
    }
    if (! xresult)
    {
        if (pxsem->count > 0)
        {
            pxsem->count--;
        }
    }
    result = pthread_mutex_unlock(&pxsem->count_lock);
    if (result)
    {
        return result;
    }
    if (xresult)
    {
        errno = xresult;
        return -1;
    }
    return 0;
}

#endif

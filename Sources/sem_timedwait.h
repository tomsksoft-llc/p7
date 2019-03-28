#ifdef __APPLE__

typedef long int _sem_t;

int _sem_init(_sem_t *psem, int flags, unsigned count);
int _sem_destroy(_sem_t *psem);
int _sem_post(_sem_t *psem);
int _sem_trywait(_sem_t *psem);
int _sem_wait(_sem_t *psem);
int _sem_timedwait(_sem_t *psem, const struct timespec *abstim);

#endif

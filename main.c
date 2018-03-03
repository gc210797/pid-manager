#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "pid.h"

struct process {
	unsigned int p_id;
	pthread_t thread;
	struct process *next;
	struct process *prev;
} *procs = NULL;

static pthread_mutex_t lck;
static pthread_attr_t attr;

void *process_work(void *proc)
{
	int rnd = rand() % 30;
	sleep(rnd);
	printf("dd: I am process with pid %d exiting after sleeping for %d\n", (*(struct process **)proc)->p_id, rnd);
	
	pthread_mutex_lock(&lck);
	
	release_pid((*(struct process **)proc)->p_id);

	struct process **p = proc;
	struct process *f = *p;

	*p = (*p)->next;
	if(*p != NULL)
		(*p)->prev = f->prev;
	free(f);

	pthread_mutex_unlock(&lck);

	return NULL;
}

void process_new()
{
	struct process *new, **p;
	int id;

	pthread_mutex_lock(&lck);

	id = allocate_pid();
	if(id == 1) {
		pthread_mutex_unlock(&lck);
		return;
	}

	new = malloc(sizeof *new);
	p = &procs;
	new->p_id = id;
	new->next = *p;
	new->prev = NULL;
	if(*p != NULL) (*p)->prev = new;
	*p = new;

	printf("I am new process with pid %d\n", new->p_id);

	pthread_mutex_unlock(&lck);

	pthread_create(&new->thread, &attr, process_work, &new);
}

int main()
{
	int i;

	pthread_mutex_init(&lck, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	if(!allocate_map()) {
		printf("unable to allocate map\n");
		pthread_mutex_destroy(&lck);
		return -1;
	}

	for(i = 0; i < 5; i++)
		process_new();

	while(procs != NULL);
	
	deallocate_map();
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&lck);

	return 0;
}

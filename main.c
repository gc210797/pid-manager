#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "pid.h"

struct process {
	unsigned int p_id;
	pthread_t thread;
	int rnd;
	struct process *next;
	struct process *prev;
} *procs = NULL;

static pthread_mutex_t lck;
static pthread_attr_t attr;
static int max_sleep;
static int no_thread;

void *process_work(void *proc)
{
	struct process *p = proc;
	sleep(p->rnd);
	printf("I am process with pid %d exiting after sleeping for %d\n", p->p_id, p->rnd);
	
	pthread_mutex_lock(&lck);
	
	release_pid(p->p_id);
	
	if(p->prev == NULL && p->next == NULL) {
		free(p);
		procs = NULL;
	}
	else {
		if(p->prev != NULL)
			p->prev->next = p->next;
		if(p->next != NULL)
			p->next->prev = p->prev;
		free(p);
	}

	pthread_mutex_unlock(&lck);

	return NULL;
}

void process_new()
{
	struct process *new;
	int id;

	pthread_mutex_lock(&lck);

	id = allocate_pid();
	if(id == 1) {
		pthread_mutex_unlock(&lck);
		return;
	}

	new = malloc(sizeof *new);
	new->p_id = id;
	new->next = procs;
	new->rnd = rand() % max_sleep;
	new->prev = NULL;
	if(procs != NULL)
		procs->prev = new;
	procs = new;

	printf("I am new process with pid %d\n", new->p_id);

	pthread_mutex_unlock(&lck);

	pthread_create(&new->thread, &attr, process_work, new);
}

int main(int argc, char **argv)
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

	no_thread = 100;
	max_sleep = 30;

	if(argc == 2) {
		if(strstr(argv[1], "-t"))
			sscanf(argv[1], "-t=%d", &no_thread);
		else if(strstr(argv[1], "-s"))
			sscanf(argv[1], "-s=%d", &max_sleep);
	}
	else if(argc == 3) {
		if(strstr(argv[1], "-t"))
			sscanf(argv[1], "-t=%d", &no_thread);
		else if(strstr(argv[1], "-s"))
			sscanf(argv[1], "-s=%d", &max_sleep);
		if(strstr(argv[2], "-t"))
			sscanf(argv[2], "-t=%d", &no_thread);
		else if(strstr(argv[2], "-s"))
			sscanf(argv[2], "-s=%d", &max_sleep);
	}
	else if(argc > 3) {
		printf("Inavlid arguments\n");
		printf("Continuing with default values\n");
	}

	//printf("%s %s\n", argv[1], argv[2]);

	for(i = 0; i < no_thread; i++)
		process_new();

	while(procs != NULL);

	deallocate_map();
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&lck);

	return 0;
}

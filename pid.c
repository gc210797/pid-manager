#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include "pid.h"

#define MIN_PID 100
#define MAX_PID 1000

struct pid_cache {
	unsigned int pid_bit;
	struct pid_cache *next;
};

static struct pid_cache *cache; //header linked list
static unsigned int *map;
static unsigned int last_set_bit;

int allocate_map()
{
	int m_size = ((MAX_PID - MIN_PID) + 1) / sizeof *map;
	map = calloc(m_size, sizeof *map);
	if(!map)
		return 0;

	cache = calloc(1, sizeof *cache);
	if(!cache) {
		free(map);
		return 0;
	}

	last_set_bit = 0;
	
	return 1;
}

int allocate_pid()
{
	int bit;
	int arr_index;
	int shift;
	
	bit = last_set_bit + 1;

	if(cache->pid_bit) {
		struct pid_cache *t;

		t = cache->next;
		bit = t->pid_bit;

		cache->next = t->next;

		free(t);
		cache->pid_bit--;
	}

<<<<<<< HEAD
	if(bit == (MAX_PID - MIN_PID) + 1) {
		printf("Pid cannot be allocated wait for other process to exit\n");
		return 1;
	}
=======
	if(bit == (MAX_PID - MIN_PID) + 1)
		return 1;
>>>>>>> 436dcf09ff636cf1c3fec852a0caaecc5e975df5
	
	arr_index = bit / sizeof *map;
	shift = bit % sizeof *map;
	map[arr_index] |= (1 << shift);
	last_set_bit = bit;

	return MIN_PID + bit;
}

void release_pid(int pid)
{
	int bit;
	int arr_index;
	int shift;
	struct pid_cache *new_entry;

	bit = pid - MIN_PID;
	
	arr_index = bit / sizeof *map;
	shift = bit % sizeof *map;
	map[arr_index] &= ~(1 << shift);

	new_entry = malloc(sizeof *new_entry);
	new_entry->pid_bit = bit;
	new_entry->next = cache->next;
	cache->next = new_entry;
	cache->pid_bit++;
}

void deallocate_map()
{
<<<<<<< HEAD
	struct pid_cache *p, *t;
=======
	void *p, *t;
>>>>>>> 436dcf09ff636cf1c3fec852a0caaecc5e975df5

	for(p = cache; p != NULL; t = p, p = p->next, free(t));
	free(map);
}

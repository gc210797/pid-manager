#include <stdlib.h>
#include <assert.h>
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
	int m_size = (MAX_PID - MIN_PID) / sizeof *map;
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
	unsigned int shift;

	if(last_set_bit == (MAX_PID - MIN_PID) + 1)
		return 1;
	
	bit = last_set_bit + 1;

	if(cache->pid) {
		struct pid_cache *t;

		t = cache->next;
		bit = t->pid_bit;

		cache->next = t->next;

		free(t);
		cache->pid_bit--;
	}
	
	arr_index = bit / sizeof *map;
	shift = bit % sizeof *map;
	map[arr_index] |= ((unsigned int)1 << shift);
	last_set_bit = bit;

	return MIN_PID + bit;
}

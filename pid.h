#ifndef PID_H
#define PID_H

int allocate_map();
int allocate_pid();
void release_pid(int pid);
void deallocate_map();

#endif

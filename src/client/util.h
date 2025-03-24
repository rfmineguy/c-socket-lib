#ifndef UTIL_H
#define UTIL_H

void register_exit(void(*)(void*), void*);
void run_exit_handlers();

#endif

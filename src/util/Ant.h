#ifndef ANT_H
#define ANT_H

void init_grid();
void run_ticks(const int n);
void generate(void* dest, const unsigned long bytes);
void digest(void* src, const unsigned long bytes);

#endif

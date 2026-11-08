#ifndef CSV_H
#define CSV_H
#include "types.h"

struct Table *read_schema(const char *filename);
void free_table(struct Table *table);

#endif
#include <stdio.h>
#include "cli.h"

void print_help(void) {
    printf("Usage:\n");
    printf("  csvstat --help\n");
    printf("  csvstat schema <csv-file>\n");
    printf("  csvstat stats <csv-file>\n");
}
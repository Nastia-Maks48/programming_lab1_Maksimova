#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "csv.h"

struct Table *read_schema(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }
    char line[1024];
    fgets(line, sizeof(line), file);
    int cols = 1;
    for(int i = 0; line[i] != '\0'; i++)
    {
        if (line[i] == '\n') 
        {
            line[i] = '\0';
            break;
        }
        if(line[i] == ',')
        {
            cols++;
        }
    }
    struct Table *table = malloc(sizeof(struct Table));
    table->cols_count = cols;
    table->rows_count = 0;
    table->columns = malloc(cols * sizeof(struct Column));
    char *ptr = line;
    for(int i = 0; i < table->cols_count; i++)
    {
        char *comma = strchr(ptr, ',');
        if (comma != NULL) {
            *comma = '\0';
        }
        table->columns[i].name = malloc(strlen(ptr) + 1);
        strcpy(table->columns[i].name, ptr);
        table->columns[i].type = TYPE_NULL;
        table->columns[i].nulls_count = 0;
        table->columns[i].sum = 0.0;
        table->columns[i].count = 0;
        table->columns[i].positive_count = 0;
        table->columns[i].negative_count = 0;
        table->columns[i].even_count = 0;
        table->columns[i].min_double = 0.0;
        table->columns[i].max_double = 0.0;
        if (comma != NULL) {
        ptr = comma + 1;
        }
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '\n') {
                line[i] = '\0';
                break;
            }
        }
        if (line[0] == '\0') continue;

        table->rows_count++; 
        ptr = line;

        for (int i = 0; i < table->cols_count; i++) 
        {
            char *comma = strchr(ptr, ',');
            if (comma != NULL) *comma = '\0';
            
            char *dot = strchr(ptr, '.');
            double val_double = strtod(ptr, NULL); 
            long long val_int = (long long)val_double; 

            if (val_double > 0) table->columns[i].positive_count++;
            if (val_double < 0) table->columns[i].negative_count++;
            table->columns[i].sum += val_double;  
            table->columns[i].count++;     

            if (table->columns[i].count == 1) 
            {
                table->columns[i].min_val = val_int;
                table->columns[i].max_val = val_int;
                table->columns[i].min_double = val_double;
                table->columns[i].max_double = val_double;
            } 
            else 
            {
                if (val_int < table->columns[i].min_val) table->columns[i].min_val = val_int;
                if (val_int > table->columns[i].max_val) table->columns[i].max_val = val_int;
                
                if (val_double < table->columns[i].min_double) table->columns[i].min_double = val_double;
                if (val_double > table->columns[i].max_double) table->columns[i].max_double = val_double;
            }

            if (dot != NULL) 
            {
                table->columns[i].type = TYPE_DOUBLE; 
            } else 
            {
                if (table->columns[i].type == TYPE_NULL) 
                {
                    table->columns[i].type = TYPE_INT; 
                }
                if (val_int % 2 == 0) table->columns[i].even_count++;
            }
            if (comma != NULL) ptr = comma + 1;
        }
    }
    fclose(file);
    return table;
}

void free_table(struct Table *table) {
    if (table == NULL) return;
    for (size_t i = 0; i < table->cols_count; i++) {
        free(table->columns[i].name);
    }
    free(table->columns);
    free(table);
}

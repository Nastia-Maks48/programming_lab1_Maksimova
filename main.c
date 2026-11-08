#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "csv.h"

void print_double(double val) {
    if (val == 0.0) val = 0.0;
    printf("%.6f\n", val);
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) {
        return 2; 
    }

    if (strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }
    if(strcmp(argv[1], "schema") == 0)
    {
        if (argc < 3) {
            fprintf(stderr, "Error: Missing file name for schema\n");
            return 2;
        }
        struct Table *table = read_schema(argv[2]);
        if (table == NULL) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[2]);
            return 1;
        }
        printf("rows %zu\n", table->rows_count);
        printf("columns %zu\n", table->cols_count);

        for (size_t i = 0; i < table->cols_count; i++) {
            const char *type_str = "NULL"; 
            if (table->columns[i].type == TYPE_INT) type_str = "INT";
            else if (table->columns[i].type == TYPE_DOUBLE) type_str = "DOUBLE";
            else if (table->columns[i].type == TYPE_STRING) type_str = "STRING";
            
            printf("%zu \"%s\" %s\n", i, table->columns[i].name, type_str);
        }
        free_table(table); 
        return 0;
    }

    if (strcmp(argv[1], "stats") == 0) {
        if (argc < 3)
        {
            fprintf(stderr, "Error: Missing file name for stats\n");
            return 2;
        }
        struct Table *table = read_schema(argv[2]); 
        if (table == NULL) {
            fprintf(stderr, "Error: Could not open file %s\n", argv[2]);
            return 1;
        }
        
        printf("rows %zu\n", table->rows_count);
        printf("columns %zu\n", table->cols_count);
        printf("\n");
        
        for (size_t i = 0; i < table->cols_count; i++) 
        {
            printf("column %zu\n", i);
            printf("name \"%s\"\n", table->columns[i].name);
            
            if (table->columns[i].type == TYPE_INT) {
                printf("type INT\n");
                printf("nulls %zu\n", table->columns[i].nulls_count);
                printf("min %lld\n", table->columns[i].min_val);
                printf("max %lld\n", table->columns[i].max_val);
                printf("mean "); print_double(table->columns[i].count ? table->columns[i].sum / table->columns[i].count : 0.0);
                printf("positive %zu\n", table->columns[i].positive_count);
                printf("negative %zu\n", table->columns[i].negative_count);
                printf("even %zu\n", table->columns[i].even_count);
            } 
            else if (table->columns[i].type == TYPE_DOUBLE) {
                printf("type DOUBLE\n");
                printf("nulls %zu\n", table->columns[i].nulls_count);
                printf("min "); print_double(table->columns[i].min_double);
                printf("max "); print_double(table->columns[i].max_double);
                printf("mean "); print_double(table->columns[i].count ? table->columns[i].sum / table->columns[i].count : 0.0);
                printf("positive %zu\n", table->columns[i].positive_count);
                printf("negative %zu\n", table->columns[i].negative_count);
            }
            
            if (i < table->cols_count - 1) printf("\n");
        }
        free_table(table);
        return 0;
    }
    fprintf(stderr, "Error: Unknown command %s\n", argv[1]);
    return 2;
    
    return 0;
}
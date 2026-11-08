#ifndef LAB_TYPE_H
#define LAB_TYPE_H

#include <stddef.h> 

enum ColumnType
{
    TYPE_NULL,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_STRING
};

struct Column
{
    char *name;
    enum ColumnType type;
    
    size_t nulls_count; 
    long long min_val; 
    long long max_val;  
    double sum;        
    size_t count;     
    size_t positive_count;
    size_t negative_count;
    size_t even_count;
    double min_double;
    double max_double;
};


struct Table
{
    size_t rows_count;
    size_t cols_count;
    struct Column *columns; 
};

#endif
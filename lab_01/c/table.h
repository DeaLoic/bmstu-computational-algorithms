#ifndef __TABLE_H__
#define __TABLE_H__

#include <stdio.h>
#include "array.h"

typedef struct table_t
{
    double_arr_t x_axis;
    double_arr_t y_axis;
    int count_of_data;
} table_t;

int form_table(char* filename, double x_start, double x_end, double step, double func(double));
int read_table(char* filename, table_t* table);
int sort_table(table_t *table);
void reverse_table(table_t *table);

table_t empty_table();
void set_null_table(table_t* table);
void set_free_table(table_t* table);




#endif
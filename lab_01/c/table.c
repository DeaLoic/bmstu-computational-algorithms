#include "table.h"
#include "array.h"
#include "error_codes.h"
#include "universal.h"
#include <stdio.h>


int form_table(char* filename, double x_start, double x_end, double step, double func(double))
{
    int error_code = INCORRECT_INPUT;
    if (filename)
    {
        FILE* input_file = fopen(filename, "w");
        if (input_file)
        {
            while (x_start <= x_end)
            {
                fprintf(input_file, "%lf %lf\n", x_start, func(x_start));
                x_start += step;
            }

            fclose(input_file);
        }
    }

    return error_code;
}

int read_table(char* filename, table_t* table)
{
    int error_code = INCORRECT_INPUT;
    if (table && filename)
    {
        set_null_table(table);
        FILE* input_file = fopen(filename, "r");
        int i = 0;
        double x, y;
        if (input_file)
        {
            while (!feof(input_file) && fscanf(input_file, "%lf %lf", &x, &y) == 2)
            {
                if (i == table->count_of_data)
                {
                    error_code = change_size_array(&(table->x_axis), i * 2 + 5, sizeof(double));
                    if (!error_code)
                    {
                        error_code = change_size_array(&(table->y_axis), i * 2 + 5, sizeof(double));
                    }
                }

                if (!error_code)
                {
                    table->x_axis[i] = x;
                    table->y_axis[i] = y;
                    table->count_of_data += 1;
                    i++;
                }
            }

            fclose(input_file);

            if (error_code)
            {
                set_free_table(table);
            }
            else
            {
                sort_table(table);
            }
            
        }
    }

    return error_code;
}

int sort_table(table_t *table)
{
    return SUCCES;
}

void reverse_table(table_t *table)
{
    if (table)
    {
        double_arr_t temp = table->x_axis;
        table->x_axis = table->y_axis;
        table->y_axis = temp;
    }
}

table_t empty_table()
{
    table_t new_table;

    set_null_table(&new_table);

    return new_table;
}

void set_null_table(table_t* table)
{
    if (table)
    {
        table->count_of_data = 0;
        table->x_axis = NULL;
        table->y_axis = NULL;
    }
}

void set_free_table(table_t* table)
{
    if (table)
    {
        delete_array(&(table->x_axis));
        delete_array(&(table->y_axis));

        set_null_table(table);
    }
}
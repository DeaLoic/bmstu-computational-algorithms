#include <stdio.h>
#include <math.h>
#include "interpolate.h"
#include "array.h"
#include "table.h"
#include "universal.h"
#include "error_codes.h"

int interpolate(table_t* table, double target_x, int polynom_power, double* result)
{
    int error_code = INCORRECT_INPUT;
    if (table && polynom_power > 0 && result)
    {
        error_code = NOT_ENOUGH_DATA;

        int start_index = find_target_index(table, target_x, polynom_power);
        if (start_index >= 0)
        {
            error_code = SUCCES;
        }

        double_arr_t finite_diffs = NULL;
        if (!error_code)
        {
            error_code = form_finite_diffs(table, start_index, polynom_power, &finite_diffs);
        }
        if (!error_code)
        {
            error_code = newton_polynom(table, start_index, polynom_power, target_x, finite_diffs, result);
        }
    }

    return error_code;
}

int back_interpolate(table_t* table, double target_y, int polynom_power, double* result)
{
    int error_code = INCORRECT_INPUT;
    if (table && polynom_power > 0 && result)
    {
        error_code = NOT_ENOUGH_DATA;
        reverse_table(table);
        int start_index = find_target_index(table, target_y, polynom_power);
        if (start_index >= 0)
        {
            error_code = NOT_MONOTONIC;
            if (is_monotonic(table, start_index, polynom_power))
            {
                error_code = SUCCES;
            }
        }

        double_arr_t finite_diffs = NULL;
        if (!error_code)
        {
            error_code = form_finite_diffs(table, start_index, polynom_power, &finite_diffs);
        }
        if (!error_code)
        {
            error_code = newton_polynom(table, start_index, polynom_power, target_y, finite_diffs, result);
        }
    }
    return error_code;
}


int find_target_index(table_t* table, double target_x, int polynom_power) // -1 if error
{
    int start_position = -1;

    if (table && table->count_of_data > polynom_power && polynom_power > 0)
    {
        int is_finded = 0;
        if (fabs(target_x - table->x_axis[0]) < fabs(target_x - table->x_axis[table->count_of_data - 1]))
        {
            start_position = 0;
        }
        else
        {
            start_position = table->count_of_data + 200; // bigger than size
        }
        
        for (int i = 0; i < table->count_of_data - 1 && !is_finded; i++)
        {
            if (table->x_axis[i] <= target_x + 1e-7 && (target_x - 1e-7 < table->x_axis[i + 1]) ||
                table->x_axis[i] >= target_x - 1e-7 && (target_x + 1e-7 > table->x_axis[i + 1]))
            {
                start_position = i;
                is_finded = 1;
            }
        }

        if (start_position - (polynom_power / 2) < 0)
        {
            start_position = 0;
        }
        else if (start_position - (polynom_power / 2) + polynom_power >= table->count_of_data)
        {
            start_position = table->count_of_data - polynom_power - 1;
        }
        else
        {
            start_position -= polynom_power / 2;
        }
    }

    return start_position;
}

int form_finite_diffs(table_t* table, int start_index, int polynom_power, double_arr_t* finite_diffs_p)
{
    int error_code = INCORRECT_INPUT;
    if (table && start_index >= 0 && polynom_power > 0 && start_index + polynom_power < table->count_of_data && finite_diffs_p)
    {
        error_code = create_array(finite_diffs_p, polynom_power + 1, sizeof(double));

        double_arr_t finite_diffs = *finite_diffs_p;
        if (!error_code)
        {
            error_code = copy_array(finite_diffs, table->y_axis + start_index, polynom_power + 1, sizeof(double));
        }

        // print_array(finite_diffs, polynom_power + 1, sizeof(double), stdout, fprintf_double);
        if (!error_code)
        {
            for (int i = 0; i < polynom_power; i++)
            {
                for (int k = i; k < polynom_power; k++)
                {
                    finite_diffs[k - i] = create_finite_diff(table->x_axis[k - i + start_index], finite_diffs[k - i],
                                                             table->x_axis[k + 1 + start_index], finite_diffs[k - i + 1]);
                }
                finite_diffs[polynom_power - i] = finite_diffs[0];
            }

            reverse_array(finite_diffs, polynom_power + 1, sizeof(double));
            change_size_array(finite_diffs_p, polynom_power, sizeof(double));

            //print_array(finite_diffs, polynom_power, sizeof(double), stdout, fprintf_double);
        }
    }

    return error_code;
}

double create_finite_diff(double x1, double y1, double x2, double y2)
{
    double result = __DBL_MAX__;
    if ((x1 - x2))
    {
        result = (y1 - y2) / (x1 - x2);
    }

    return result;
}

int newton_polynom(table_t* table, int start_index, int polynom_power, double x, double_arr_t finite_diffs, double* result)
{
    int error_code = INCORRECT_INPUT;

    if (table && start_index >= 0 && polynom_power > 0 &&
        start_index + polynom_power < table->count_of_data && finite_diffs && result)
    {
        error_code = SUCCES;
        *result = table->y_axis[start_index];

        double cur_member;
        for (int i = 0; i < polynom_power; i++)
        {
            cur_member = 1;
            for (int k = 0; k < i + 1; k++)
            {
                cur_member *= (x - table->x_axis[start_index + k]);
            }
            *result += cur_member * finite_diffs[i];
        }
    }
    return error_code;
}

int is_monotonic(table_t* table, int start_position, int offset)
{
    int is_monotonic_flag = 0;
    if (table && start_position >= 0 && offset > 0 && start_position + offset < table->count_of_data)
    {
        int is_incremented = 0;
        if (table->y_axis[start_position] < table->y_axis[start_position + 1] - 1e-7)
        {
            is_incremented = 1;
        }

        int current_increment = 0;
        is_monotonic_flag = 1;

        for (int i = 0; i < offset - 1 && is_monotonic_flag; i++)
        {
            if (table->y_axis[start_position] < table->y_axis[start_position + 1] - 1e-7)
            {
                current_increment = 1;
            }
            else if (table->y_axis[start_position] > table->y_axis[start_position + 1] + 1e-7)
            {
                current_increment = 0;
            }
            else
            {
                is_monotonic_flag = 0;
            }

            if (current_increment != is_incremented)
            {
                is_monotonic_flag = 0;
            }

            start_position++;
        }
    }
    return is_monotonic_flag;
}

int root_by_half_division(table_t* table, double eps, double func(double), double* result)
{
    int error_code = INCORRECT_INPUT;

    int section = find_section(table, 0);
    if (section >= 0)
    {
        error_code = SUCCES;

        double a = table->x_axis[section];
        double b = table->x_axis[section + 1];
        error_code = half_division(a, b, eps, func, result);
    }

    return error_code;
}

int find_section(table_t* table, double target_y) // -1 if root doesnt exist
{
    int start_position = -1;

    if (table && table->count_of_data > 1)
    {
        for (int i = 0; i < table->count_of_data - 1; i++)
        {
            if (table->y_axis[i] <= target_y + 1e-7 && target_y - 1e-7 <= table->y_axis[i + 1] ||
                table->y_axis[i] >= target_y - 1e-7 && target_y + 1e-7 >= table->y_axis[i + 1])
            {
                start_position = i;
            }
        }
    }

    return start_position;
}

int half_division(double a, double b, double eps, double func(double), double* result)
{
    int error_code = INCORRECT_INPUT;

    if (result && func)
    {
        error_code = SUCCES;
        double c;
        int iter = 0;
        c = (a + b) / 2;
        // printf("\nA: %lf, B: %lf\n", a, b);
        while (fabs((b - a) / c) > eps && iter < MAX_ITERATION)
        {
            iter++;
            c = (a + b) / 2;
            if (func(a) * func(c) > 0)
            {
                a = c;
            }
            else
            {
                b = c;
            }
        }
        *result = c;
    }

    return error_code;
}
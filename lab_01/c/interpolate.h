#ifndef __INTERPOLATE_H__
#define __INTERPOLATE_H__

#include "table.h"
#include "array.h"

#define MAX_ITERATION 1000000

int interpolate(table_t* table, double target_x, int polynom_power, double* result);
int back_interpolate(table_t* table, double target_y, int polynom_power, double* result);
int root_by_half_division(table_t* table, double eps, double func(double), double* result);

int half_division(double a, double b, double eps, double func(double), double* result);
int find_section(table_t* table, double target_y);

double create_finite_diff(double x1, double y1, double x2, double y2);

int find_target_index(table_t* table, double target_x, int polynom_power); // -1 if error
// finite_diffs
int form_finite_diffs(table_t* table, int start_index, int polynom_power, double_arr_t* finite_diffs);
int newton_polynom(table_t* table, int start_index, int polynom_power, double x, double_arr_t finite_diffs, double* result);

int is_monotonic(table_t* table, int start_position, int offset);
int root_place(table_t* table); // -1 if root doesnt exist

#endif
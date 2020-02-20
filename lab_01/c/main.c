#include <stdio.h>
#include "interpolate.h"
#include "array.h"
#include "table.h"
#include "error_codes.h"
#include "math.h"

#define INPUT_FILE "table.txt"
#define PI 3.141592653
#define EPS 1e-7

#define FORM_TABLE 1
#define X_START 0
#define X_END 10
#define STEP 1

void parse_error_code(int error);

double source_func(double x)
{
    return x * x;
}

int main()
{
    int error_code = SUCCES;
    table_t table;

    if (FORM_TABLE)
    {
        form_table(INPUT_FILE, X_START, X_END, STEP, source_func);
    }
    error_code = read_table(INPUT_FILE, &table);
    if (error_code)
    {
        printf("Error while parse table\n");
    }

    int n = 0;
    double target_x = 0;
    double computing_y = 0;
    double root = 0;
    double root_half = 0;

    printf("Input x and polynom poower: ");
    if (!error_code && scanf("%lf %d", &target_x, &n) == 2)
    {
        error_code = interpolate(&table, target_x, n, &computing_y);
        if (!error_code)
        {
            printf("\nResult of interpolation: %lf\nExpected: %lf\n", computing_y, source_func(target_x));
        }
        else
        {
            printf("\nSome error in interpolate!\n");
            parse_error_code(error_code);
        }

        error_code = root_by_half_division(&table, EPS, source_func, &root_half);
        if (!error_code)
        {
            printf("\nRoot by half division: %lf\nFunc from this x: %lf\n", root_half, source_func(root_half));
        }
        else
        {
            printf("\nSome error in half division root finding\nRoot cant be find\n");
        }
        
        error_code = back_interpolate(&table, 0, n, &root);
        if (!error_code)
        {
            printf("\nRoot by reverse interpolation: %lf\nFunc from this x: %lf\n", root, source_func(root));
        }
        else
        {
            printf("\nSome error in reverse interpolate root finding\n");
            parse_error_code(error_code);
        }

    }
    else
    {
        printf("\nError in input data\n");
    }

    return error_code;
}

void parse_error_code(int error_code)
{
    if (error_code == NOT_ENOUGH_DATA)
    {
        printf("Polynom power to big\n");
    }
    else if (error_code == NOT_MONOTONIC)
    {
        printf("Function not monotonic- increase or decrease\n");
    }
    else if (error_code == LOGIC_ERROR)
    {
        printf("Other logic error\n");
    }
    else
    {
        printf("Unknown error\n");
    }
}
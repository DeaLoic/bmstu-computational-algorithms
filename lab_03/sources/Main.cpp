#include <iostream>
#include "PrettyTable.h"
#include "Spline.h"

#define TABLE_FILE "table.txt"

double FuncPrototype(double x)
{
    return x * x;
}

int main()
{
    PrettyTable table;
    table.Form(TABLE_FILE, FuncPrototype, vector<double>({ -5, 5, 0.1 }));
    table.ReadPrettyTable(TABLE_FILE);

    Spline spline = Spline(table);

    double targetX;
    cout << "Input x: ";
    cin >> targetX;

    double y;
    y = spline.Solve(targetX);

    cout << "\nResult: " << y << "\nExpected: " << FuncPrototype(targetX);
    
    return 0;
}
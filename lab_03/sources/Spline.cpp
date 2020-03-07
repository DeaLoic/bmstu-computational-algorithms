#include <vector>
#include <tuple>
#include <algorithm>
#include <cmath>
#include "Spline.h"
#include "PrettyTable.h"

Spline::Spline(PrettyTable table)
{
    this->table = table;
    FormCoeffsC();
}

void Spline::FormCoeffsC()
{
    vector<tuple<double, double> > ForwardCoeffs;

    double zeta = 0;
    double nu = 0;
    ForwardCoeffs.push_back(tuple<double, double>(zeta, nu));
    ForwardCoeffs.push_back(tuple<double, double>(zeta, nu));

    double A, B, D, F;
    for (int i = 1; i < (table.GetSize() - 1); i++)
    {
        A = GetStep(i);
        D = GetStep(i + 1);
        B = - 2 * (A + D);
        F = -3 * ((table.GetY(i + 1) - table.GetY(i)) / D - (table.GetY(i) - table.GetY(i - 1)) / A);

        zeta = D / (B - A * get<0>(ForwardCoeffs[i]));
        nu = (F + A * table.GetY(i + 1)) / (B - A * get<0>(ForwardCoeffs[i]));

        ForwardCoeffs.push_back(tuple<double, double>(zeta, nu));
    }

    this->coeffsC = {0};
    for (int i = 1; i < table.GetSize(); i++)
    {
        this->coeffsC.push_back(coeffsC[i - 1] * get<0>(ForwardCoeffs[ForwardCoeffs.size() - i]) + 
                                                 get<1>(ForwardCoeffs[ForwardCoeffs.size() - i]));
    }

    reverse(this->coeffsC.begin(), this->coeffsC.end());
}

double Spline::GetStep(int i)
{
    return table.GetX(i) - table.GetX(i - 1);
}

double Spline::GetA(int i)
{
    return table.GetY(i - 1);
}

double Spline::GetB(int i)
{
    return (table.GetY(i) - table.GetY(i - 1)) / GetStep(i) - 1.0 / 3.0 * GetStep(i) * (GetC(i + 1) + 2 * GetC(i));
}

double Spline::GetC(int i)
{
    return coeffsC[i - 1];
}

double Spline::GetD(int i)
{
    return (GetC(i + 1) - GetC(i)) / (3 * GetStep(i));
}

double Spline::Solve(double x)
{
    int i = FindInterval(x);

    double step = x - table.GetX(i - 1);
    return GetA(i) + GetB(i) * step + GetC(i) * step * step + GetD(i) * step * step * step;
}

int Spline::FindInterval(double x)
{
    int interval = 0;

    for (int i = 0; i < table.GetSize() - 1 && !interval; i++)
    {
        if (table.GetX(i) < x && x <= table.GetX(i + 1) + 1e-9)
        {
            interval = i;
        }
    }

    if (interval == 0 && abs(x - table.GetX(table.GetSize() - 1)) < abs(x - table.GetX(0)))
    {
        interval = table.GetSize() - 1;
    }

    return interval;
}
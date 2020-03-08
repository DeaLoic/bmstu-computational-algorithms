#ifndef __SPLINE_H__
#define __SPLINE_H__

#include "PrettyTable.h"
#include <vector>
#include <tuple>

using namespace std;

class Spline
{
private:
    PrettyTable table;
    vector<double> coeffsC;

    double GetA(int i);
    double GetB(int i);
    double GetC(int i);
    double GetD(int i);

    void FormCoeffsC();
    double GetStep(int i);
    int FindInterval(double x);
public:
    Spline(PrettyTable table);
    double Solve(double x);
    void PrintCoeffsC();
};

#endif
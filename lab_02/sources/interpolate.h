#ifndef __INTERPOLATE_H__
#define __INTERPOLATE_H__

#include <vector>
#include "table.h"

class Interpolator
{
private: 
    Table &table;

    std::vector<double> startCoordinates;
    std::vector<std::vector<double> > workConfiguration;
    std::vector<int> polynomPowers;

    void FormFiniteDiffs(std::vector<double> &x_dimension, std::vector<double> &y_dimension, std::vector<double> &finiteDiffs);
    double CreateFiniteDiff(double x1, double y1, double x2, double y2);

    void FormConfiguration();
    void FindIndexes(std::vector<vector<int> > &indexes);

    double NewtonInterpolate(std::vector<std::vector<double> > workConfiguration, int targetConfiguration = 0);
    double NewtonPolynom(std::vector<double> &x_dimension, std::vector<double> &y_dimension, std::vector<double> &finiteDiffs, double targetX);
public:
    Interpolator(Table &table);
    double Interpolate(std::vector<double> startCoordinates, std::vector<int> polynomPowers);
};

#endif
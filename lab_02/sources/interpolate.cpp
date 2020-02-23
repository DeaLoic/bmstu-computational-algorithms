#include "table.h"
#include "interpolate.h"
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace std;

Interpolator::Interpolator(Table &tabl) : table(tabl)
{
}

void Interpolator::FormFiniteDiffs(vector<double> &x_dimension, vector<double> &y_dimension, vector<double> &finiteDiffs)
{
    // y_dimension using like result finiteDiffs array

    int polynomPower = x_dimension.size() - 1;

    finiteDiffs.erase(finiteDiffs.begin(), finiteDiffs.end());
    for (int i = 0; i < y_dimension.size(); i++)
    {
        finiteDiffs.push_back(y_dimension[i]);
    }

    for (int i = 0; i < polynomPower; i++)
    {
        for (int k = i; k < polynomPower; k++)
        {
            finiteDiffs[k - i] = CreateFiniteDiff(x_dimension[k - i], finiteDiffs[k - i], 
                                                  x_dimension[k + 1], finiteDiffs[k - i + 1]);
        }
        finiteDiffs[polynomPower - i] = finiteDiffs[0];
    }

    reverse(finiteDiffs.begin(), finiteDiffs.end());
    finiteDiffs.resize(polynomPower);
}

double Interpolator::CreateFiniteDiff(double x1, double y1, double x2, double y2)
{
    if (x1 - x2)
    {
        return (y1 - y2) / (x1 - x2);
    }
    else
    {
        throw new overflow_error("Zero division error");
    }
    
}

void Interpolator::FormConfiguration()
{
    workConfiguration.erase(workConfiguration.begin(), workConfiguration.end());
    
    vector<vector<int> > indexes;
    FindIndexes(indexes);


    for (int i = 0; i < indexes.size(); i++)
    {
        workConfiguration.push_back(vector<double>());
        vector<double> curDimension = table.GetDimension(i);
        for (int k = 0; k < indexes[i].size(); k++)
        {
            workConfiguration[i].push_back(curDimension[indexes[i][k]]);
        }
    }

    workConfiguration.push_back(vector<double>());
    vector<int> coordinates(indexes.size());
    int allCoords = 1;

    for (int i = 0; i < indexes.size(); i++)
    {
        allCoords *= indexes[i].size();
    }

    int curDimensionSize = allCoords;
	int subCycleIndex = 0;
    for (int i = 0; i < allCoords; i++)
    {
        curDimensionSize = allCoords;
		subCycleIndex = i;
        for (int k = 0; k < indexes.size(); k++)
        {
            curDimensionSize /= indexes[k].size();
            coordinates[k] = indexes[k][subCycleIndex / curDimensionSize];
			subCycleIndex %= curDimensionSize;
        }
        workConfiguration[indexes.size()].push_back(table.GetElementSourceCoords(coordinates));
    }
}

void Interpolator::FindIndexes(vector<vector<int> > &indexes)
{
    indexes.erase(indexes.begin(), indexes.end());
    int startPosition = 0;
    for (int i = 0; i < table.GetCoordsCount(); i++)
    {
        vector<double> curDimension = table.GetDimension(i);

        if (curDimension.size() < polynomPowers[i])
        {
            throw new invalid_argument("Too big polynom");
        }

        if (fabs(startCoordinates[i] - curDimension[0]) < fabs(startCoordinates[i] - curDimension[curDimension.size() - 1]))
        {
            startPosition = 0;
        }
        else
        {
            startPosition = curDimension.size() + 100;
        }
        
        for (int k = 0; (k < curDimension.size() - 1); k++)
        {
            if (((curDimension[k] <= startCoordinates[i] + 1e-7) && (startCoordinates[i] - 1e-7 < curDimension[k + 1])) ||
                curDimension[k] >= startCoordinates[i] - 1e-7 && (startCoordinates[i] - 1e-7 > curDimension[k + 1]))
                {
                    startPosition = k;
                }
        }

        if (startPosition - (polynomPowers[i] / 2) < 0)
        {
            startPosition = 0;
        }
        else if (startPosition - (polynomPowers[i] / 2) + polynomPowers[i] >= curDimension.size())
        {
            startPosition = curDimension.size() - polynomPowers[i] - 1;
        }
        else
        {
            startPosition -= polynomPowers[i] / 2;
        }
        
        indexes.push_back(vector<int>());
        for (int k = 0; k <= polynomPowers[i]; k++)
        {
            indexes[i].push_back(startPosition + k);
        }
    }
}

double Interpolator::NewtonInterpolate(vector<vector<double> > startConfiguration, int targetConfiguration)
{
    vector<double> lastStepConfiguration;
    vector<vector<double> > newConfiguration;

    int startConfigurationSize = startConfiguration.size();
    if (startConfigurationSize == 2)
    {
        lastStepConfiguration = startConfiguration[1];
    }
    else
    {
        int currentDimensionSize = 1;
        for (int i = 1; i < startConfigurationSize - 1; i++)
        {
            newConfiguration.push_back(startConfiguration[i]);
            currentDimensionSize *= startConfiguration[i].size();
        }
        newConfiguration.push_back(vector<double>());

        vector<double>::iterator valueIterator = startConfiguration[startConfigurationSize - 1].begin();
        for (int i = 0; i < startConfiguration[0].size(); i++)
        {
            newConfiguration[startConfigurationSize - 2].assign(valueIterator, valueIterator + currentDimensionSize);
            valueIterator += currentDimensionSize;
            lastStepConfiguration.push_back(NewtonInterpolate(newConfiguration, targetConfiguration + 1));
        }
    }

    vector<double> finiteDiffs;
    vector<double> &refFiniteDiffs = finiteDiffs;
    FormFiniteDiffs((startConfiguration[0]), lastStepConfiguration, refFiniteDiffs);

    double result = NewtonPolynom((startConfiguration[0]), lastStepConfiguration, refFiniteDiffs, startCoordinates[targetConfiguration]);
    return result;
}

double Interpolator::NewtonPolynom(std::vector<double> &x_dimension, std::vector<double> &y_dimension, std::vector<double> &finiteDiffs, double targetX)
{
    double result = y_dimension[0];
    
    for (int i = 0; i < x_dimension.size() - 1; i++)
    {
        double curMember = 1;
        for (int k = 0; k < i + 1; k++)
        {
            curMember *= (targetX - x_dimension[k]);
        }

        result += curMember * finiteDiffs[i];
    }

    return result;
}

double Interpolator::Interpolate(vector<double> startCoordinates, vector<int> polynomPowers)
{
    this->startCoordinates = startCoordinates;
    this->polynomPowers = polynomPowers;

    FormConfiguration();

    for (int i = 0; i < workConfiguration.size(); i++)
    {
        cout << "Dimension work #" << i << " : ";
        for (int k = 0; k < workConfiguration[i].size(); k++)
        {
            cout << workConfiguration[i][k] << " ";
        }
        cout << "\n";
    }
    return NewtonInterpolate(workConfiguration);
}
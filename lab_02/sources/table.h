#ifndef __TABLE_H__
#define __TABLE_H__

#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Table
{
private:
    vector<vector<double> > tableVector;
public:
    Table();
    Table(vector<vector<double> > tableVector);

    void Parse(std::string filename, int dimension = 2);
    void Form(std::string filename, double (*func)(vector<double> args), vector<vector<double> > coordsOffset);

    void Print();
    double GetElementSourceCoords(vector<int> coords);

    int GetCoordsCount();
    int GetDimensionSize(int dimension);
    
    vector<double> GetDimension(int dimension);

    bool IsCorrect();
};

#endif
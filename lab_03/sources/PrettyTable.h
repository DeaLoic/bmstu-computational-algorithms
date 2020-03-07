#ifndef __PRETTY_TABLE_H__
#define __PRETTY_TABLE_H__

#include <iostream>
#include <vector>
#include <tuple>
#include <string>

using namespace std;

// two dimension in pretty format
class PrettyTable
{
private:
    vector<tuple<double, double> > tableBody;
public:
    void ReadPrettyTable(string filename);
    void Form(string filename, double (*func)(double), vector<double> coordsOffset);

    int GetSize();
    tuple<double, double> GetPoint(int i);
    double GetX(int i);
    double GetY(int i);
};

#endif
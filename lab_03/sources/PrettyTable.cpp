#include "PrettyTable.h"
#include <tuple>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void PrettyTable::ReadPrettyTable(string filename)
{
    ifstream file;
    file.open(filename);

    if (file.is_open())
    {
        tuple<double, double> currentDot;
        double currentX, currentY;
        while (file >> currentX && file >> currentY)
        {
            tableBody.push_back(make_tuple(currentX, currentY));
        }

        if (!file.eof())
        {
            tableBody.clear();
            throw new runtime_error("File should contain even count of double");
        }

        file.close();
    }
    else
    {
        throw new runtime_error("File cant be open");
    }
}

void PrettyTable::Form(string filename, double (*func)(double), vector<double> arguments)
{
    ofstream out;
    out.open(filename);
    if (out.is_open())
    {
        double currentX = 0;
        double currentY;
        for (currentX = arguments[0]; currentX <= arguments[1]; currentX += arguments[2])
        {
            try
            {
                currentY = func(currentX);
            }
            catch (exception &e)
            {
                throw new invalid_argument("Cant count func in this x: %lf" + to_string(currentX));
            }

            out << currentX << "   " << currentY << "\n";
        }

        out.close();
    }
    else
    {
        throw new runtime_error("File cant be open");
    }
}

int PrettyTable::GetSize()
{
    return tableBody.size();
}

tuple<double, double> PrettyTable::GetPoint(int i)
{
    if (i >= 0 && i < GetSize())
    {
        return tableBody[i];
    }
    else
    {
        throw new out_of_range("Incorrect index " + to_string(i) + ". Size: " + to_string(GetSize()));
    }
}

double PrettyTable::GetX(int i)
{
    return get<0>(GetPoint(i));
}

double PrettyTable::GetY(int i)
{
    return get<1>(GetPoint(i));
}
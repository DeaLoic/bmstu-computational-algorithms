#include "table.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

Table::Table()
{
}

Table::Table(vector<vector<double> > tableVector)
{
    this->tableVector = tableVector;
}

void Table::Parse(string fileName, int dimension)
{
    if (dimension < 2)
    {
        throw exception();
    }

    ifstream in;
    in.open(fileName);
    double tempVar = 0;

    tableVector.erase(tableVector.begin(), tableVector.end());

    stringstream currentStringStream;
    string currentString = "";
    for (int i = 0; i < dimension - 1; i++)
    {
        tableVector.push_back(vector<double>());
        getline(in, currentString);
		currentStringStream.clear();
		currentStringStream << currentString;
        while (currentStringStream >> tempVar)
        {
            tableVector[i].push_back(tempVar);
        }
    }

    tableVector.push_back(vector<double>());
    while (in >> tempVar)
    {
        tableVector[dimension - 1].push_back(tempVar);
    }

    in.close();
}

void Table::Form(std::string filename, double (*func)(vector<double> args), vector<vector<double> > coordsOffset)
{
    ofstream out;
    double currentCoord, end, beg, step;
    out.open(filename, ofstream::out);
    for (int i = 0; i < coordsOffset.size(); i++)
    {
        beg = coordsOffset[i][0];
        end = coordsOffset[i][1];
        step = coordsOffset[i][2];
        coordsOffset[i].clear();
        while (beg <= end)
        {
            coordsOffset[i].push_back(beg);
            out << beg << " ";
            beg += step;
        }
        out << "\n";
    }

    int allCoords = 1;
    for (int i = 0; i < coordsOffset.size(); i++)
    {
        allCoords *= coordsOffset[i].size();
    }

    vector<double> coordinates(coordsOffset.size());
    int curDimensionSize = allCoords;
	int subCycleIndex = 0;
    for (int i = 0; i < allCoords; i++)
    {
        curDimensionSize = allCoords;
		subCycleIndex = i;
        for (int k = 0; k < coordsOffset.size(); k++)
        {
            curDimensionSize /= coordsOffset[k].size();
            coordinates[k] = coordsOffset[k][subCycleIndex / curDimensionSize];
			subCycleIndex %= curDimensionSize;
        }

        out << func(coordinates) << " ";
        if (i % (allCoords /  coordsOffset[0].size()) == 0)
        {
            out << "\n";
        }
    }

}

void Table::Print()
{
    for (int i = 0; i < tableVector.size(); i++)
    {
        cout << "Dimension #" << i << ":" << ' ';
        for (int k = 0; k < tableVector[i].size(); k++)
        {
            cout << tableVector[i][k] << ' ';
        }
        cout << '\n';
    }
}

bool Table::IsCorrect()
{
    bool isCorrect = false;
    unsigned int expectedLastDimSize = 1;
    if (tableVector.size() >= 2)
    {
        for (int i = 0; i < tableVector.size() - 1; i++)
        {
            expectedLastDimSize *= tableVector[i].size();
        }

        if (tableVector[tableVector.size() - 1].size() == expectedLastDimSize)
        {
            isCorrect = true;
        }
    }

    return isCorrect;
}

double Table::GetElementSourceCoords(vector<int> coords)
{
    if (coords.size() != GetCoordsCount() || !IsCorrect())
    {
        throw new invalid_argument("Bad coords");
    }
    else
    {
        int offset = 0;
        for (int i = 0; i < GetCoordsCount(); i++)
        {
            offset += GetDimensionSize(i) * coords[i];
        }

        return tableVector[GetCoordsCount()][offset];
    }
    
}

// Index from 0
int Table::GetDimensionSize(int dimension)
{
    int dimensionSize = 0;
    if (dimension >= 0 && dimension < GetCoordsCount())
    {
        dimensionSize = 1;
        for (int i = GetCoordsCount() - 1; i > dimension; i--)
        {
            dimensionSize *= tableVector[i].size();
        }
    }

    return dimensionSize;
}

int Table::GetCoordsCount()
{
    return tableVector.size() - 1;
}

vector<double> Table::GetDimension(int dimension)
{
    return tableVector[dimension];
}
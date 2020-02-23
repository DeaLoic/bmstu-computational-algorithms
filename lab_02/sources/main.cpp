#include <iostream>
#include <string>
#include <vector>
#include "interpolate.h"
#include "table.h"

#define INPUT_FILE "table.txt"

using namespace std;

double func(vector<double> arguments)
{
	return arguments[0] * arguments[0] + arguments[1] * arguments[1];
}

int main()
{

	Table table = Table();

	vector<vector<double> > arguments = {{-5, 5, 0.3}, {-5, 5, 0.3}};
	table.Form(INPUT_FILE, func, arguments);

	int dimensionCount = 0;
	cout << "Input dimensions count: ";
	cin >> dimensionCount;
	table.Parse(INPUT_FILE, dimensionCount);

	//table.Print();

	cout << "Please input " << table.GetCoordsCount() << " coordinates: ";
	vector<double> startCoordinates;
	double tempCoord = 0;
	for (int i = 0; i < dimensionCount - 1; i++)
	{
		(cin >> tempCoord);
		startCoordinates.push_back(tempCoord);
	}

	cout << "Please input " << table.GetCoordsCount() << " polynom powers: ";
	vector<int> polynomPowers;
	int tempInt = 0;
	for (int i = 0; i < dimensionCount - 1; i++)
	{
		(cin >> tempInt);
		polynomPowers.push_back(tempInt);
	}

	Interpolator interpolator = Interpolator(table);

	try
	{
		double result = interpolator.Interpolate(startCoordinates, polynomPowers);
		cout << "\nResult of interpolation: " << result << "\n";
	}
	catch (const exception &ex)
	{
		cout << ex.what();
	}

	system("pause");

    return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include "interpolate.h"
#include "table.h"

using namespace std;

int main()
{

	Table table = Table();

	int tempInt = 0;
	cout << "Input dimensions count: ";
	cin >> tempInt;
	table.Parse(string("table.txt"), tempInt);

	table.Print();


	cout << "Please input " << table.GetCoordsCount() << " coordinates: ";
	vector<double> startCoordinates;
	double tempCoord = 0;
	for (int i = 0; i < tempInt - 1; i++)
	{
		(cin >> tempCoord);
		startCoordinates.push_back(tempCoord);
	}

	cout << "Please input " << table.GetCoordsCount() << " polynom powers: ";
	vector<int> polynomPowers;
	for (int i = 0; i < tempInt - 1; i++)
	{
		(cin >> tempInt);
		polynomPowers.push_back(tempInt);
	}

	cout << startCoordinates.size() << " " << polynomPowers.size() << "\n";

	Interpolator interpolator = Interpolator(table);

	try
	{
		cout << "Result of interpolation: " << interpolator.Interpolate(startCoordinates, polynomPowers);
	}
	catch (const exception &ex)
	{
		cout << ex.what();
	}

	cout << "\nThe end\n";

    
    return 0;
}
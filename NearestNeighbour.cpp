#include "NearestNeighbour.h"
#include <iostream>
#include <vector>

using namespace std;

//------------------------------------------------------------------------------------------------------------------------------------
NearestNeighbour::NearestNeighbour(int n, int** m, vector<int>& path) : N(n), matrix(m), currentPathNN(path)
{
	visited.resize(N, false);
}
//------------------------------------------------------------------------------------------------------------------------------------
NearestNeighbour::~NearestNeighbour()
{
	matrix = nullptr;
}
//------------------------------------------------------------------------------------------------------------------------------------
void NearestNeighbour::findNearestNeighbourPath()
{
	// wierzcholek startowy
	int start = 0;
	currentPathNN.emplace_back(start);

	// zaznaczenie ze odwiedzono wierzcholek startowy
	if (start >= 0 && start < N)
		visited[start] = true;

	for (int i = 0; i < N - 1; i++)
	{
		// znalezienie i dodanie do tablicy najkrotszego odcinka od danego wierzcholka
		int nearestNeighbour = findNearestNeighbour(start);
		currentPathNN.emplace_back(nearestNeighbour);

		// zaznaczenie ze odwiedzono wierzcholek
		if (nearestNeighbour >= 0 && nearestNeighbour < N)
			visited[nearestNeighbour] = true;
		start = nearestNeighbour; // nastepny wierzcholek
	}
}
//------------------------------------------------------------------------------------------------------------------------------------
int NearestNeighbour::findNearestNeighbour(int v)
{
	int nearestNeighbour = -1;
	int minDistance = INT_MAX;

	for (int i = 0; i < N; i++)
	{
		// jesli sprawdzany wierzcholek nie byl odwiedzony i odleglosc jest mniejsza
		if (!visited[i] && matrix[v][i] < minDistance)
		{
			minDistance = matrix[v][i];
			nearestNeighbour = i;
		}
	}
	return nearestNeighbour;
}
//------------------------------------------------------------------------------------------------------------------------------------
void NearestNeighbour::printCurrentPathNN()
{
	for (int i = 0; i < currentPathNN.size(); i++)
	{
		cout << currentPathNN[i] << " ";
	}
	cout << "\n";
}
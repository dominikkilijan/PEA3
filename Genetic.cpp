#include "Genetic.h"
#include "AdjacencyMatrix.h"
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <time.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <math.h>
#include <chrono>

using namespace std;
using namespace std::chrono;


//------------------------------------------------------------------------------------------------------------------------------------
//Genetic(int, int**, int, double, double, double, int, int); // wierzcholki, macierz, stopTime, startingPopulation, 
// mutation, crossover, crossoverChoice, mutationChoice


Genetic::Genetic(int n, int** m, int sTime, double startPop, double mut, double cross, int cChoice, int mChoice)
{
	N = n;
	matrix = m;
	stopTime = sTime;
	startingPopulation = startPop;
	mutation = mut;
	crossover = cross;
	crossoverChoice = cChoice;
	mutationChoice = mChoice;
	
	// zarezerwowanie odpowiedniej ilosci miejsca
	currentPath.reserve(N);
	bestPath.reserve(N);

	// tylko na czas random
	for (int i = 0; i < N; i++)
	{
		currentPath.emplace_back(i);
	}

	// zmienne zwiazane z pomiarem czasu
	long long int frequency = 0;
	long long int start = 0;
	long long int elapsed = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	cout << "Konstruktor dziala\n";
}
//------------------------------------------------------------------------------------------------------------------------------------
Genetic::~Genetic()
{
	matrix = nullptr;
}
//------------------------------------------------------------------------------------------------------------------------------------
// funkcja do liczenia czasu
long long int Genetic::read_QPC()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return((long long int)count.QuadPart);
}
//------------------------------------------------------------------------------------------------------------------------------------
//wyliczanie wagi aktualnej sciezki
double Genetic::countSum(vector<int>& countPath)
{
	double countSum = 0;

	for (int i = 0; i < N; i++)
	{
		countSum += matrix[countPath[i]][countPath[(i + 1) % N]]; // dzieki modulo na koncu zapetli sie do wierzcholka poczatkowego
	}

	return countSum;
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::randomPath()
{
	auto rng = default_random_engine{};
	shuffle(begin(currentPath), end(currentPath), rng);
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::printCurrentPath()
{
	for (int i = 0; i < currentPath.size(); i++)
	{
		cout << currentPath[i] << " ";
	}
	cout << "\n";
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::PrintBestPath()
{
	for (int i = 0; i < bestPath.size(); i++)
	{
		cout << bestPath[i] << " ";
	}
	cout << "\n";
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::TSPGenetic()
{
	// rozpoczecie pomiaru czasu
	start = read_QPC();
	
	// algorytm
	cout << "Rozpoczecie algorytmu\n";
	geneticAlgorithm();
	cout << "Koniec algorytmu\n";
	// koniec algorytmu

	// wyswietlenie wyniku i zapisanie do pliku
	fstream file;
	file.open("temp.txt", ios::out);
	file << N << endl;

	cout << "\n\nWaga = " << bestSum << endl;
	cout << "Czas znaleznienia najlepszego rozwiazania w ms: " << setprecision(10) << bestElapsed << endl;
	cout << "Sciezka: ";

	// wyswietlenie koncowej sciezki
	for (int i = 0; i <= bestPath.size(); i++)
	{
		if (i != bestPath.size())
		{
			cout << bestPath[i] << "->";
			file << bestPath[i] << "\n";
		}
		else
		{
			cout << bestPath[0] << "\n";
			file << bestPath[0] << "\n";
		}
	}
	cout << "\n";
	file.close();
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::geneticAlgorithm() // poki co to jest pelna losowosc. Tylko zeby bylo cokolwiek
{
	// poczatkowa sciezka
	cout << "Poczatkowa sciezka\n";
	randomPath();

	// poczatkowe wyniki
	cout << "Poczatkowe wyniki\n";
	currentSum = countSum(currentPath);
	cout << "bestSum = currentSum\n";
	bestSum = currentSum;
	
	// przygotowanie minutnika algorytmu
	cout << "Rzeczy do minutnika\n";
	const time_point<system_clock> startTime = system_clock::now();
	seconds stopTimeSeconds = seconds(stopTime);
	seconds finalTime;
	
	cout << "Algorytm genetyczny\n";
	// petla jesli nie skonczyl sie czas
	while ((system_clock::now() - startTime) < stopTimeSeconds)
	{
		randomPath();
		currentSum = countSum(currentPath);

		if (currentSum < bestSum)
		{
			// jesli rozwiazanie jest lepsze niz poprzednie
			bestSum = currentSum;
			bestPath = currentPath;


			// zapisanie i wyswietlenie czasu znalezienia rozwiazania
			QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
			bestElapsed = ((1000.0 * (read_QPC() - start)) / frequency);

			cout << "bestSum = " << bestSum << "\n";
			cout << "Znalezienie bestSum w ms: " << setprecision(10) << bestElapsed << endl;

		}
	}
}
//------------------------------------------------------------------------------------------------------------------------------------

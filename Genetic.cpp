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
Genetic::Genetic(int n, int sTime, double alpha, int** m)
{
	N = n;
	stopTime = sTime;

	a = alpha;
	matrix = m;
	// zarezerwowanie odpowiedniej ilosci miejsca
	currentPath.reserve(N);
	bestPath.reserve(N);
	finalPath.reserve(N);

	// pseudolosowosc do prawdopodobienstwa
	srand(temperature + time(NULL));

	// zmienne zwiazane z pomiarem czasu
	long long int frequency = 0;
	long long int start = 0;
	long long int elapsed = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
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
// wyliczanie wagi aktualnej sciezki sciezki
double Genetic::countSum(vector<int> countPath)
{
	countPath.reserve(N);
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
// wyliczanie sasiedztwa metoda 2-opt
void Genetic::neighbourPath()
{
	int id1 = rand() % N;
	int id2 = rand() % N;

	while (id1 == id2) {
		id2 = rand() % N;
	}

	if (id1 > id2) {
		swap(id1, id2);
	}
	vector<int> newPath = currentPath;

	reverse(newPath.begin() + id1, newPath.begin() + id2 + 1);

	double newDistance = countSum(newPath);
	double currentDistance = countSum(currentPath);

	currentPath = newPath;
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::nextTemperature()
{
	temperature *= a;
}
//------------------------------------------------------------------------------------------------------------------------------------
long double Genetic::TSPGenetic()
{
	// rozpoczecie pomiaru czasu
	start = read_QPC();

	// algorytm
	geneticAlgorithm();

	// koniec algorytmu


	// koniec pomiaru czasu
	//elapsed = read_QPC() - start;
	long double timeElapsed = 0;
	//timeElapsed = ((1000.0 * elapsed) / frequency);

	// wyswietlenie wyniku i zapisanie do pliku
	fstream file;
	file.open("temp.txt", ios::out);
	file << N << endl;
	long double finalTemp = exp(-delta / temperature);

	/*cout << "\n\nWaga = " << finalSum << endl;
	cout << "Prawdopodobienstwo koncowe = " << finalTemp << endl;
	cout << "Temperatura koncowa = " << temperature << endl;
	cout << "Czas znaleznienia najlepszego rozwiazania w ms: " << setprecision(10) << finalElapsed << endl;
	cout << "Sciezka: ";

	for (int i = 0; i <= finalPath.size(); i++)
	{
		if (i != finalPath.size())
		{
			cout << finalPath[i] << "->";
			file << finalPath[i] << "\n";
		}
		else
		{
			cout << finalPath[0] << "\n";
			file << finalPath[0] << "\n";
		}
	}
	cout << "\n";
	file.close();*/

	return timeElapsed;
}
//------------------------------------------------------------------------------------------------------------------------------------
int Genetic::geneticAlgorithm()
{

	return 0;
}
//------------------------------------------------------------------------------------------------------------------------------------

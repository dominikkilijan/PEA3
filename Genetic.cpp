#include "Genetic.h"
#include "AdjacencyMatrix.h"
#include "NearestNeighbour.h"
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
#include <unordered_map>

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
	mutationValue = mut;
	crossoverValue = cross;
	crossoverChoice = cChoice;
	mutationChoice = mChoice;
	
	// zarezerwowanie odpowiedniej ilosci miejsca
	currentPath.reserve(N);
	bestPath.reserve(N);
	population.reserve(startingPopulation);

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
void Genetic::initPath(vector<int>& path)
{
	// tylko na czas random
	for (int i = 0; i < N; i++)
	{
		path.emplace_back(i);
	}
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::randomPath(vector<int>& path)
{	
	auto seed = static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count());
	auto rng = std::default_random_engine(seed);

	shuffle(begin(path), end(path), rng);
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::printPath(vector<int>& path)
{
	for (int i = 0; i < path.size(); i++)
	{
		cout << path[i] << " ";
	}
	cout << "\n";
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::printSpecimen(Specimen& a)
{
	cout << "path: \n";
	printPath(a.path);
	cout << "sum = " << a.sum << "\n";
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::initPopulation()
{
	for (int i = 0; i < startingPopulation; i++)
	{
		Specimen a;
		initPath(a.path);
		randomPath(a.path);
		a.sum = countSum(a.path);
		population.emplace_back(a);
	}
}
//------------------------------------------------------------------------------------------------------------------------------------
bool Genetic::compareSpecimen(Specimen& a, Specimen& b)
{
	return a.sum < b.sum;
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::sortPopulation()
{
	sort(population.begin(), population.end(), [&](Specimen& a, Specimen& b) {
		return compareSpecimen(a, b);
		});

	// survival of the fittest
	// zostawiamy 10% -- pozniej 10%. Do testowania to jest za malo aktualnie
	int idToErase = ceil(0.5 * startingPopulation);
	population.erase(population.begin() + idToErase, population.end());
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::nextGeneration()
{
	// chwilowy vector na dzieci
	vector<Specimen> newPopulation;
	int newChildren = startingPopulation - population.size() + 1;
	newPopulation.reserve(newChildren);

	// generowanie liczb losowych (rand() bylo niewystarczajace)
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> distribution(0, population.size()-1);
	
	Specimen child;

	// dodanie potomka na kazde wolne miejsce w tabeli
	for (int i = population.size(); i < startingPopulation; i++)
	{
		// wybor ktorzy rodzice. najlepsze rozwiazanie powinno miec szczegolne szanse, taki samiec alfa ktory plodzi duzo potomstwa
		int id1 = distribution(gen);
		int num = rand() % 100;
		if (num < 70)
			id1 = 0;

		int id2 = 0;
		do
		{
			id2 = distribution(gen);
		} while (id1 == id2); // srednio dziala dla malych populacji. na chwile tylko rodzice to jedynie 0 i 1

		child = crossover(population[id1], population[id2]);
		mutation(child);
		newPopulation.emplace_back(child);
	}
	// polaczenie najsilniejszych osobnikow z nowym pokoleniem	
	population.insert(population.end(), newPopulation.begin(), newPopulation.end());
}
//------------------------------------------------------------------------------------------------------------------------------------
Specimen Genetic::crossover(Specimen& p1, Specimen& p2)
{
	if (crossoverChoice == 1)
	{
		return orderedCrossover(p1, p2);
	}
	else if (crossoverChoice == 2)
		 return PMXCrossover(p1, p2);
}
//------------------------------------------------------------------------------------------------------------------------------------
Specimen Genetic::orderedCrossover(Specimen& p1, Specimen& p2)
{
	Specimen child;
	child.path.resize(N);
	vector<int> copiedSequence;

	int toCross = floor((1-crossoverValue) * N);
	int toFill = N - toCross;
	int start = rand() % toFill;
	int end = start + toCross - 1;
	copiedSequence.reserve(toCross);

	// kopia sekwencji od pierwszego rodzica
	for (int i = start; i <= end; i++)
	{
		if (i < child.path.size()) // ostroznosci nigdy za wiele
		{
			child.path[i] = p1.path[i];
			copiedSequence.emplace_back(p1.path[i]);
		}
	}

	// uzupelnianie reszty
	int endID = (end + 1) % N; // indeks zaraz za skopiowana sekwencja
	// petla tyle razy ile jest wolnych miejsc (bez skopiowanej sekwencji)
	int maxIter = endID + toFill;
	for (int i = endID; i < maxIter; i++)
	{
		// czy wierzcholek byl w skopiowanej sekwencji
		while (find(copiedSequence.begin(), copiedSequence.end(), p2.path[endID]) != copiedSequence.end())
		{
			endID = (endID + 1) % N; // jesli byl to bierzemy kolejny wierzcholek od p2
		}

		child.path[(i % N)] = p2.path[endID]; // Jesli skonczylo sie miejsce za sekwencja to idziemy do poczatku
		endID = (endID + 1) % N;			
	}
	child.sum = countSum(child.path);

	return child;
}
//------------------------------------------------------------------------------------------------------------------------------------
Specimen Genetic::PMXCrossover(Specimen& p1, Specimen& p2)
{
	Specimen child;
	child.path.resize(N);
	unordered_map<int, int> pairsOfVertices;

	int toCross = floor((1-crossoverValue) * N);
	int toFill = N - toCross;
	int start = rand() % toFill;
	int end = start + toCross - 1;

	// kopia sekwencji od pierwszego rodzica
	for (int i = start; i <= end; i++)
	{
		if (i < child.path.size()) // ostroznosci nigdy za wiele
		{
			child.path[i] = p1.path[i];
			pairsOfVertices[p1.path[i]] = p2.path[i];
		}
	}

	// uzupelnianie reszty
	for (int i = 0; i < N; i++)
	{
		if (i < start || i > end)
		{
			int current = p2.path[i];
			while (pairsOfVertices.find(current) != pairsOfVertices.end())
			{
				current = pairsOfVertices[current];
			}
			child.path[i] = current;
		}
	}

	child.sum = countSum(child.path);
	return child;
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::mutation(Specimen& a)
{
	if (mutationChoice == 1)
		invertGenes(a);
	else if (mutationChoice == 2)
		swapGenes(a);
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::invertGenes(Specimen& a) 
{	
	int	toMutate = floor(mutationValue * N);
	if (toMutate < 4)
		toMutate += 4;

	int num = N - toMutate;

	int start = rand() % num;
	int end = start + toMutate;

	reverse(a.path.begin() + start, a.path.begin() + end);

	a.sum = countSum(a.path);
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::swapGenes(Specimen& a)
{
	// generowanie liczb losowych (rand() bylo niewystarczajace)
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> distribution(0, N - 1);

	for (int i = 0; i < (mutationValue * 100); i++)
	{
		int id1 = distribution(gen);
		cout << i << endl;
		int id2 = 0;
		do
		{
			id2 = distribution(gen);
		} while (id1 == id2);
	
		iter_swap(a.path.begin() + id1, a.path.begin() + id2);

		a.sum = countSum(a.path);
	}
}
//------------------------------------------------------------------------------------------------------------------------------------
void Genetic::TSPGenetic()
{
	// rozpoczecie pomiaru czasu
	start = read_QPC();
	
	// algorytm
	geneticAlgorithm();
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
	//// greedy sciezka
	//NearestNeighbour nn(N, matrix, bestPath);
	//nn.findNearestNeighbourPath();
	
	// poczatkowa populacja
	initPopulation();

	// poczatkowe wyniki
	bestSum = population[0].sum;
	bestPath = population[0].path;
	

	// bedzie wytlumaczone w petli
	sortPopulation();
	if (population[0].sum < bestSum)
	{
		bestSum = population[0].sum;
		bestPath = population[0].path;
		QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
		bestElapsed = ((1000.0 * (read_QPC() - start)) / frequency);

		cout << "bestSum = " << bestSum << "\n";
		cout << "Znalezienie bestSum w ms: " << setprecision(10) << bestElapsed << endl;
	}
	// tego tak w sumie tutaj nie trzeba jesli randomPath dziala dobrze. Co innego jesli robimy nn wiec zostawie zakomentowane
	/*for (int i = 0; i < startingPopulation; i++)
	{
		mutation(population[i]);
		printPath(population[i].path);
	}*/

	// przygotowanie minutnika algorytmu
	const time_point<system_clock> startTime = system_clock::now();
	seconds stopTimeSeconds = seconds(stopTime);
	seconds finalTime;
	
	// petla jesli nie skonczyl sie czas
	while ((system_clock::now() - startTime) < stopTimeSeconds)
	//for (int i = 0; i < 1; i++)
	{
		// tworzymy dzieci crossoverem tj losujemy dwoch rodzicow -> crossover -> mutacja -> dodajemy do vectora az bedzie pelny
		nextGeneration();
		// sortowanie populacji. Zostawiamy 10% populacji, reszte usuwamy
		sortPopulation();
		// population jest posortowane wedlug wagi wiec najlepsze rozwiazanie jest na pierwszym miejscu
		if (population[0].sum < bestSum)
		{
			// jesli rozwiazanie jest lepsze niz poprzednie
			bestSum = population[0].sum;
			bestPath = population[0].path;

			// zapisanie i wyswietlenie czasu znalezienia rozwiazania
			QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
			bestElapsed = ((1000.0 * (read_QPC() - start)) / frequency);

			cout << "bestSum = " << bestSum << "\n";
			cout << "Znalezienie bestSum w ms: " << setprecision(10) << bestElapsed << endl;
		}
	}
}
//------------------------------------------------------------------------------------------------------------------------------------

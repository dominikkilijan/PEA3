#pragma once
#include <vector>

using namespace std;


class Genetic
{
public:
	Genetic(int, int**, int, double, double, double, int, int ); // wierzcholki, macierz, stopTime, startingPopulation, 
																 // mutation, crossover, crossoverChoice, mutationChoice
	~Genetic();
	long long int read_QPC();
	double countSum(vector<int>&);
	void randomPath();
	void printCurrentPath();
	void PrintBestPath();
	void TSPGenetic();
	void geneticAlgorithm();


private:
	vector<int> currentPath;
	vector<int> bestPath;

	int N;
	int** matrix;

	int stopTime;
	double startingPopulation;
	double mutation;
	double crossover;
	int crossoverChoice;
	int mutationChoice;

	long int bestSum, currentSum;

	// zmienne do czasu
	long double bestElapsed;
	long long int start, elapsed, frequency;
};


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
	double countSum(vector<int>);
	void startingPath();
	void randomPath();
	void startingTemperature();
	long double TSPGenetic();
	int geneticAlgorithm();
	void printCurrentPath();
	void PrintBestPath();
	void neighbourPath();
	bool probability();
	void nextTemperature();


private:
	vector<int> currentPath;
	vector<int> bestPath;
	vector<int> finalPath;

	//int stopTime, double startingPopulation, double mutation, double crossover, int crossoverChoice, int mutationChoice
	int N;
	int** matrix;

	int stopTime;
	double startingPopulation;
	double mutation;
	double crossover;
	int crossoverChoice;
	int mutationChoice;

	double bestSum, currentSum, finalSum;

	long double finalElapsed;
	long long int start, elapsed, frequency;
};


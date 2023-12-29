#pragma once
#include <vector>

using namespace std;

struct Specimen
{
	vector<int> path;
	long int sum;
};


class Genetic
{
public:
	Genetic(int, int**, int, double, double, double, int, int ); // wierzcholki, macierz, stopTime, startingPopulation, 
																 // mutation, crossover, crossoverChoice, mutationChoice
	~Genetic();
	long long int read_QPC();
	double countSum(vector<int>&);
	void initPath(vector<int>&);
	void randomPath(vector<int>&);

	void printPath(vector<int>&);

	void initPopulation();
	bool compareSpecimen(Specimen&, Specimen&);
	void nextGeneration();



	Specimen crossover(Specimen&, Specimen&);
	Specimen PMXCross(Specimen&, Specimen&);
	Specimen orderedCross(Specimen&, Specimen&);

	void mutation(Specimen&);
	void swapGenes(Specimen&);
	void invertGenes(Specimen&);

	void TSPGenetic();
	void geneticAlgorithm();


private:
	vector<Specimen> population;
	vector<int> currentPath;
	vector<int> bestPath;
	vector<int> finalPath;

	int N;
	int** matrix;

	int stopTime;
	double startingPopulation;
	double mutationValue;
	double crossoverValue;
	int crossoverChoice;
	int mutationChoice;

	long int bestSum, currentSum, finalSum;

	// zmienne do czasu
	long double bestElapsed;
	long long int start, elapsed, frequency;
};


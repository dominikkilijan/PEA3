#pragma once
#include <fstream>

using namespace std;

class AdjacencyMatrix
{
public:
	~AdjacencyMatrix();

	void fillFromFile(fstream*);
	void fillFromFileXML(fstream*);
	void printAdjacencyMatrix();
	void deleteAdjacencyMatrix();
	void createAdjacencyMatrix();
	void runAlgorithm(int, double, double, double, int, int);
	int** getAdjacencyMatrix();


private:
	int N;
	int** matrix;
	long double sumElapsed;


};


